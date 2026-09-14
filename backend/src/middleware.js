/**
 * JWT 鉴权中间件
 * 前端请求需带请求头：Authorization: Bearer <token>
 * 验证通过后把用户信息挂到 req.user。
 * token 中带版本号 v，与 users.token_version 不一致即视为已失效
 * （修改密码后会提升版本号，旧设备上的 token 立即作废）。
 */
const jwt = require('jsonwebtoken');
const { query } = require('./db');

const SECRET = process.env.JWT_SECRET || 'dev_secret';

async function authRequired(req, res, next) {
  try {
    const header = req.headers.authorization || '';
    const token = header.startsWith('Bearer ') ? header.slice(7) : null;
    if (!token) {
      return res.status(401).json({ error: '未登录，请先登录' });
    }

    let payload;
    try {
      payload = jwt.verify(token, SECRET);
    } catch (e) {
      return res.status(401).json({ error: '登录已过期，请重新登录' });
    }

    // 校验 token 版本（改密后旧 token 的 v 会落后）
    const rows = await query(
      'SELECT username, token_version FROM users WHERE id = ? LIMIT 1',
      [payload.id]
    );
    if (rows.length === 0) {
      return res.status(401).json({ error: '账号不存在，请重新登录' });
    }
    const tokenV = Number.isInteger(payload.v) ? payload.v : 0;
    if (tokenV !== rows[0].token_version) {
      return res.status(401).json({ error: '登录状态已失效（可能密码已修改），请重新登录' });
    }

    req.user = { id: payload.id, username: rows[0].username, v: tokenV };
    next();
  } catch (err) {
    next(err);
  }
}

module.exports = { authRequired, SECRET };
