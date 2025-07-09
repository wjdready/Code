
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

## 使用 Cmake 构建

```sh
# 需要使用 Ninja 构建, cmake -B build -G ninja
cmake_minimum_required(VERSION 3.22.1)

# 最低支持 API 级别
set(ANDROID_PLATFORM android-21)

# 设置目标平台 armeabi-v7a/arm64-v8a/x86/x86_64
set(ANDROID_ABI armeabi-v7a)

# 设置 ndk 路径:
if(NOT DEFINED ANDROID_NDK)
    set(ANDROID_NDK ${CMAKE_CURRENT_SOURCE_DIR}/../../ndk/26.3.11579264)
endif()

set(CMAKE_TOOLCHAIN_FILE ${ANDROID_NDK}/build/cmake/android.toolchain.cmake)

project(mylib)

add_library(${CMAKE_PROJECT_NAME} SHARED
    add.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log
)
```
