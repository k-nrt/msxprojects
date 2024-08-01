@echo off
setlocal enabledelayedexpansion

set CURRENT_DIR=%cd%
set BAT_DIR=%~dp0

set CSPROJECTS=
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\bmp2cell\bmp2cell.csproj"
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\bmp2sprite\bmp2sprite.csproj"
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\msxbmp2mc\msxbmp2mc.csproj"
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\ihx2bin\ihx2bin.csproj"
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\makedsk\makedsk.csproj"
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\writedsk\writedsk.csproj"

for %%i in (%CSPROJECTS%) do (
    set CSPROJ_PATH=%BAT_DIR%\%%~i
    dotnet build "!CSPROJ_PATH!" --configuration Release --property:Platform=AnyCPU
    if !ERRORLEVEL! neq 0 (
        echo error in !CSPROJ_PATH!
        goto :error_end_of_bat
    )
)

:end_of_bat
cd /d %CURRENT_DIR%
exit /b 0

:error_end_of_bat
cd /d %CURRENT_DIR%
exit /b 1
