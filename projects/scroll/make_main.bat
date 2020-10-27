@echo off
setlocal enabledelayedexpansion

if "%SDCC_ROOT_DIR%" neq ""  goto check_dir
echo set SDCC_ROOT_DIR
exit /b

:check_dir
if exist .\Release goto initialize_variables
mkdir .\Release

:initialize_variables
set OutputName=scroll
set OutDir=.\Release
set AsSrc=halt
set CcSrc=main
set SdccAsSrc=divunsigned mul __sdcc_call_hl divsigned modunsigned

set SdccPath=%SDCC_ROOT_DIR%
set SdccBinPath=%SdccPath%\bin
set SdccIncPath=%SdccPath%\include
set SdccSrcPath=%SdccPath%\lib\src\z80
set IncPath=..\..\include
set LibPath=..\..\lib
set BinPath=..\..\bin
set CCOptions= -c -mz80 --std-sdcc99 --vc --no-std-crt0 --nostdinc --nostdlib
set LinkerOptions=-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x8030 --data-loc 0xc000
rem set LinkerOptions=-i -m -w -b _CODE=0x4030 -b _DATA=0xc000
set AS=%SdccBinPath%\sdasz80
set CC=%SdccBinPath%\sdcc
set LD=%SdccBinPath%\sdldz80

set SdkLibs=msx-bios-wrapper msx-timer msx-sprite msx-rand
set SdkLibPath=..\..\lib
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

echo bin %OutDir%\%OutputName%.bin
%BinPath%\ihx2bin "%OutDir%\%OutputName%.ihx" "%OutDir%\%OutputName%.bin" -offset 32768 -size 16384

%BinPath%\makedsk  %OutDir%\%OutputName%.dsk
%BinPath%\writedsk %OutDir%\%OutputName%.dsk %OutDir%\disk_ipl.bin -sector 0
%BinPath%\writedsk %OutDir%\%OutputName%.dsk %OutDir%\%OutputName%.bin -sector 2

%BinPath%\writedsk %OutDir%\%OutputName%.dsk makemap\makemap\stage1.rbg -sector 60
%BinPath%\writedsk %OutDir%\%OutputName%.dsk makemap\makemap\stage1.pnt -sector 62
%BinPath%\writedsk %OutDir%\%OutputName%.dsk makemap\makemap\stage1.pgt -sector 64
