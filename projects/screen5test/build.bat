@echo off
setlocal enabledelayedexpansion
set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=screen5test
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\bin

set SdccAsmSrc=mul.s divunsigned.s divsigned.s
set MsxAsmSrc=msx-bios-wrapper.s
set MsxCcSrc=msx-timer.c msx-sprite.c msx-rand.c
set ProjectAsmSrc=
set ProjectCcSrc=main.c nrt_sprite.c hmmm.c

call %ProjectDir%\..\build\build-32k-rom.bat

:end_of_bat
endlocal
exit /b
