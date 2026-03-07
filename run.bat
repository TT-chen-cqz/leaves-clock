@echo off
cd /d %1 2>nul
cd .. 2>nul
title 更新程序
echo 本程序由tt-tools-upd提供支持
echo 正在更新中……
rd /s /q bin 2>nul
7za x upd.7z -o.\bin 1>nul 2>nul
del 7za.exe 2>nul
del 7za.dll 2>nul
del upd.7z 2>nul
echo 更新完成！
pause
exit