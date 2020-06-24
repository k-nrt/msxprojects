@echo off
setlocal enabledelayedexpansion

set OutputName=ugcube
set OutDir=.\Release
set AsSrc=vdp_command vdp_palette vdp_write vdp_read 
set AsSrc=%AsSrc% sincos mul_core scaler_math 
set AsSrc=%AsSrc% model_zx_plane
set AsSrc=%AsSrc% clip clip_line_xyz_s8
set AsSrc=%AsSrc% pers pers_draw_lines pers_transform_no_clip pers_transform_clip_xy pers_transform_clip_xyz 
set AsSrc=%AsSrc% pers_transform_unsigned pers_transform_unsigned_fast
set CcSrc=main vdp_command sincos model_cube pers test test_sincos test_line test_pers
set SdccAsSrc=divunsigned mul __sdcc_call_hl divsigned

set SdccPath=D:\SDCC
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

for %%i in (%SdccAsSrc%) do (
    echo as %%i
    "%AS%" -l -o "%OutDir%\%%i.s.rel" "%SdccSrcPath%\%%i.s"
    if %errorlevel% neq 0 exit /b
)

rem for %%i in (%CcSrc%) do "%CC%" %%i.c %CCOptions% -I"%IncPath%" -I"%SdccIncPath%" -o "%OutDir%\%%i.rel"  --opt-code-speed --max-allocs-per-node 8
for %%i in (%CcSrc%) do (
    echo cc %%i
    "%CC%" %%i.c %CCOptions% -I"%IncPath%" -I"%SdccIncPath%" -o "%OutDir%\%%i.rel" --opt-code-speed
    if %errorlevel% neq 0 exit /b
)

set Objects="%OutDir%\crt0.s.rel"
for %%i in (%AsSrc%) do set Objects=!Objects! "%OutDir%\%%i.s.rel"
for %%i in (%SdccAsSrc%) do set Objects=!Objects! "%OutDir%\%%i.s.rel"
for %%i in (%CcSrc%) do set Objects=!Objects! "%OutDir%\%%i.rel"

echo link %OutDir%\%OutputName%.ihx
"%CC%" -o "%OutDir%\%OutputName%.ihx" %Objects% %Libs% %LinkerOptions%
if %errorlevel% neq 0 exit /b

echo bin %OutDir%\%OutputName%.rom
%BinPath%\ihx2bin "%OutDir%\%OutputName%.ihx" "%OutDir%\%OutputName%.rom" -offset 16384 -size 32768
