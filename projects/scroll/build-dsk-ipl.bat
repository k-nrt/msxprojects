@echo off
setlocal enabledelayedexpansion
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

call %ProjectDir%\..\build\build-dsk-ipl.bat

:end_of_bat
endlocal
exit /b
