set OutDir=..\..\lib
sdasz80 -o "%OutDir%\div.rel" "divmixed.s" "divsigned.s" "divunsigned.s" "modunsigned.s"
sdasz80 -o "%OutDir%\mul.rel" "mul.s"
sdasz80 -o "%OutDir%\msx-bios-wrapper.rel" "msx-bios-wrapper.s"
sdcc "msx-rand.c" -c -mz80 -o "%OutDir%\msx-rand.rel" -I"..\..\include" --opt-code-speed
sdcc "msx-sound.c" -c -mz80 -o "%OutDir%\msx-sound.rel" -I"..\..\include" --opt-code-speed
rem sdcc "msx-sound.c" -c -mz80 -o "%OutDir%\msx-sound.rel" -I"..\..\include"
sdcc "msx-sprite.c" -c -mz80 -o "%OutDir%\msx-sprite.rel" -I"..\..\include" --opt-code-speed
sdcc "msx-timer.c" -c -mz80 -o "%OutDir%\msx-timer.rel" -I"..\..\include" --opt-code-speed
