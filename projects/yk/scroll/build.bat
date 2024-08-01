@echo off
setlocal enabledelayedexpansion
set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=scroll
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\..\bin

set SdccAsmSrc=mul.s divunsigned.s divsigned.s modunsigned.s modsigned.s __sdcc_call_hl.s
set MsxAsmSrc=msx-dos.s msx-dos-vdp.s msx-dos-input.s
set MsxCcSrc=
set ProjectAsmSrc=yk-bg.s
set ProjectCcSrc=main.c

call %ProjectDir%\..\..\build\build-com.bat
if %ERRORLEVEL% neq 0 (exit /b 1)

set DskDir=%ProjectDir%\..\dsk\
if not exist "%DskDir%" (
	mkdir "%DskDir%"
)

copy "%OutDir%\%OutName%.com" "%DskDir%"

:end_of_bat
endlocal
exit /b 0
