
@REM 实现从 cmd 中直接执行比如 msys -c "make run"

@REM set HOME=C:\Users\%USERNAME%
set HOME=C:\ProgramFiles\msys64\home\shino

@if "%1" == "-c" (
    if not "%~2" == "" (
        @C:\ProgramFiles\msys64\msys2_shell.cmd  -ucrt64  -defterm -no-start -use-full-path -here %1 %2
    ) else (
        echo -c option give, but no arg
    )

@REM 这是调试用的, VSCode 调用 GDB 时会传入 /d 
) else if "%1" == "/d" (

    if not "%~3" == "" (
        @C:\ProgramFiles\msys64\msys2_shell.cmd  -ucrt64  -defterm -no-start -use-full-path -here -c %3
    ) else (
        echo /d option give, but no arg %2
    )

) else (
    @C:\ProgramFiles\msys64\msys2_shell.cmd  -ucrt64  -defterm -no-start -use-full-path -here
)

