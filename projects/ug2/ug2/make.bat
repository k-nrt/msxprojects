setlocal enabledelayedexpansion

set OutputName=ug2
set OutDir=.\Release
set AsSrc=vdp_command pers slot math sincos
set CcSrc=main vdp_command slot pers math sincos key_matrix vec_math model_data view player player_beam input explosion bg enemy

set SdccPath=C:\Program Files\SDCC\bin
set SdccIncPath=C:\Program Files\SDCC\include
set IncPath=..\..\..\include
set LibPath=..\..\..\lib
set BinPath=..\..\..\bin
set CCOptions= -c -mz80 --std-sdcc99 --vc --no-std-crt0 --nostdinc --nostdlib
set LinkerOptions=-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x4030 --data-loc 0xe000 -Wl -b_CODE2=0x8000
rem set LinkerOptions=-i -m -w -b _CODE=0x4030 -b _DATA=0xc000
set AS=%SdccPath%\sdasz80
set CC=%SdccPath%\sdcc
set LD=%SdccPath%\sdldz80

set SdkLibs=msx-bios-wrapper msx-timer msx-sprite msx-rand div mul
set SdkLibPath=..\..\..\lib
for %%n in (%SdkLibs%) do set Libs=!Libs! "%SdkLibPath%\%%n.rel"

"%AS%" -g -l -o "%OutDir%\crt0.s.rel" crt0.s
for %%i in (%AsSrc%) do "%AS%" -l -o "%OutDir%\%%i.s.rel" %%i.s
rem for %%i in (%CcSrc%) do "%CC%" %%i.c %CCOptions% -I"%IncPath%" -I"%SdccIncPath%" -o "%OutDir%\%%i.rel"  --opt-code-speed --max-allocs-per-node 8
for %%i in (%CcSrc%) do "%CC%" %%i.c %CCOptions% -I"%IncPath%" -I"%SdccIncPath%" -o "%OutDir%\%%i.rel"  --opt-code-speed

set Objects="%OutDir%\crt0.s.rel"
for %%i in (%AsSrc%) do set Objects=!Objects! "%OutDir%\%%i.s.rel"
for %%i in (%CcSrc%) do set Objects=!Objects! "%OutDir%\%%i.rel"

"%CC%" -o "%OutDir%\%OutputName%.ihx" %Objects% %Libs% %LinkerOptions%
%BinPath%\ihx2bin "%OutDir%\%OutputName%.ihx" "%OutDir%\%OutputName%.rom" -offset 16384 -size 32768
