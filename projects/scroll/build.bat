@echo off
setlocal enabledelayedexpansion

call build-dsk-ipl.bat

set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=scroll
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\bin

set SdccAsmSrc=divunsigned.s mul.s __sdcc_call_hl.s divsigned.s modunsigned.s
set MsxAsmSrc=msx-bios-wrapper.s
set MsxCcSrc=msx-timer.c msx-sprite.c msx-rand.c
set ProjectAsmSrc=halt.s
set ProjectCcSrc=main.c

call %ProjectDir%\..\build\build-dsk-main.bat

%BinPath%\makedsk  %OutDir%\%OutName%.dsk
%BinPath%\writedsk %OutDir%\%OutName%.dsk %OutDir%\dsk-ipl.bin -sector 0

%BinPath%\writedsk %OutDir%\%OutName%.dsk %OutDir%\%OutName%.bin -sector 2

%BinPath%\writedsk %OutDir%\%OutName%.dsk makemap\stage1.rbg -sector 60
%BinPath%\writedsk %OutDir%\%OutName%.dsk makemap\stage1.pnt -sector 62
%BinPath%\writedsk %OutDir%\%OutName%.dsk makemap\stage1.pgt -sector 64

:end_of_bat
endlocal
exit /b
