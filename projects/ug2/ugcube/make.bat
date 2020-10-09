@echo off
setlocal enabledelayedexpansion

if "%SDCC_ROOT_DIR%" neq ""  goto check_dir
echo set SDCC_ROOT_DIR
exit /b

:check_dir
if exist .\Release goto initialize_variables
mkdir .\Release

:initialize_variables
set OutputName=ugcube
set OutDir=.\Release
set AsSrc=vdp_command vdp_palette vdp_write vdp_read vdp_pset
set AsSrc=%AsSrc% sincos mul_core scaler_math timer halt
set AsSrc=%AsSrc% mesh_zx_plane
set AsSrc=%AsSrc% clip clip_line_xyz_s8
set AsSrc=%AsSrc% pers pers_draw_lines pers_create_bbox
set AsSrc=%AsSrc% pers_transform_clip_test pers_transform_no_clip pers_transform_clip_xy pers_transform_clip_xyz pers_transform_position
set AsSrc=%AsSrc% pers_transform_unsigned pers_transform_unsigned_fast
set AsSrc=%AsSrc% mtk_star
set CcSrc=main vdp_command sincos mesh_cube pers bbox flipper
set CcSrc=%CcSrc% test test_sincos test_line test_pers
set CcSrc=%CcSrc% mtk_main mtk_mesh mtk_model mtk_input mtk_effect mtk_player mtk_enemy mtk_shot
set SdccAsSrc=divunsigned mul __sdcc_call_hl divsigned

set SdccPath=%SDCC_ROOT_DIR%
set SdccBinPath=%SdccPath%\bin
set SdccIncPath=%SdccPath%\include
set SdccSrcPath=%SdccPath%\lib\src\z80
set IncPath=..\..\..\include
set LibPath=..\..\..\lib
set BinPath=..\..\..\bin
set CCOptions= -c -mz80 --std-sdcc99 --vc --no-std-crt0 --nostdinc --nostdlib
set LinkerOptions=-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x4030 --data-loc 0xe000 -Wl -b_CODE2=0x8000
rem set LinkerOptions=-i -m -w -b _CODE=0x4030 -b _DATA=0xc000
set AS=%SdccBinPath%\sdasz80
set CC=%SdccBinPath%\sdcc
set LD=%SdccBinPath%\sdldz80

set SdkLibs=msx-bios-wrapper msx-timer msx-sprite msx-rand
set SdkLibPath=..\..\..\lib
for %%n in (%SdkLibs%) do set Libs=!Libs! "%SdkLibPath%\%%n.rel"

"%AS%" -g -l -o "%OutDir%\crt0.s.rel" crt0.s
if %errorlevel% neq 0 exit /b

for %%i in (%AsSrc%) do (
    echo as %%i
    "%AS%" -l -o "%OutDir%\%%i.s.rel" %%i.s
    if %errorlevel% neq 0 exit /b
)

set SdccAsInputs=
set IsEnableSdccAsInputs=
for %%i in (%SdccAsSrc%) do (
    echo as %%i
    set SdccAsInputs=!SdccAsInputs! "%SdccSrcPath%\%%i.s"
    set IsEnableSdccAsInputs=True
)

set SdccAsOutput=
if "%IsEnableSdccAsInputs%" neq "" (
    set SdccAsOutput=sdcc_as_output
    "%AS%" -l -o "%OutDir%\!SdccAsOutput!.s.rel" %SdccAsInputs%
    if %errorlevel% neq 0 exit /b
)

for %%i in (%CcSrc%) do (
    echo cc %%i
    "%CC%" %%i.c %CCOptions% -I"%IncPath%" -I"%SdccIncPath%" -o "%OutDir%\%%i.rel" --opt-code-speed
    if %errorlevel% neq 0 exit /b
)

set Objects="%OutDir%\crt0.s.rel"
for %%i in (%AsSrc%) do set Objects=!Objects! "%OutDir%\%%i.s.rel"
if "%SdccAsOutput%" neq "" set Objects=!Objects! "%OutDir%\%SdccAsOutput%.s.rel"
for %%i in (%CcSrc%) do set Objects=!Objects! "%OutDir%\%%i.rel"

echo link %OutDir%\%OutputName%.ihx
"%CC%" -o "%OutDir%\%OutputName%.ihx" %Objects% %Libs% %LinkerOptions%
if %errorlevel% neq 0 exit /b

echo bin %OutDir%\%OutputName%.rom
%BinPath%\ihx2bin "%OutDir%\%OutputName%.ihx" "%OutDir%\%OutputName%.rom" -offset 16384 -size 32768
