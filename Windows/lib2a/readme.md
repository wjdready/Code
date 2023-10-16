

```sh
# msvc .lib 转 mingw .a
# lib.exe 需安装 VS, gendef 通过 pacman -Fx gendef.exe 查找然后安装对应包

lib.exe /def:your_library.lib /OUT:your_library.obj
gendef.exe your_library.obj
dlltool.exe -dllname your_library.dll -def your_library.def -output-lib libyour_library.a
```
