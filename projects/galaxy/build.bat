@echo off
setlocal enabledelayedexpansion
set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=galaxy
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\bin

set SdccAsmSrc=mul.s divunsigned.s divsigned.s
set MsxAsmSrc=msx-bios-wrapper.s
set MsxCcSrc=msx-timer.c msx-sprite.c msx-rand.c
set ProjectAsmSrc=
set ProjectCcSrc=main.c sprite_blinker.c

python bmp2sprite.py galaxy.png --output galaxy_sprite.inl
if %errorlevel% neq 0 (
    goto end_of_bat
)

call %ProjectDir%\..\build\build-32k-rom.bat

:end_of_bat
endlocal
exit /b
