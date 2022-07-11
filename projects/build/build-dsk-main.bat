@echo off
setlocal enabledelayedexpansion

set BuildRoot=%~fp0
set BuildRoot=%BuildRoot:\build-dsk-main.bat=%
set BinPath=%BuildRoot%\..\..\bin

if "%CodeLoc%" equ "" (
	set CodeLoc=0x8030
)
if "%DataLoc%" equ "" (
	set DataLoc=0xc000
)

set Crt0Input=%BuildRoot%\crt0-dsk-main.s
set Crt0Output=%OutDir%\crt0-dsk-main.s.rel

call "%BuildRoot%\build.bat"
if %errorlevel% neq 0 (
	goto error_end_of_bat
)

"%BinPath%\ihx2bin.exe" "%OutDir%\%OutName%.ihx" "%OutDir%\%OutName%.bin" -offset 32768 -size 16384
if %errorlevel% neq 0 (
	echo error
	goto error_end_of_bat
)

:end_of_bat
endlocal
exit /B 0

:error_end_of_bat
endlocal
exit /B 1
