/**
 * 游戏记录路由（全部需要登录，按用户隔离）
 *  POST   /api/records        上报一条记录
 *  GET    /api/records        查询我的记录（含统计）
 *  DELETE /api/records        清空我的记录
 */
const express = require('express');
const { query } = require('./db');
const { authRequired } = require('./middleware');

const router = express.Router();
router.use(authRequired); // 该路由下所有接口都要登录

const VALID_RESULT = ['win', 'lose', 'draw', 'score'];

/* 上报一条游戏记录 */
router.post('/', async (req, res) => {
  const gameName = String(req.body.gameName || '').trim();
  const result = String(req.body.result || '');
  const detail = String(req.body.detail || '').slice(0, 200);
  const duration = Math.max(0, parseInt(req.body.duration, 10) || 0);

  if (!gameName || gameName.length > 32) {
    return res.status(400).json({ error: '游戏名无效' });
  }
  if (!VALID_RESULT.includes(result)) {
    return res.status(400).json({ error: '结果必须是 win/lose/draw' });
  }

  await query(
    'INSERT INTO game_records (user_id, game_name, result, detail, duration) VALUES (?, ?, ?, ?, ?)',
    [req.user.id, gameName, result, detail, duration]
  );
  res.status(201).json({ ok: true });
});

/* 查询我的记录 + 统计 */
router.get('/', async (req, res) => {
  const records = await query(
    `SELECT id, game_name AS gameName, result, detail, duration,
            DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s') AS time
       FROM game_records
      WHERE user_id = ?
      ORDER BY created_at DESC, id DESC
      LIMIT 200`,
    [req.user.id]
  );

  // 统计：总场次、胜、负、平、胜率（平局/得分不计入胜率分母）
  const statsRow = await query(
    `SELECT
       COUNT(*) AS total,
       SUM(result = 'win')  AS win,
       SUM(result = 'lose') AS lose,
       SUM(result = 'draw') AS draw,
       SUM(result = 'score') AS score
     FROM game_records WHERE user_id = ?`,
    [req.user.id]
  );
  const s = statsRow[0];
  const decided = Number(s.win) + Number(s.lose);
  const rate = decided === 0 ? 0 : Math.round((Number(s.win) / decided) * 100);

  res.json({
    stats: {
      total: Number(s.total),
      win: Number(s.win),
      lose: Number(s.lose),
      draw: Number(s.draw),
      score: Number(s.score),
      rate
    },
    records
  });
});

/* 清空我的记录 */
router.delete('/', async (req, res) => {
  await query('DELETE FROM game_records WHERE user_id = ?', [req.user.id]);
  res.json({ ok: true });
});

/* 删除单条记录（必须是本人的记录，防止越权删除）*/
router.delete('/:id', async (req, res) => {
  const id = parseInt(req.params.id, 10);
  if (!Number.isInteger(id) || id <= 0) {
    return res.status(400).json({ error: '记录 id 无效' });
  }
  const result = await query(
    'DELETE FROM game_records WHERE id = ? AND user_id = ?',
    [id, req.user.id]
  );
  if (result.affectedRows === 0) {
    return res.status(404).json({ error: '记录不存在或无权删除' });
  }
  res.json({ ok: true });
});

module.exports = router;
