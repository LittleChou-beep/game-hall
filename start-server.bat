@echo off
REM ============================================================
REM  Start MySQL (if needed) + Node.js backend for the game hall.
REM  Frontend: just open index.html in a browser (or keep using
REM  the python server at http://localhost:8000).
REM ============================================================

setlocal

set "MYSQLD=D:\phpstudy_pro\Extensions\MySQL8.0.12\bin\mysqld.exe"
set "MY_INI=D:\phpstudy_pro\Extensions\MySQL8.0.12\my.ini"

echo [1/2] Checking MySQL on port 3306 ...
netstat -ano | findstr ":3306" | findstr "LISTENING" >nul
if errorlevel 1 (
  echo       MySQL not running, starting it ...
  start "" /B "%MYSQLD%" --defaults-file="%MY_INI%"
  timeout /t 6 /nobreak >nul
) else (
  echo       MySQL is already running.
)

echo [2/2] Starting backend (http://localhost:3000) ...
echo       Press Ctrl+C in this window to stop the server.
cd /d "%~dp0backend"
call npm.cmd start

endlocal
