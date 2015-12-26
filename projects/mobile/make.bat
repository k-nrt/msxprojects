set OutDir=.\Release
set IncPath=..\..\include
set LibPath=..\..\lib
set BinPath=..\..\bin
set Libs="%LibPath%\msx-bios-wrapper.rel"
set Libs=%Libs% "%LibPath%\msx-timer.rel"
set Libs=%Libs% "%LibPath%\msx-sprite.rel"
set Libs=%Libs% "%LibPath%\msx-rand.rel"
set Libs=%Libs% "%LibPath%\div.rel"
set Options=-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x4030 --data-loc 0xc000

sdasz80 -o "%OutDir%\crt0.rel" crt0.s
sdcc main.c -c -mz80 -I"%IncPath%" -o "%OutDir%\main.rel" --opt-code-speed
sdcc xsp.c  -c -mz80 -I"%IncPath%" -o "%OutDir%\xsp.rel" --opt-code-speed

set Objects="%OutDir%\crt0.rel" "%OutDir%\main.rel" "%OutDir%\xsp.rel"
sdcc -o "%OutDir%\mobile.ihx" %Objects% %Libs% %Options%
%BinPath%\ihx2bin "%OutDir%\mobile.ihx" "%OutDir%\mobile.rom" -offset 16384 -size 32768
