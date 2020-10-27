@echo off
setlocal enabledelayedexpansion

if "%SDCC_ROOT_DIR%" neq ""  goto check_dir
echo set SDCC_ROOT_DIR
exit /b

:check_dir
if exist .\Release goto initialize_variables
mkdir .\Release

:initialize_variables
set OutputName=disk_ipl
set OutDir=.\Release
set AsSrc=disk_ipl
set CcSrc=
set SdccAsSrc=

set SdccPath=%SDCC_ROOT_DIR%
set SdccBinPath=%SdccPath%\bin
set SdccIncPath=%SdccPath%\include
set SdccSrcPath=%SdccPath%\lib\src\z80
set IncPath=..\..\..\include
set LibPath=..\..\..\lib
set BinPath=..\..\bin
set CCOptions= -c -mz80 --std-sdcc99 --vc --no-std-crt0 --nostdinc --nostdlib
set LinkerOptions=-mz80 --no-std-crt0 --nostdinc --nostdlib --data-loc 0xe000
rem set LinkerOptions=-i -m -w -b _CODE=0x4030 -b _DATA=0xc000
set AS=%SdccBinPath%\sdasz80
set CC=%SdccBinPath%\sdcc
set LD=%SdccBinPath%\sdldz80

set SdkLibs=msx-bios-wrapper msx-timer msx-sprite msx-rand
set SdkLibPath=..\..\..\lib
rem for %%n in (%SdkLibs%) do set Libs=!Libs! "%SdkLibPath%\%%n.rel"

rem "%AS%" -g -l -o "%OutDir%\crt0.s.rel" crt0.s
rem if %errorlevel% neq 0 exit /b

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

set Objects=
for %%i in (%AsSrc%) do set Objects=!Objects! "%OutDir%\%%i.s.rel"
if "%SdccAsOutput%" neq "" set Objects=!Objects! "%OutDir%\%SdccAsOutput%.s.rel"
for %%i in (%CcSrc%) do set Objects=!Objects! "%OutDir%\%%i.rel"

echo link %OutDir%\%OutputName%.ihx
"%CC%" -o "%OutDir%\%OutputName%.ihx" %Objects% %Libs% %LinkerOptions%
if %errorlevel% neq 0 exit /b

echo bin %OutDir%\%OutputName%.bin
%BinPath%\ihx2bin "%OutDir%\%OutputName%.ihx" "%OutDir%\%OutputName%.bin" -offset 49152 -size 512