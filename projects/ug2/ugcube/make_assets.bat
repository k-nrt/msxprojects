@echo off
setlocal enabledelayedexpansion

set MakeModel=..\makemodel\bin\Release\makemodel.exe
set AssetRoot=.\assets
set Models=e1

for %%i in (%Models%) do (
    echo model %%i
    "%MakeModel%" -o "%%i.inc" -i %AssetRoot%\%%i.mqo -name %%i -type ugxmesh
    if %errorlevel% neq 0 exit /b
)
