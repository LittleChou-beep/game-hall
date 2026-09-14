/**
 * 服务入口
 * 启动：npm start （先确保 MySQL 已运行、已执行 npm run init-db）
 */
const express = require('express');
const cors = require('cors');
const path = require('path');
require('dotenv').config({ path: require('path').join(__dirname, '../.env') });

const { pool, query } = require('./db');
const { authRequired } = require('./middleware');
const authRoutes = require('./auth');
const recordRoutes = require('./records');

const app = express();
app.use(cors());
app.use(express.json());

// 托管前端静态文件（index.html 等），生产环境由后端统一提供服务
app.use(express.static(path.join(__dirname, '../../')));

// 极简请求日志（不引额外依赖）
app.use((req, res, next) => {
  const t = Date.now();
  res.on('finish', () => {
    console.log(`[${new Date().toISOString().slice(11, 19)}] ${req.method} ${req.originalUrl} ${res.statusCode} ${Date.now() - t}ms`);
  });
  next();
});

// 健康检查（顺便探测数据库连通性）
app.get('/api/health', async (req, res) => {
  try {
    await pool.query('SELECT 1');
    res.json({ status: 'ok', db: 'connected' });
  } catch (e) {
    res.status(503).json({ status: 'error', db: e.message });
  }
});

/* 个人中心：账号信息 + 总览统计 + 各游戏分布 */
app.get('/api/profile', authRequired, async (req, res) => {
  const users = await query(
    `SELECT username, DATE_FORMAT(created_at, '%Y-%m-%d') AS createdAt
       FROM users WHERE id = ? LIMIT 1`,
    [req.user.id]
  );
  if (users.length === 0) {
    return res.status(404).json({ error: '用户不存在' });
  }

  const statsRow = await query(
    `SELECT COUNT(*) AS total,
            SUM(result = 'win')  AS win,
            SUM(result = 'lose') AS lose,
            SUM(result = 'draw') AS draw,
            SUM(result = 'score') AS score
       FROM game_records WHERE user_id = ?`,
    [req.user.id]
  );
  const s = statsRow[0];
  const win = Number(s.win), lose = Number(s.lose);
  const decided = win + lose;

  const gameRows = await query(
    `SELECT game_name AS game,
            COUNT(*) AS total,
            SUM(result = 'win')  AS win,
            SUM(result = 'lose') AS lose,
            SUM(result = 'draw') AS draw,
            SUM(result = 'score') AS score
       FROM game_records WHERE user_id = ?
      GROUP BY game_name
      ORDER BY total DESC, game_name ASC`,
    [req.user.id]
  );
  const games = gameRows.map(g => {
    const gWin = Number(g.win), gLose = Number(g.lose), gDecided = gWin + gLose;
    return {
      game: g.game,
      total: Number(g.total),
      win: gWin, lose: gLose, draw: Number(g.draw), score: Number(g.score),
      rate: gDecided === 0 ? null : Math.round((gWin / gDecided) * 100)
    };
  });

  res.json({
    username: users[0].username,
    createdAt: users[0].createdAt,
    stats: {
      total: Number(s.total), win, lose,
      draw: Number(s.draw), score: Number(s.score),
      rate: decided === 0 ? 0 : Math.round((win / decided) * 100)
    },
    games
  });
});

/* 公开排行榜：全局胜负榜 + 各分数型游戏用时榜（无需登录） */
app.get('/api/leaderboard', async (req, res) => {
  try {
    // 全局胜负榜：按胜场降序，胜场相同再按总局数降序，最多 10 条
    const globalRows = await query(
      `SELECT u.username,
              COUNT(*)              AS total,
              SUM(r.result = 'win')  AS win,
              SUM(r.result = 'lose') AS lose
         FROM game_records r
         JOIN users u ON r.user_id = u.id
        GROUP BY u.id
        ORDER BY win DESC, total DESC
        LIMIT 10`
    );
    const global = globalRows.map(g => {
      const win = Number(g.win), lose = Number(g.lose);
      const decided = win + lose;
      return {
        username: g.username,
        win, lose,
        total: Number(g.total),
        // 胜率 = 胜场 / (胜+负)，无胜负局时记 0
        rate: decided === 0 ? 0 : Math.round((win / decided) * 100)
      };
    });

    // 按游戏排行：只取 result='score' 的记录，每游戏取 top3，按 duration 降序
    // 先列出所有出现过 score 记录的游戏名（按名排序，只含有记录的游戏）
    const gameRows = await query(
      `SELECT DISTINCT game_name AS game
         FROM game_records
        WHERE result = 'score'
        ORDER BY game_name ASC`
    );
    const byGame = [];
    for (const g of gameRows) {
      const tops = await query(
        `SELECT u.username, r.detail, r.duration
           FROM game_records r
           JOIN users u ON r.user_id = u.id
          WHERE r.game_name = ? AND r.result = 'score'
          ORDER BY r.duration DESC
          LIMIT 3`,
        [g.game]
      );
      byGame.push({
        game: g.game,
        tops: tops.map(t => ({
          username: t.username,
          detail: t.detail,
          duration: Number(t.duration)
        }))
      });
    }

    res.json({ global, byGame });
  } catch (e) {
    console.error('[leaderboard error]', e);
    res.status(500).json({ error: '服务器内部错误' });
  }
});

app.use('/api/auth', authRoutes);
app.use('/api/records', recordRoutes);

// 非 API 路由返回 index.html（前端单页应用）
app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, '../../index.html'));
});

// 兜底错误处理
app.use((err, req, res, next) => {
  console.error('[server error]', err);
  res.status(500).json({ error: '服务器内部错误' });
});

const PORT = Number(process.env.PORT) || 3000;

// 启动前先探测数据库，失败时给出明确提示
async function start() {
  try {
    await pool.query('SELECT 1');
  } catch (e) {
    console.error('无法连接 MySQL：' + e.message);
    console.error('请确认：1) MySQL 已启动  2) backend/.env 配置正确  3) 已执行 npm run init-db');
    process.exit(1);
  }
  app.listen(PORT, () => {
    console.log('游戏大厅后端已启动：http://localhost:' + PORT);
  });
}

start();
