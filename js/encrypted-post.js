/* Payload v1: PBKDF2-SHA256 / AES-256-GCM; no persistent password or plaintext storage. */
(() => {
    'use strict';
    const form = document.getElementById('unlock-form');
    if (!form) return;
    const input = document.getElementById('post-password');
    const status = document.getElementById('unlock-status');
    const content = document.getElementById('decrypted-content');
    const lock = document.getElementById('lock-post');
    const submit = form.querySelector('button');
    const bytes = value => Uint8Array.from(atob(value), char => char.charCodeAt(0));
    if (!window.crypto || !window.crypto.subtle) {
        status.textContent = '当前环境不支持安全解密，请使用现代浏览器通过 HTTPS 或 localhost 访问。';
        submit.disabled = true;
        return;
    }
    form.addEventListener('submit', async event => {
        event.preventDefault();
        submit.disabled = true;
        status.textContent = '正在解密…';
        try {
            const payload = JSON.parse(document.getElementById('encrypted-payload').textContent);
            if (payload.version !== 1 || payload.iterations !== 600000) throw new Error('Unsupported payload');
            const material = await crypto.subtle.importKey('raw', new TextEncoder().encode(input.value), 'PBKDF2', false, ['deriveKey']);
            input.value = '';
            const key = await crypto.subtle.deriveKey({ name: 'PBKDF2', hash: 'SHA-256', salt: bytes(payload.salt), iterations: payload.iterations }, material, { name: 'AES-GCM', length: 256 }, false, ['decrypt']);
            const plaintext = await crypto.subtle.decrypt({ name: 'AES-GCM', iv: bytes(payload.iv) }, key, bytes(payload.data));
            content.innerHTML = new TextDecoder().decode(plaintext);
            content.hidden = false;
            lock.hidden = false;
            form.hidden = true;
            status.textContent = '';
            lock.focus();
        } catch (_error) {
            status.textContent = '密码错误或文章数据损坏，请重新输入。';
            input.focus();
        } finally {
            input.value = '';
            submit.disabled = false;
        }
    });
    function relock() {
        content.replaceChildren();
        content.hidden = true;
        lock.hidden = true;
        form.hidden = false;
        input.value = '';
        status.textContent = '';
    }
    lock.addEventListener('click', () => { relock(); input.focus(); });
    window.addEventListener('pagehide', relock);
})();
