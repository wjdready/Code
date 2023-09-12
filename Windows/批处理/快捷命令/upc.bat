@REM 该命令可以在虚拟机中快速进入 Windows 某个文件夹
@REM 首先共享整个 C 盘, 然后在 ssh 时传入路径参数, 让虚拟机进入 Windows 文件夹中

@setlocal enabledelayedexpansion

@set "thispath=%cd%"
@set "newPath=!thispath:\=/!"
@set "newPath=!newPath:C:=/share/C!"

@REM @echo 替换前的路径：%thispath%
@REM @echo 替换后的路径：%newPath%

@if "%1" == "." (
    ssh shino@172.16.91.136 -t "cd %newPath%; export DISPLAY=:0; exec bash"
) else (
    ssh shino@172.16.91.136
)
