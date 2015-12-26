set OutDir=.\Release
set IncPath=..\..\include
set LibPath=..\..\lib
set BinPath=..\..\bin
set Libs="%LibPath%\msx-bios-wrapper.rel"
set Libs=%Libs% "%LibPath%\msx-timer.rel"
set Libs=%Libs% "%LibPath%\msx-sprite.rel"
set Libs=%Libs% "%LibPath%\msx-rand.rel"
set Libs=%Libs% "%LibPath%\div.rel"
set Libs=%Libs% "%LibPath%\mul.rel"
set Options=-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x4030 --data-loc 0xc000

rem %BinPath%\bmp2cell pattern16x16.png -o galaxy_sprite.inl
%BinPath%\bmp2cell pattern16x16.png -pattern 0 0 128 128 -patternout "pattern16x16.inc" -bitmapout "pattern16x162.png" -dither 2x2 -color YCbCr

sdasz80 -o "%OutDir%\crt0.rel" crt0.s
rem sdcc main.c -c -mz80 -I"%IncPath%" -o "%OutDir%\main.rel" --opt-code-speed
sdcc main.c -c -mz80 -I"%IncPath%" -o "%OutDir%\main.rel"
sdcc pat.c -c -mz80 -I"%IncPath%" -o "%OutDir%\pat.rel" --opt-code-speed
sdcc obj.c -c -mz80 -I"%IncPath%" -o "%OutDir%\obj.rel" --opt-code-speed

set Objects="%OutDir%\crt0.rel" "%OutDir%\main.rel" "%OutDir%\pat.rel" "%OutDir%\obj.rel"
sdcc -o "%OutDir%\screen2test.ihx" %Objects% %Libs% %Options% --opt-code-speed
%BinPath%\ihx2bin "%OutDir%\screen2test.ihx" "%OutDir%\screen2test.rom" -offset 16384 -size 32768
