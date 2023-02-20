@set GAME_SAVE_DIR="%HOMEDRIVE%%HOMEPATH%\AppData\LocalLow\Ludeon Studios\RimWorld by Ludeon Studios"
@set BACKUP_SAVE_DIR=".\save"

@if exist %BACKUP_SAVE_DIR% (
    @echo Game save path:
    @echo %GAME_SAVE_DIR%
    @set /p var=Replace save? 
    @xcopy %BACKUP_SAVE_DIR% %GAME_SAVE_DIR% /s/e/y/i/q
) else (
    @echo No backup find
)

@pause
