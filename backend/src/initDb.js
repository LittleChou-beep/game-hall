/**
 * 数据库初始化：自动创建数据库和两张表。
 * 幂等，可重复执行（CREATE ... IF NOT EXISTS + 列存在性检查）。
 * 运行：npm run init-db
 */
const mysql = require('mysql2/promise');
require('dotenv').config();

const DB_NAME = process.env.DB_NAME || 'game_hall';

const SCHEMA = `
CREATE DATABASE IF NOT EXISTS \`${DB_NAME}\`
  DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_unicode_ci;

USE \`${DB_NAME}\`;

CREATE TABLE IF NOT EXISTS users (
  id            INT UNSIGNED NOT NULL AUTO_INCREMENT,
  username      VARCHAR(32)  NOT NULL,
  password_hash VARCHAR(100) NOT NULL,
  token_version INT UNSIGNED NOT NULL DEFAULT 0,
  created_at    DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id),
  UNIQUE KEY uk_username (username)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS game_records (
  id         INT UNSIGNED NOT NULL AUTO_INCREMENT,
  user_id    INT UNSIGNED NOT NULL,
  game_name  VARCHAR(32)  NOT NULL,
  result     ENUM('win','lose','draw','score') NOT NULL,
  detail     VARCHAR(200) NOT NULL DEFAULT '',
  duration   INT UNSIGNED NOT NULL DEFAULT 0,
  created_at DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id),
  KEY idx_user_time (user_id, created_at),
  CONSTRAINT fk_records_user FOREIGN KEY (user_id)
    REFERENCES users (id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
`;

/** 判断某列是否已存在（MySQL 8 没有 ADD COLUMN IF NOT EXISTS） */
async function columnExists(conn, table, column) {
  const [rows] = await conn.query(
    `SELECT 1 FROM information_schema.columns
      WHERE table_schema = ? AND table_name = ? AND column_name = ? LIMIT 1`,
    [DB_NAME, table, column]
  );
  return rows.length > 0;
}

async function main() {
  // 先不指定 database 连接，以便创建库本身
  const conn = await mysql.createConnection({
    host: process.env.DB_HOST || '127.0.0.1',
    port: Number(process.env.DB_PORT) || 3306,
    user: process.env.DB_USER || 'root',
    password: process.env.DB_PASSWORD || '',
    multipleStatements: true
  });

  await conn.query(SCHEMA);

  // ---- 旧库迁移（全部幂等）----
  // 1) users.token_version：改密后作废旧 token 用
  if (!(await columnExists(conn, 'users', 'token_version'))) {
    await conn.query('ALTER TABLE users ADD COLUMN token_version INT UNSIGNED NOT NULL DEFAULT 0 AFTER password_hash');
    console.log('[init-db] 已为旧库添加 users.token_version 列');
  }
  // 2) game_records.result 枚举扩展 'score'
  await conn.query("ALTER TABLE game_records MODIFY result ENUM('win','lose','draw','score') NOT NULL");

  await conn.end();
  console.log('[init-db] 数据库和表初始化完成：' + DB_NAME);
}

main().catch(err => {
  console.error('[init-db] 失败：', err.message);
  process.exit(1);
});
