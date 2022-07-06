@echo off
setlocal enabledelayedexpansion
set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=screen2test
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\bin

set SdccAsmSrc=mul.s divunsigned.s divsigned.s
set MsxAsmSrc=msx-bios-wrapper.s
set MsxCcSrc=msx-timer.c msx-sprite.c msx-rand.c
set ProjectAsmSrc=
set ProjectCcSrc=main.c pat.c obj.c

"%BinPath%\bmp2cell.exe" pattern16x16.png -pattern 0 0 128 128 -patternout "pattern16x16.inc" -bitmapout "pattern16x162.png" -dither 2x2 -color YCbCr

call %ProjectDir%\..\build\build-32k-rom.bat

:end_of_bat
endlocal
exit /b
