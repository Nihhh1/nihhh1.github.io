import test from 'node:test';
import assert from 'node:assert/strict';
import { webcrypto } from 'node:crypto';
import { readFile } from 'node:fs/promises';
import vm from 'node:vm';
import YAML from 'yaml';
import { encryptPost } from './encrypt-post.mjs';

const password = 'test-only-密码-123456';
const secret = 'PRIVATE_SENTINEL_不应出现在发布产物';
const source = `---\ntitle: 公开标题\nsubtitle: 秘密副标题\npassword: 不应被复制\nexcerpt: 不应被复制\ntags: [测试]\n---\n# ${secret}\n\n**中文正文**\n\n<script>alert(1)</script>\n\n{{ site.secret }}\n`;
const getPayload = output => YAML.parse(output.split('---\n')[1]).encrypted_payload;
async function decrypt(payload, pass) {
    const material = await webcrypto.subtle.importKey('raw', new TextEncoder().encode(pass), 'PBKDF2', false, ['deriveKey']);
    const key = await webcrypto.subtle.deriveKey({ name: 'PBKDF2', hash: 'SHA-256', salt: Buffer.from(payload.salt, 'base64'), iterations: payload.iterations }, material, { name: 'AES-GCM', length: 256 }, false, ['decrypt']);
    return new TextDecoder().decode(await webcrypto.subtle.decrypt({ name: 'AES-GCM', iv: Buffer.from(payload.iv, 'base64') }, key, Buffer.from(payload.data, 'base64')));
}

test('encrypted output omits plaintext and private metadata; Unicode HTML round trip', async () => {
    const output = await encryptPost(source, password);
    for (const value of [secret, password, '秘密副标题', '不应被复制', '<script>']) assert.ok(!output.includes(value));
    const html = await decrypt(getPayload(output), password);
    assert.ok(html.includes(secret));
    assert.ok(html.includes('<strong>中文正文</strong>'));
    assert.ok(!html.includes('<script>'));
    assert.ok(html.includes('{{ site.secret }}'));
});

test('wrong password and tampering fail authentication; each encryption is random', async () => {
    const first = getPayload(await encryptPost(source, password));
    const second = getPayload(await encryptPost(source, password));
    assert.notEqual(first.salt, second.salt);
    assert.notEqual(first.iv, second.iv);
    await assert.rejects(decrypt(first, 'wrong-password'));
    const data = Buffer.from(first.data, 'base64'); data[0] ^= 1;
    await assert.rejects(decrypt({ ...first, data: data.toString('base64') }, password));
});

test('reject short passwords and invalid or already encrypted source', async () => {
    await assert.rejects(encryptPost(source, 'short'));
    await assert.rejects(encryptPost('no metadata', password));
    await assert.rejects(encryptPost(await encryptPost(source, password), password));
});

test('browser script: wrong password, unlock, relock, and pagehide', async () => {
    const payload = getPayload(await encryptPost(source, password));
    const elements = {};
    for (const id of ['unlock-form', 'post-password', 'unlock-status', 'decrypted-content', 'lock-post', 'encrypted-payload', 'submit']) {
        elements[id] = { hidden: false, value: '', textContent: '', innerHTML: '', handlers: {},
            addEventListener(event, handler) { this.handlers[event] = handler; }, focus() {},
            replaceChildren() { this.innerHTML = ''; }, querySelector() { return elements.submit; } };
    }
    elements['encrypted-payload'].textContent = JSON.stringify(payload);
    const window = { crypto: webcrypto, addEventListener(event, handler) { this[event] = handler; } };
    vm.runInNewContext(await readFile(new URL('../js/encrypted-post.js', import.meta.url), 'utf8'), {
        document: { getElementById: id => elements[id] }, window, crypto: webcrypto, TextEncoder, TextDecoder, Uint8Array, atob
    });
    const submit = async pass => { elements['post-password'].value = pass; await elements['unlock-form'].handlers.submit({ preventDefault() {} }); };
    await submit('wrong');
    assert.match(elements['unlock-status'].textContent, /密码错误/);
    assert.equal(elements['decrypted-content'].innerHTML, '');
    await submit(password);
    assert.ok(elements['decrypted-content'].innerHTML.includes(secret));
    assert.equal(elements['unlock-form'].hidden, true);
    assert.equal(elements['post-password'].value, '');
    elements['lock-post'].handlers.click();
    assert.equal(elements['decrypted-content'].innerHTML, '');
    assert.equal(elements['unlock-form'].hidden, false);
    await submit(password);
    window.pagehide();
    assert.equal(elements['decrypted-content'].innerHTML, '');
});
