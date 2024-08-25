@echo off
setlocal enabledelayedexpansion

set CURRENT_DIR=%cd%
set BAT_DIR=%~dp0

set CSPROJECTS=
set CSPROJECTS=%CSPROJECTS% "%BAT_DIR%makemodel\makemodel.csproj"
set CSPROJECTS=%CSPROJECTS% "%BAT_DIR%makesincos\makesincos.csproj"

set TOOLS_DIR=%BAT_DIR%tools\
if not exist "%TOOLS_DIR%" (
	mkdir "%TOOLS_DIR%"
	if !ERRORLEVEL! neq 0 (
		goto :error_end_of_bat
	)
)

for %%I in (%CSPROJECTS%) do (
	set CSPROJ_PATH=%%~I
	dotnet build "!CSPROJ_PATH!" --configuration Release --property:Platform=AnyCPU
	if !ERRORLEVEL! neq 0 (
		echo error in !CSPROJ_PATH!
		goto :error_end_of_bat
	)

	set CSPROJ_DIR=%%~dpI
	set CSPROJ_NAME=%%~nI
	copy /Y "!CSPROJ_DIR!bin\Release\!CSPROJ_NAME!.exe" "%TOOLS_DIR%" >nul
	if !ERRORLEVEL! neq 0 (
		echo can not copy !CSPROJ_NAME!.exe
		goto :error_end_of_bat
	)
)

:end_of_bat
cd /d %CURRENT_DIR%
exit /b 0

:error_end_of_bat
cd /d %CURRENT_DIR%
exit /b 1
