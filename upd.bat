@echo off
cd /d %~dp0 2>nul
set link=https://hk.gh-proxy.org/https://github.com/TT-chen-cqz/leaves-clock/releases/download/leaves-clock
mkdir %temp%\upd 1>nul 2>nul
goto %1 2>nul
:check
del /q /f %temp%\upd\now_version.txt 1>nul 2>nul
curl -s -k -L %link%/version.txt -o %temp%\upd\now_version.txt 2>nul
if %errorlevel% neq 0 (
    exit /b 1
)
set /p ver=<%temp%\upd\now_version.txt
if %ver% GTR %2 (
    exit /b 0
)
exit /b 1
:log
del %temp%\upd\upd_log.txt 1>nul 2>nul
curl -s -k -L %link%/upd_log.txt -o %temp%\upd\upd_log.txt 2>nul
if %errorlevel% neq 0 (
    exit /b 1
)
exit /b 0
:run
del %temp%\upd\run.bat 1>nul 2>nul
curl -s -k -L %link%/upd.7z -o ..\upd.7z 2>nul
if %errorlevel% neq 0 (
    exit /b 1
)
copy run.bat %temp%\upd\run.bat 1>nul 2>nul
copy 7za.exe ..\7za.exe 1>nul 2>nul
copy 7za.dll ..\7za.dll 1>nul 2>nul
start %temp%\upd\run %~dp0
exit /b 0