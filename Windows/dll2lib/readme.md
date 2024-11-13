
https://ivanzz1001.github.io/records/post/windows/2018/01/05/win-export

pexports 下载地址 http://sourceforge.net/projects/mingw/files/MinGW/Extension/pexports

pexports -o mylib.dll  > mylib.def
lib /def:mylib.def /machine:x86 /out:mylib.lib
cl demo.c -link mylib.lib
