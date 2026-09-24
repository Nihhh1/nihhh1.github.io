import { readFile, writeFile, realpath, mkdir } from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { webcrypto } from 'node:crypto';
import readline from 'node:readline/promises';
import { Writable } from 'node:stream';
import MarkdownIt from 'markdown-it';
import YAML from 'yaml';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const iterations = 600000;
const markdown = new MarkdownIt({ html: false, linkify: true });
const base64 = value => Buffer.from(value).toString('base64');

export async function encryptPost(source, password) {
  if ([...password].length < 12) throw new Error('密码至少需要 12 个字符，建议使用随机长密码。');
  const match = source.replace(/^\uFEFF/, '').match(/^---\r?\n([\s\S]*?)\r?\n---(?:\r?\n|$)([\s\S]*)$/);
  if (!match) throw new Error('文章必须包含 YAML front matter。');
  const metadata = YAML.parse(match[1]);
  if (!metadata || typeof metadata.title !== 'string') throw new Error('请设置公开的 title。');
  if (metadata.encrypted) throw new Error('输入已经是加密文章，请使用原文。');
  if (!match[2].trim()) throw new Error('正文不能为空。');
  // Only explicitly public metadata is copied. Never publish arbitrary source fields.
  const publicData = { layout: 'post', title: metadata.title, encrypted: true, catalog: false };
  for (const key of ['date', 'author', 'tags', 'categories', 'permalink']) {
    if (metadata[key] !== undefined) publicData[key] = metadata[key];
  }
  const salt = webcrypto.getRandomValues(new Uint8Array(16));
  const iv = webcrypto.getRandomValues(new Uint8Array(12));
  const material = await webcrypto.subtle.importKey('raw', new TextEncoder().encode(password), 'PBKDF2', false, ['deriveKey']);
  const key = await webcrypto.subtle.deriveKey({ name: 'PBKDF2', hash: 'SHA-256', salt, iterations }, material, { name: 'AES-GCM', length: 256 }, false, ['encrypt']);
  const data = await webcrypto.subtle.encrypt({ name: 'AES-GCM', iv }, key, new TextEncoder().encode(markdown.render(match[2])));
  publicData.encrypted_payload = { version: 1, iterations, salt: base64(salt), iv: base64(iv), data: base64(data) };
  return `---\n${YAML.stringify(publicData)}---\n\n本文已加密，请在文章页面输入密码阅读。\n`;
}

async function askPassword(prompt) {
  if (!process.stdin.isTTY) throw new Error('请在交互式终端运行，密码不接受命令行参数或文件。');
  process.stdout.write(prompt);
  const silent = new Writable({ write(_chunk, _encoding, callback) { callback(); } });
  const rl = readline.createInterface({ input: process.stdin, output: silent, terminal: true });
  try { return await rl.question(''); }
  finally { rl.close(); process.stdout.write('\n'); }
}

async function main() {
  const args = process.argv.slice(2);
  if (args.length !== 1) throw new Error('用法：npm run encrypt -- _private/YYYY-MM-DD-文章名.md');
  const input = await realpath(path.resolve(args[0]));
  const privateRoot = await realpath(path.join(root, '_private'));
  if (!input.startsWith(privateRoot + path.sep)) throw new Error('原文必须放在 _private/ 中，避免误提交。');
  const name = path.basename(input);
  if (!/^\d{4}-\d{1,2}-\d{1,2}-.+\.md$/.test(name)) throw new Error('文件名必须是 YYYY-MM-DD-文章名.md。');
  const source = await readFile(input, 'utf8');
  const password = await askPassword('设置文章密码（至少 12 字符，输入不显示）：');
  const confirmation = await askPassword('再次输入密码：');
  if (password !== confirmation) throw new Error('两次密码不一致，未写入文件。');
  const output = await encryptPost(source, password);
  const target = path.join(root, '_posts', name);
  try {
    const existing = await readFile(target, 'utf8');
    if (!/^encrypted: true$/m.test(existing)) throw new Error('同名公开文章已存在。请先备份并移走原文；已公开的 Git 历史不会因加密消失。');
  } catch (error) { if (error.code !== 'ENOENT') throw error; }
  await mkdir(path.dirname(target), { recursive: true });
  await writeFile(target, output, 'utf8');
  console.log(`已生成：_posts/${name}（仅含公开元数据和密文）`);
}

if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
  main().catch(error => { console.error(error.message); process.exitCode = 1; });
}
