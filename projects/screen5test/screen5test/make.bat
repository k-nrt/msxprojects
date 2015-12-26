setlocal enabledelayedexpansion

set OutputName=screen5test
set OutDir=.\Release
set AsSrc=crt0
set CcSrc=main nrt_sprite hmmm

set SdccPath=C:\Program Files\SDCC\bin
set IncPath=..\..\..\include
set LibPath=..\..\..\lib
set BinPath=..\..\..\bin
set Options=-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x4030 --data-loc 0xc000
set AS=%SdccPath%\sdasz80
set CC=%SdccPath%\sdcc

set SdkLibs=msx-bios-wrapper msx-timer msx-sprite msx-rand div mul
set SdkLibPath=..\..\..\lib
for %%n in (%SdkLibs%) do set Libs=!Libs! "%SdkLibPath%\%%n.rel"

for %%i in (%AsSrc%) do "%AS%" -o "%OutDir%\%%i.rel" %%i.s
for %%i in (%CcSrc%) do "%CC%" %%i.c -c -mz80 -I"%IncPath%" -o "%OutDir%\%%i.rel" --opt-code-speed

set Objects=
for %%i in (%AsSrc%) do set Objects=!Objects! "%OutDir%\%%i.rel"
for %%i in (%CcSrc%) do set Objects=!Objects! "%OutDir%\%%i.rel"

"%CC%" -o "%OutDir%\%OutputName%.ihx" %Objects% %Libs% %Options% --opt-code-speed
%BinPath%\ihx2bin "%OutDir%\%OutputName%.ihx" "%OutDir%\%OutputName%.rom" -offset 16384 -size 32768
