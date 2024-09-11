@echo off
setlocal enabledelayedexpansion
set CURRENT_DIR=%cd%
set BAT_DIR=%~dp0

set MAKE_MODEL=%BAT_DIR%..\tools\makemodel.exe
set ASSET_DIR=%BAT_DIR%assets

if not exist "%MAKE_MODEL%" (
	call "%BAT_DIR%..\build-tools.bat"
	if !ERRORLEVEL! neq 0 (
		goto :error_end_of_bat
	)
)

"%MAKE_MODEL%" -i %ASSET_DIR%\e1.mqo -obj enemy1     -o mtk_mesh_enemy1.inc  -name Enemy1    -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)
"%MAKE_MODEL%" -i %ASSET_DIR%\e1.mqo -obj beam       -o mtk_mesh_beam.inc    -name Beam      -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)
"%MAKE_MODEL%" -i %ASSET_DIR%\e1.mqo -obj enemy_shot -o mtk_enemy_shot.inc   -name EnemyShot -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)

set OBJ=-obj e2-body1 -obj e2-leg-l1 -obj e2-leg-r1 -obj e2-head1 -obj e2-shield1 -obj e2-cannon1
"%MAKE_MODEL%" -i %ASSET_DIR%\e2.mqo %OBJ% -o mtk_mesh_enemy2_move.inc -name Enemy2Move -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)

set OBJ=-obj e2-body2 -obj e2-leg-l2 -obj e2-leg-r2 -obj e2-head2 -obj e2-shield2 -obj e2-cannon2
"%MAKE_MODEL%" -i %ASSET_DIR%\e2.mqo %OBJ% -o mtk_mesh_enemy2_defence.inc -name Enemy2Defence -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)

set OBJ=-obj e2-body3 -obj e2-leg-l3 -obj e2-leg-r3 -obj e2-head3 -obj e2-shield3 -obj e2-cannon3
"%MAKE_MODEL%" -i %ASSET_DIR%\e2.mqo %OBJ% -o mtk_mesh_enemy2_attack.inc -name Enemy2Attack -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)

"%MAKE_MODEL%" -i %ASSET_DIR%\explosion.mqo -prefix exp0 -o mtk_mesh_exp0.inc -name Exp0 -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)
"%MAKE_MODEL%" -i %ASSET_DIR%\explosion.mqo -prefix exp1 -o mtk_mesh_exp1.inc -name Exp1 -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)
"%MAKE_MODEL%" -i %ASSET_DIR%\explosion.mqo -prefix exp2 -o mtk_mesh_exp2.inc -name Exp2 -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)
"%MAKE_MODEL%" -i %ASSET_DIR%\explosion.mqo -prefix exp3 -o mtk_mesh_exp3.inc -name Exp3 -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)

"%MAKE_MODEL%" -i %ASSET_DIR%\planet.mqo -prefix planet -o mtk_mesh_planet.inc -name Planet -type ugxmesh
if %ERRORLEVEL% neq 0 (goto :error_end_of_bat)

:end_of_bat
cd /d %CURRENT_DIR%
exit /b 0

:error_end_of_bat
cd /d %CURRENT_DIR%
exit /b 1
