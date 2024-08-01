@echo off
setlocal enabledelayedexpansion

set BuildRoot=%~fp0
set BuildRoot=%BuildRoot:\build-dsk-ipl.bat=%
set BinPath=%BuildRoot%\..\..\bin

if "%CodeLoc%" equ "" (
	set CodeLoc=0xc000
)
if "%DataLoc%" equ "" (
	set DataLoc=0xe000
)

set Crt0Input=%BuildRoot%\dsk-ipl.s
set Crt0Output=%OutDir%\dsk-ipl.s.rel

call "%BuildRoot%\build.bat"
if %errorlevel% neq 0 (
	goto :error_end_of_bat
)

"%BinPath%\ihx2bin.exe" -input "%OutDir%\%OutName%.ihx" -output "%OutDir%\%OutName%.bin" -offset 49152 -size 512
if %errorlevel% neq 0 (
	echo error
	goto :error_end_of_bat
)

:end_of_bat
endlocal
exit /B 0

:error_end_of_bat
endlocal
exit /B 1
