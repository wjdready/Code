---
mytags: myblog
title: Android 命令行工具的使用
date: 2023-09-19 08:24:00
categories: [Android, SDK]
tags: [Android]
---

本文主要介绍 Android 命令行工具的使用
<!-- more -->

Android [命令行工具](https://developer.android.com/tools) 

### 下载命令行工具

在 Android Studio 下载界面最下面，[仅下载命令行工具](https://developer.android.com/studio)

当然也可以自动下载 (下载地址可以在弹出同意协议时右击获取下载链接)

```sh
mkdir -p android-sdk
export ANDROID_SDK_ROOT=android-sdk
export PATH=$PATH:$ANDROID_SDK_ROOT/cmdline-tools/latest/bin
export PATH=$PATH:$ANDROID_SDK_ROOT/platform-tools

TOOLS_URL=https://dl.google.com/android/repository/commandlinetools-win-10406996_latest.zip

if [ ! -d "$ANDROID_SDK_ROOT/cmdline-tools" ]; then
    mkdir -p $ANDROID_SDK_ROOT/cmdline-tools

    wget -c $TOOLS_URL -O cmdline-tools-latest.zip

    unzip cmdline-tools-latest.zip -d $ANDROID_SDK_ROOT/cmdline-tools
    mv $ANDROID_SDK_ROOT/cmdline-tools/cmdline-tools $ANDROID_SDK_ROOT/cmdline-tools/latest

    # 希望顺便下载平台工具
    sdkmanager.bat "platform-tools"
fi
```

### 创建模拟器

```sh
# 列出所有包
sdkmanager.bat --list

# 安装模拟器和对应的包
sdkmanager.bat "platforms;android-23"

# 安装模拟器和系统镜像 (选 x86 本机运行会快很多)
sdkmanager.bat "emulator" "platform-tools" "system-images;android-23;google_apis;x86_64"

# 创建设备 
avdmanager.bat create avd --force --name Nexus6P --abi google_apis/x86_64 --package 'system-images;android-23;google_apis;x86_64' --device "Nexus 6P"

# 列出所有设备
avdmanager.bat list avd

# 启动设备
./android-sdk/emulator/emulator -avd Nexus6P -skin 1440x2560

# 支持 adb 调试, 需下载 platform-tools
adb devices
```

### 创建工程

使用 Android Studio 创建

gradle 下载慢, 使用国内镜像源, 修改 `gradle/wrapper/gradle-wrapper.properties`

添加下载地址前缀 `mirrors.huaweicloud.com/gradle`

```sh
# 命令行编译
./gradlew.bat build
```

> 可能会出现 JDK 版本不对导致编译失败，设置好对应版本即可

编译后可生成未签名的 apk

#### 手动打包

参考官网[命令行构建方式](https://developer.android.com/build/building-cmdline)

[更加极端的方式 (仅供参考)](https://authmane512.medium.com/how-to-build-an-apk-from-command-line-without-ide-7260e1e22676)

```sh
# 下载构建工具
export PATH=$PATH:$ANDROID_SDK_ROOT/build-tools/34.0.0
sdkmanager.bat "build-tools;34.0.0"
```
