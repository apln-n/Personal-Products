@echo off
cd /d %~dp0
cd ../
@chcp 65001 > nul

rem 同ディレクトリにある全ての.urlファイルのファイル名を取得する
dir /b *.url > filename_all.txt

rem 同ディレクトリにある全ての.urlファイルの中身をurl_all.txtにコピーする
copy /b *.url url_all.txt > nul

exit /b
