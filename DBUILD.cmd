@echo off
set debug=1
mode co80,102

goto build

:tryagain
echo.
echo Premere un tasto qualsiasi per rieseguire NMAKE
echo Premere Ctrl-C per terminare.
pause > nul

:build
cls
nmake -nologo
if errorlevel 1 goto tryagain

