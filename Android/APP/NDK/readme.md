
## 下载 cmdline-tools

```sh
export ANDROID_SDK_ROOT=$PWD/android-sdk
export PATH=$PATH:$ANDROID_SDK_ROOT/cmdline-tools/latest/bin
export PATH=$PATH:$ANDROID_SDK_ROOT/platform-tools
export PATH=$PATH:$ANDROID_SDK_ROOT/ndk-bundle

TOOLS_URL=https://dl.google.com/android/repository/commandlinetools-win-10406996_latest.zip

if [ ! -d "$ANDROID_SDK_ROOT/cmdline-tools" ]; then
    mkdir -p $ANDROID_SDK_ROOT/cmdline-tools

    wget -c $TOOLS_URL -O cmdline-tools-latest.zip

    unzip cmdline-tools-latest.zip -d $ANDROID_SDK_ROOT/cmdline-tools
    mv $ANDROID_SDK_ROOT/cmdline-tools/cmdline-tools $ANDROID_SDK_ROOT/cmdline-tools/latest

    # 下载平台工具和 NDK
    sdkmanager.bat platform-tools ndk-bundle
fi
```

## Hello demo

```sh
mkdir demo && cd demo

vim jni/Android.mk
# .. 输入下面 Android.mk 的内容

vim main.cpp
# .. 输入下面 main.cpp 的内容

# 进行编译
ndk-build.cmd
```

main.cpp

```cpp
#include <stdio.h>

int main()
{
    printf("Hello from NDK executable!\n");
    return 0;
}
```

Android.mk

```makefile
LOCAL_PATH := $(call my-dir)  
 
include $(CLEAR_VARS)  
 
LOCAL_MODULE := my_executable  
LOCAL_SRC_FILES := ../main.cpp
 
include $(BUILD_EXECUTABLE) # BUILD_SHARED_LIBRARY
```

要在编译系统时自动添加比如 my_executable 则需要添加在相应的地方添加

PRODUCT_PACKAGES += my_executable

从而在编译时自动将命令添加到 system/bin
