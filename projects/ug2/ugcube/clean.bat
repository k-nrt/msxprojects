@echo off
setlocal enabledelayedexpansion

set CurrentDir=%cd%
set ProjectRoot=%~fp0
set ProjectRoot=%ProjectRoot:\clean.bat=%

cd /D "%ProjectRoot%"
if exist ReleaseRom (
	rmdir /S /Q ReleaseRom
)

if exist ReleaseCom (
	rmdir /S /Q ReleaseCom
)

cd /D "%CurrentDir%"
endlocal
exit /b
