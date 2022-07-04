@echo off
setlocal enabledelayedexpansion

set CurrentDir=%cd%
set ProjectRoot=%~fp0
set ProjectRoot=%ProjectRoot:\clean.bat=%

cd /D %ProjectRoot%
if exist Release (
    rmdir /S /Q Release
)

cd /D %CurrentDir%
endlocal
exit /b
