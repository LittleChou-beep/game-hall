/**
 * 账号相关路由：注册、登录、获取当前登录用户
 */
const express = require('express');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const { query } = require('./db');
const { SECRET, authRequired } = require('./middleware');

const router = express.Router();

/* POST /api/auth/register  { username, password } */
router.post('/register', async (req, res) => {
  const username = String(req.body.username || '').trim();
  const password = String(req.body.password || '');

  if (username.length < 3 || username.length > 20) {
    return res.status(400).json({ error: '用户名需为 3~20 个字符' });
  }
  if (!/^[A-Za-z0-9_\u4e00-\u9fa5]+$/.test(username)) {
    return res.status(400).json({ error: '用户名只能包含字母、数字、下划线或中文' });
  }
  if (password.length < 6 || password.length > 32) {
    return res.status(400).json({ error: '密码长度需为 6~32 位' });
  }

  const exists = await query('SELECT id FROM users WHERE username = ? LIMIT 1', [username]);
  if (exists.length > 0) {
    return res.status(409).json({ error: '该用户名已被注册' });
  }

  const hash = await bcrypt.hash(password, 10);
  const result = await query(
    'INSERT INTO users (username, password_hash) VALUES (?, ?)',
    [username, hash]
  );

  const token = jwt.sign({ id: result.insertId, username, v: 0 }, SECRET, { expiresIn: '7d' });
  res.status(201).json({ token, username });
});

/* POST /api/auth/login  { username, password } */
router.post('/login', async (req, res) => {
  const username = String(req.body.username || '').trim();
  const password = String(req.body.password || '');

  if (!username || !password) {
    return res.status(400).json({ error: '请输入用户名和密码' });
  }

  const rows = await query(
    'SELECT id, username, password_hash, token_version FROM users WHERE username = ? LIMIT 1',
    [username]
  );
  if (rows.length === 0) {
    return res.status(401).json({ error: '用户名或密码错误' });
  }

  const user = rows[0];
  const ok = await bcrypt.compare(password, user.password_hash);
  if (!ok) {
    return res.status(401).json({ error: '用户名或密码错误' });
  }

  const token = jwt.sign(
    { id: user.id, username: user.username, v: user.token_version },
    SECRET,
    { expiresIn: '7d' }
  );
  res.json({ token, username: user.username });
});

/* GET /api/auth/me  验证 token 是否有效 */
router.get('/me', authRequired, (req, res) => {
  res.json({ username: req.user.username });
});

/* POST /api/auth/change-password  { oldPassword, newPassword }
 * 必须登录；先校验旧密码，再把新密码 bcrypt 哈希后更新 */
router.post('/change-password', authRequired, async (req, res) => {
  const oldPassword = String(req.body.oldPassword || '');
  const newPassword = String(req.body.newPassword || '');

  if (!oldPassword || !newPassword) {
    return res.status(400).json({ error: '请填写旧密码和新密码' });
  }
  if (newPassword.length < 6 || newPassword.length > 32) {
    return res.status(400).json({ error: '新密码长度需为 6~32 位' });
  }
  if (oldPassword === newPassword) {
    return res.status(400).json({ error: '新密码不能与旧密码相同' });
  }

  const rows = await query('SELECT password_hash FROM users WHERE id = ? LIMIT 1', [req.user.id]);
  if (rows.length === 0) {
    return res.status(404).json({ error: '用户不存在' });
  }

  const ok = await bcrypt.compare(oldPassword, rows[0].password_hash);
  if (!ok) {
    // 已登录但旧密码错误属于业务校验失败，用 400 而非 401，
    // 避免前端把它当成"登录失效"而误清 token
    return res.status(400).json({ error: '旧密码不正确' });
  }

  const newHash = await bcrypt.hash(newPassword, 10);
  // 提升 token 版本号：所有旧设备上的 token 立即失效
  await query(
    'UPDATE users SET password_hash = ?, token_version = token_version + 1 WHERE id = ?',
    [newHash, req.user.id]
  );

  // 给当前会话签发新版本 token，无需重新登录
  const newToken = jwt.sign(
    { id: req.user.id, username: req.user.username, v: req.user.v + 1 },
    SECRET,
    { expiresIn: '7d' }
  );
  res.json({ ok: true, message: '密码修改成功', token: newToken });
});

module.exports = router;
