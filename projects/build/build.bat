@echo off
setlocal enabledelayedexpansion

if "%SDCC_ROOT_DIR%" equ "" (
	echo set SDCC_ROOT_DIR
	goto :error_end_of_bat
)

if "%ProjectDir%" equ "" (
	echo set ProjectDir
	goto :error_end_of_bat
)

if "%OutDir%" equ "" (
	echo set OutDir
	goto :error_end_of_bat
)

if "%OutName%" equ "" (
	echo set OutName
	goto :error_end_of_bat
)

set BuildRoot=%~fp0
set BuildRoot=%BuildRoot:\build.bat=%
set IncPath=%IncPath% "%BuildRoot%\..\..\include" "%SDCC_ROOT_DIR%\include"

set CcOptions=-mz80 --std-sdcc99 --vc --no-std-crt0 --nostdinc --nostdlib --opt-code-speed
set CcOptions=%CcOptions% %AdditionalCcOptions%

if "%CodeLoc%" equ "" (
	goto :error_end_of_bat
)
if "%DataLoc%" equ "" (
	goto :error_end_of_bat
)

set LinkerOptions=-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc %CodeLoc% --data-loc %DataLoc%
set LinkerOptions=%LinkerOptions% %AdditionalLinkerOptions%

set Sdasz80Exe=%SDCC_ROOT_DIR%\bin\sdasz80.exe
set SdccExe=%SDCC_ROOT_DIR%\bin\sdcc.exe

if not exist "%OutDir%" (
	mkdir "%OutDir%"
)

:assemble_crt0
if "%Crt0Input%" equ "" (
	goto :error_end_of_bat
)

if "%Crt0Output%" equ "" (
	goto :error_end_of_bat
)

"%Sdasz80Exe%" -o "%Crt0Output%" "%Crt0Input%"
if %errorlevel% neq 0 (
	echo error
	goto :error_end_of_bat
)

:assemble_sdcc_asm_libs
set SdccAsmSrcs=
set SdccAsmLibSrcDir=%SDCC_ROOT_DIR%\lib\src\z80
for %%i in (%SdccAsmSrc%) do (
	set InputPath=%SdccAsmLibSrcDir%\%%~i
	set OutputPath=%OutDir%\%%~ni.s.rel
	rem echo !InputPath! !OutputPath!
	echo %%i
	set SdccAsmSrcs=!SdccAsmSrcs! "!InputPath!"
)

if "%SdccAsmSrc%" neq "" (
	set SdccAsmOutputs=%OutDir%\sdcc-asm-libs.s.rel
	"%Sdasz80Exe%" -o "!SdccAsmOutputs!" %SdccAsmSrcs%
	if %errorlevel% neq 0 (
		echo error in %SdccAsmSrcs%
		goto :error_end_of_bat
	)
)

set MsxAsmOutputs=
set MsxAsmLibSrcDir=%BuildRoot%\..\..\src\lib
for %%i in (%MsxAsmSrc%) do (
	set InputPath=%MsxAsmLibSrcDir%\%%i
	set OutputPath=%OutDir%\%%~ni.s.rel
	rem echo !InputPath! !OutputPath!
	echo %%i
	"%Sdasz80Exe%" -o "!OutputPath!" "!InputPath!"
	if %errorlevel% neq 0 (
		echo error in !InputPath!
		goto :error_end_of_bat
	)
	set MsxAsmOutputs=!MsxAsmOutputs! "!OutputPath!"
)

set MsxCcOutputs=
set MsxCcLibSrcDir=%BuildRoot%\..\..\src\lib
for %%i in (%MsxCcSrc%) do (
	set InputPath=%MsxCcLibSrcDir%\%%i
	set OutputPath=%OutDir%\%%~ni.c.rel
	rem echo !InputPath! !OutputPath!
	echo %%i
	"%SdccExe%" "!InputPath!" -c -I"%BuildRoot%\..\..\include" -o "!OutputPath!" %CcOptions%
	if %errorlevel% neq 0 (
		echo error in !InputPath!
		goto :error_end_of_bat
	)
	set MsxCcOutputs=!MsxCcOutputs! "!OutputPath!"
)

set ProjectAsmOutputs=
for %%i in (%ProjectAsmSrc%) do (
	set InputPath=%ProjectDir%\%%~i
	set OutputPath=%OutDir%\%%~ni.s.rel
	rem echo !InputPath! !OutputPath!
	echo %%i
	"%Sdasz80Exe%" -o "!OutputPath!" "!InputPath!"
	if %errorlevel% neq 0 (
		echo error in !InputPath!
		goto :error_end_of_bat
	)
	set ProjectAsmOutputs=!ProjectAsmOutputs! "!OutputPath!"
)

set ProjectIncPaths=
for %%i in (%IncPath%) do (
	set ProjectIncPaths=!ProjectIncPaths! -I"%%~i"
)

set ProjectCcOutputs=
for %%i in (%ProjectCcSrc%) do (
	set InputPath=%ProjectDir%\%%~i
	set OutputPath=%OutDir%\%%~ni.c.rel
	rem echo !InputPath! !OutputPath!
	echo %%i
	"%SdccExe%" "!InputPath!" -c %ProjectIncPaths% -o "!OutputPath!" %CcOptions%
	if %errorlevel% neq 0 (
		echo error in !InputPath!
		goto :error_end_of_bat
	)
	set ProjectCcOutputs=!ProjectCcOutputs! "!OutputPath!"
)

set Outputs=%Crt0Output% %SdccAsmOutputs% %MsxAsmOutputs% %MsxCcOutputs% %ProjectAsmOutputs% %ProjectCcOutputs%
"%SdccExe%" -o "%OutDir%\%OutName%.ihx" %Outputs% %LinkerOptions%
if %errorlevel% neq 0 (
	echo link error
	goto :error_end_of_bat
)

:end_of_bat
endlocal
exit /B 0

:error_end_of_bat
endlocal
exit /B 1
