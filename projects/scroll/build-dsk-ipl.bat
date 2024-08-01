@echo off
setlocal enabledelayedexpansion

set CURRENT_DIR=%cd%
set BAT_DIR=%~dp0

cd /D "%BAT_DIR%"

set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build-dsk-ipl.bat=%

set OutName=dsk-ipl
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\bin

set SdccAsmSrc=
set MsxAsmSrc=
set MsxCcSrc=
set ProjectAsmSrc=
set ProjectCcSrc=

call "%ProjectDir%\..\build\build-dsk-ipl.bat"
if %ERRORLEVEL% neq 0 (
    goto :error_end_of_bat
)

:end_of_bat
cd /D "%CURRENT_DIR%"
endlocal
exit /b 0

:error_end_of_bat
cd /D "%CURRENT_DIR%"
endlocal
exit /b 1
