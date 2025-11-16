
官方最新版地址生成工具
https://visualstudio.microsoft.com/visual-cpp-build-tools/

旧版本生成工具, 2022 为 17, 2019 为 16, 2017 为 15
https://aka.ms/vs/17/release/vs_buildtools.exe

community IDE 版本:
https://search.ddooo.com/search.html?wd=visual%20studio%20community

目前想同时安装多个版本 VS 环境，只能下载每个版本的 installer, 启动后安装其中的某个组件，之后打开 installer 就可以看到各个版本的 VS.

值得注意的是 2022 生成工具安装在 `C:\Program Files (x86)\Microsoft Visual Studio\2022` 
但是如果是 `community IDE` 却安装在 `C:\Program Files\Microsoft Visual Studio\2022\Community` (不再支持32位IDE)
因此安装 `2022 Community` 就不要安装 `生成工具` 了，要不然冲突

