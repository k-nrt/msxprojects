@echo off
setlocal enabledelayedexpansion

set CURRENT_DIR=%cd%
set BAT_DIR=%~dp0

cd /D %BAT_DIR%
call build-dsk-ipl.bat
if %ERRORLEVEL% neq 0 (
    goto :error_end_of_bat
)

set ProjectDir=%~fp0
set ProjectDir=%ProjectDir:\build.bat=%

set OutName=scroll
set OutDir=%ProjectDir%\Release
set BinPath=%ProjectDir%\..\..\bin

set SdccAsmSrc=divunsigned.s mul.s __sdcc_call_hl.s divsigned.s modunsigned.s
set MsxAsmSrc=msx-bios-wrapper.s
set MsxCcSrc=msx-timer.c msx-sprite.c msx-rand.c
set ProjectAsmSrc=halt.s
set ProjectCcSrc=main.c

call "%ProjectDir%\..\build\build-dsk-main.bat"
if %ERRORLEVEL% neq 0 (
    goto :error_end_of_bat
)

"%BinPath%\makedsk.exe" --output "%OutDir%\%OutName%.dsk" --size 720
if %ERRORLEVEL% neq 0 (
    echo error makedsk
    goto :error_end_of_bat
)

set FILE_AND_SECTORS=
set FILE_AND_SECTORS=%FILE_AND_SECTORS% %OutDir%\dsk-ipl.bin 0
set FILE_AND_SECTORS=%FILE_AND_SECTORS% %OutDir%\%OutName%.bin 2
set FILE_AND_SECTORS=%FILE_AND_SECTORS% makemap\stage1.rbg 60
set FILE_AND_SECTORS=%FILE_AND_SECTORS% makemap\stage1.pnt 62
set FILE_AND_SECTORS=%FILE_AND_SECTORS% makemap\stage1.pgt 64

set TYPE=filename
for %%i in (%FILE_AND_SECTORS%) do (
    if "!TYPE!" equ "filename" (
        set FILENAME=%%i
        set TYPE=sector
    ) else if "!TYPE!" equ "sector"  (
        set SECTOR=%%i
        "%BinPath%\writedsk.exe" "%OutDir%\%OutName%.dsk" "!FILENAME!" --sector !SECTOR!
        if %ERRORLEVEL% neq 0 (
            goto :error_end_of_bat
        )
        set TYPE=filename
    )
)

:end_of_bat
cd /D "%CURRENT_DIR%"
endlocal
exit /B 0

:error_end_of_bat
cd /D "%CURRENT_DIR%"
endlocal
exit /B 1
