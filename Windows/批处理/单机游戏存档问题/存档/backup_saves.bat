@set GAME_SAVE_DIR="%HOMEDRIVE%%HOMEPATH%\AppData\LocalLow\Ludeon Studios\RimWorld by Ludeon Studios"
@set BACKUP_SAVE_DIR=".\save"

@REM @type save_local.txt

@set /p user_path=<save_local.txt
@echo %user_path%


@for /f "tokens=1,2 delims=\" %%a in ("%user_path%") do @( 
    @set PATHDIR1=%%a 
    @set PATHDIR2=%%b

)

@echo %PATHDIR1%
@echo %PATHDIR2%

@if "%PATHDIR2%" == "Users" (
    @echo "GOood"
)

set perfix="C:\%PATHDIR2%"
set sub_str=%user_path:\%perfix\%=%
echo %perfix%

@REM SET _substring=!_donor:~%_startchar%,%_length%!

@REM set var1=Abc_123
@REM set var2=%var1:*_=%
@REM echo %var2%

@REM echo %GAME_SAVE_DIR%

@REM @if exist %GAME_SAVE_DIR% (
@REM     @xcopy %GAME_SAVE_DIR% %BACKUP_SAVE_DIR% /s/e/y/i/q
@REM     @echo Game save backup OK
@REM ) else (
@REM     @echo Not find game save
@REM )

@REM @pause

