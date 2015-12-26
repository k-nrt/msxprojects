set MM=..\makemodel\bin\Release\makemodel.exe
"%MM%" -i ..\assets\cube.mqo -prefix player -o model_player.h -scale 0.1 -name Player
"%MM%" -i ..\assets\cube.mqo -prefix beam   -o model_beam.h   -scale 0.1 -name Beam
"%MM%" -i ..\assets\cube.mqo -prefix exp0   -o model_exp0.h   -scale 0.1 -name Exp0
"%MM%" -i ..\assets\cube.mqo -prefix exp1   -o model_exp1.h   -scale 0.1 -name Exp1
"%MM%" -i ..\assets\cube.mqo -prefix exp2   -o model_exp2.h   -scale 0.1 -name Exp2
"%MM%" -i ..\assets\cube.mqo -prefix exp3   -o model_exp3.h   -scale 0.1 -name Exp3
"%MM%" -i ..\assets\cube.mqo -prefix enemy0 -o model_enemy0.h -scale 0.1 -name Enemy0
"%MM%" -i ..\assets\cube.mqo -prefix enemy1 -o model_enemy1.h -scale 0.1 -name Enemy1
"%MM%" -i ..\assets\cube.mqo -prefix enemy2 -o model_enemy2.h -scale 0.1 -name Enemy2
