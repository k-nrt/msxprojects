@echo off
setlocal enabledelayedexpansion

set CURRENT_DIR=%cd%
set BAT_DIR=%~dp0

rem set MSBUILD_EXE=%windir%\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe
set MSBUILD_EXE=%windir%\Microsoft.NET\Framework64\v3.5\MSBuild.exe

if not exist "%MSBUILD_EXE%" (
    echo not found %MSBUILD_EXE%
    goto error_end_of_bat
)

set CSPROJECTS=
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\bmp2cell\bmp2cell.csproj"
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\bmp2sprite\bmp2sprite.csproj"
set CSPROJECTS=%CSPROJECTS% "msx_tools_cs\msxbmp2mc\msxbmp2mc.csproj"

for %%i in (%CSPROJECTS%) do (
    set CSPROJ_PATH=%BAT_DIR%\%%~i
    "%MSBUILD_EXE%" "!CSPROJ_PATH!" -target:Rebuild -property:Configuration=Release;Platform=AnyCPU
    if !ERRORLEVEL! neq 0 (
        echo error in !CSPROJ_PATH!
        goto error_end_of_bat
    )
)

:end_of_bat
cd /d %CURRENT_DIR%
exit /b 0

:error_end_of_bat
cd /d %CURRENT_DIR%
exit /b 1
