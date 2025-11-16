


### [2023-03-01 20:49:43]

无法更新，错误代码 0x80070003，参考[解决方案](https://zhuanlan.zhihu.com/p/457775349), 采用方法 4 生效;

```shell
Sfc /scannow
# 现在让扫描过程自行完成。然后，需要键入以下命令并按 Enter
DISM /Online /Cleanup-Image /restorehealth
```

## 查看文件被哪个进程占用

Win 搜索 `资源监视器`

