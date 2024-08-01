@echo off
setlocal enabledelayedexpansion

set BuildRoot=%~fp0
set BuildRoot=%BuildRoot:\build-com.bat=%
set BinPath=%BuildRoot%\..\..\bin

if "%CodeLoc%" equ "" (
	set CodeLoc=0x0130
)
if "%DataLoc%" equ "" (
	set DataLoc=0xc000
)

set Crt0Input=%BuildRoot%\crt0-com.s
set Crt0Output=%OutDir%\crt0-com.s.rel

call "%BuildRoot%\build.bat"
if %errorlevel% neq 0 (
	goto error_end_of_bat
)

"%BinPath%\ihx2bin.exe" -input "%OutDir%\%OutName%.ihx" -output "%OutDir%\%OutName%.com"

if %errorlevel% neq 0 (
	echo error in %OutDir%\%OutName%.ihx
	goto error_end_of_bat
)

:end_of_bat
endlocal
exit /B 0

:error_end_of_bat
endlocal
exit /B 1
