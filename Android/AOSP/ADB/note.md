

## 获取 App 应用

```shell
# 监视当前活动的应用，打开该应用
adb shell am monitor

# 然后查询该应用完整路径
adb shell pm path com.ahyq.multichannel

# 从而获取该应用
adb pull /data/app/com.ahyq.multichannel-2/base.apk

# 获取失败也可以先通过 shell 拷贝到 sdcard 上，然后再拷贝到本地
adb pull /sdcard/base.apk

adb shell pm list packages       # 列出所有app
adb shell pm list packages -3    # 列出除了系统应用的第三方应用包名

adb install test.apk             # 安装应用, -r 安装到 SD 卡
adb uninstall cn.com.test.mobile # 卸载应用，需要指定包
```
