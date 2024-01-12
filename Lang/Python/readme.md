

MSYS 使用自己安装的 Python 版本:

需要再MSYS 环境中导出 Python home:

例如 添加到 .bashrc 中

```
export PYTHONPATH=/c/ProgramFiles/Python/Python38
export PYTHONHOME=/c/ProgramFiles/Python/Python38
export PATH=$PATH:$PYTHONHOME/Scripts

```

更新 pip 
```
python -m pip install --upgrade pip
```

# 更换镜像源

https://mirrors.tuna.tsinghua.edu.cn/help/pypi/


## MSYS2 中 pip 安装失败

出现 error: --plat-name must be one of ('win32', 'win-amd64', 'win-arm32', 'win-arm64') 
[参考](https://github.com/pyproj4/pyproj/issues/1009)

```sh
SETUPTOOLS_USE_DISTUTILS=stdlib pip install cocotb
```

但是发现安装好后还是会报 ctype 的错误，虽然已经装有 libffi

## MSYS 上使用 Python 是真麻烦

索性直接将默认的 python 和 pip 更名 python_msys 和 pip_msys 直接给屏蔽掉, 然后使用自己安装的官方的 python

cocotb 库在 MSYS 上 python 路径导出有问题，通过进入内部调试发现问题出在

```sh
# 根据这个获取的 python 路径在执行时会导致路径分隔符 \ 缺失
$(shell cocotb-config --python-bin)

# 使用 cygpath 将 \ 转为 /
$(shell cygpath -m `cocotb-config --python-bin`)
```
