@echo off
cd /d %~dp0
cd ../
@chcp 65001 > nul

del url_all.txt, filename_all.txt

exit /b
