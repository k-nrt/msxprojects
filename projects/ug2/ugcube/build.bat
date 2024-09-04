@echo off
setlocal enabledelayedexpansion
set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=ugcube
set BinPath=%ProjectDir%\..\..\bin

if "%~1" equ "--system" (
	set System=%~2
)

if "%System%" equ "" (
	set System=msx-dos
	rem set System=rom32k
	set DskDir=%ProjectDir%\dsk
)

set SdccAsmSrc=divunsigned.s mul.s __sdcc_call_hl.s divsigned.s __sdcc_call_iy.s
set MsxAsmSrc=msx-timer.s
set MsxCcSrc=msx-rand.c
set ProjectAsmSrc=vdp_command.s vdp_palette.s vdp_write.s vdp_read.s vdp_pset.s
set ProjectAsmSrc=%ProjectAsmSrc% sincos.s mul_core.s scaler_math.s halt.s
set ProjectAsmSrc=%ProjectAsmSrc% mesh_zx_plane.s
set ProjectAsmSrc=%ProjectAsmSrc% clip.s clip_line_xyz_s8.s
set ProjectAsmSrc=%ProjectAsmSrc% pers.s pers_draw_lines.s pers_draw_lines_clip_xy.s pers_create_bbox.s
set ProjectAsmSrc=%ProjectAsmSrc% pers_transform_clip_test.s pers_transform_no_clip.s pers_transform_clip_xy.s pers_transform_clip_xyz.s pers_transform_position.s
set ProjectAsmSrc=%ProjectAsmSrc% pers_transform_unsigned.s pers_transform_unsigned_fast.s
set ProjectAsmSrc=%ProjectAsmSrc% mtk_star.s
set ProjectCcSrc=main.c vdp_command.c sincos.c mesh_cube.c pers.c bbox.c flipper.c
set ProjectCcSrc=%ProjectCcSrc% test.c test_sincos.c test_line.c test_pers.c
set ProjectCcSrc=%ProjectCcSrc% mtk_main.c mtk_mesh.c mtk_model.c mtk_input.c mtk_effect.c mtk_player.c mtk_enemy.c mtk_shot.c mtk_star.c
set ProjectCcSrc=%ProjectCcSrc% mtk_far_bg.c mtk_world.c mtk_enemy_shot.c

if "%System%" equ "msx-dos" (
	set MsxAsmSrc=%MsxAsmSrc% msx-dos-vdp.s msx-dos-input.s msx-dos.s
	set AdditionalCcOptions=-DSYSTEM_MSXDOS
	set DataLoc=0xa000
	set AdditionalLinkerOptions=-Wl -b_CODE2=0x4000
	set OutDir=%ProjectDir%\ReleaseCom

	call "%ProjectDir%\..\..\build\build-com.bat"
	if !ErrorLevel! neq 0 (goto :error_end_of_bat)

	if not exist "!DskDir!" (
		mkdir "!DskDir!"
		if !ErrorLevel! neq 0 (
			echo can not create !DskDir!
			goto :error_end_of_bat
		)
	)

	copy "!OutDir!\%OutName%.com" "!DskDir!\%OutName%.com" >nul

) else if "%System%" equ "rom32k" (
	set MsxAsmSrc=%MsxAsmSrc% msx-bios-wrapper.s
	set AdditionalCcOptions=-DSYSTEM_ROM32K
	set DataLoc=0xe000
	set AdditionalLinkerOptions=-Wl -b_CODE2=0x8000
	set OutDir=%ProjectDir%\ReleaseRom

	call %ProjectDir%\..\..\build\build-32k-rom.bat
	if !ErrorLevel! neq 0 (goto :error_end_of_bat)

) else (
	echo unknown system
	goto :error_end_of_bat
)

echo System=%System%

:end_of_bat
endlocal
exit /b 0

:error_end_of_bat
endlocal
exit /b 1
