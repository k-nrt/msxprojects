@echo off
setlocal enabledelayedexpansion
set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=ug2
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\bin

set SdccAsmSrc=mul.s divunsigned.s
set MsxAsmSrc=msx-bios-wrapper.s
set MsxCcSrc=msx-timer.c msx-sprite.c msx-rand.c
set ProjectAsmSrc=vdp_command.s pers.s slot.s math.s sincos.s
set ProjectCcSrc=main.c vdp_command.c slot.c pers.c math.c sincos.c key_matrix.c vec_math.c model_data.c view.c player.c player_beam.c input.c explosion.c bg.c enemy.c
set DataLoc=0xe000
set AdditionalLinkerOptions=-Wl -b_CODE2=0x8000

call %ProjectDir%\..\..\build\build-32k-rom.bat

:end_of_bat
endlocal
exit /b
