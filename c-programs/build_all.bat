@echo off
REM ============================================================
REM  Build all C programs in one click.
REM  Usage: double-click, or run "build_all.bat" in cmd.
REM ============================================================

setlocal

REM 无论从哪个目录调用，都切换到本脚本所在目录，保证源码/exe 同目录
cd /d "%~dp0"

set "CC=gcc"
set "CFLAGS=-Wall -Wextra -std=c11 -finput-charset=UTF-8 -fexec-charset=GBK"
set "SRC=guess_number multiplication_table fibonacci rps calculator converter launcher tic_tac_toe blackjack guess_word"

echo ============================================
echo   Building all C programs...
echo ============================================

set "OK=0"
set "FAIL=0"

for %%S in (%SRC%) do (
    echo [build] %%S.c -^> %%S.exe
    %CC% %CFLAGS% %%S.c -o %%S.exe
    if errorlevel 1 (
        echo   [FAIL] %%S.c
        set /a FAIL=FAIL+1
    ) else (
        echo   [OK]
        set /a OK=OK+1
    )
)

echo.
echo ============================================
echo   Done. OK=%OK%  FAIL=%FAIL%
echo ============================================

if %FAIL%==0 (
    echo All programs built successfully.
    echo Run launcher.exe to open the tool menu.
) else (
    echo %FAIL% programs failed to build.
)

echo.
pause
