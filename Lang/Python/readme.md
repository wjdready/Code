

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