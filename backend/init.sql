# ============================================================
#  游戏大厅数据库初始化脚本
#  用法（MySQL 已启动时）：
#    mysql -uroot -proot < init.sql
#  或在 backend 目录执行：
#    npm run init-db
# ============================================================

CREATE DATABASE IF NOT EXISTS game_hall
  DEFAULT CHARACTER SET utf8mb4
  DEFAULT COLLATE utf8mb4_unicode_ci;

USE game_hall;

-- 用户表
CREATE TABLE IF NOT EXISTS users (
  id            INT UNSIGNED NOT NULL AUTO_INCREMENT,
  username      VARCHAR(32)  NOT NULL,
  password_hash VARCHAR(100) NOT NULL,
  token_version INT UNSIGNED NOT NULL DEFAULT 0,
  created_at    DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id),
  UNIQUE KEY uk_username (username)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- 游戏记录表（按 user_id 隔离每个账号的战绩）
CREATE TABLE IF NOT EXISTS game_records (
  id        INT UNSIGNED NOT NULL AUTO_INCREMENT,
  user_id   INT UNSIGNED NOT NULL,
  game_name VARCHAR(32)  NOT NULL,           -- 游戏名：猜数字 / 井字棋 ...
  result    ENUM('win','lose','draw','score') NOT NULL,
  detail    VARCHAR(200) NOT NULL DEFAULT '',-- 详情：如"共猜了 7 次"
  duration  INT UNSIGNED NOT NULL DEFAULT 0, -- 用时（秒）
  created_at DATETIME    NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id),
  KEY idx_user_time (user_id, created_at),
  CONSTRAINT fk_records_user FOREIGN KEY (user_id)
    REFERENCES users (id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
