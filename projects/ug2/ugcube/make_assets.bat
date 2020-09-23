@echo off
setlocal enabledelayedexpansion

set MakeModel=..\makemodel\bin\Release\makemodel.exe
set AssetRoot=.\assets

"%MakeModel%" -i %AssetRoot%\e1.mqo -prefix enemy1 -o mtk_mesh_enemy1.inc -name Enemy1  -type ugxmesh
"%MakeModel%" -i %AssetRoot%\e1.mqo -prefix beam   -o mtk_mesh_beam.inc   -name Beam    -type ugxmesh

"%MakeModel%" -i %AssetRoot%\explosion.mqo -prefix exp0 -o mtk_mesh_exp0.inc -name Exp0 -type ugxmesh
"%MakeModel%" -i %AssetRoot%\explosion.mqo -prefix exp1 -o mtk_mesh_exp1.inc -name Exp1 -type ugxmesh
"%MakeModel%" -i %AssetRoot%\explosion.mqo -prefix exp2 -o mtk_mesh_exp2.inc -name Exp2 -type ugxmesh
"%MakeModel%" -i %AssetRoot%\explosion.mqo -prefix exp3 -o mtk_mesh_exp3.inc -name Exp3 -type ugxmesh
