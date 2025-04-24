
## Android.mk 文件

定义一个动态库模块如下:

```makefile
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# 模块名称以及最终输出的库文件名称
LOCAL_MODULE := audio.primary.$(TARGET_BOARD_HARDWARE)

# 源文件列表
LOCAL_SRC_FILES := \
	audio_hw.c

# 相对(RELATIVE)于 `system` 目录输出一个新的目录 hw. 
# 模块会输出到 `system/lib/hw/` 以及 `/system/lib64/hw/` 
LOCAL_MODULE_RELATIVE_PATH := hw 

# 标记模块是专有的 (PROPRIETARY) 
# 这时 LOCAL_MODULE_RELATIVE_PATH 相对的是 `/system/vender` 这个专门用于开发商模块的路径,
# 这时模块会输出到 `/system/vendor/lib/hw/` 以及 `/system/vendor/lib64/hw/`
LOCAL_PROPRIETARY_MODULE := true 

# 头文件列表
LOCAL_HEADER_LIBRARIES += libhardware_headers

# 编译选项
LOCAL_CFLAGS := -Wno-unused-parameter
LOCAL_CFLAGS += -DLIBTINYALSA_ENABLE_VNDK_EXT
LOCAL_CFLAGS += -Wno-error

# 依赖的库列表
LOCAL_SHARED_LIBRARIES := liblog libcutils libtinyalsa

# 头文件路径列表
LOCAL_C_INCLUDES += \
	external/tinyalsa/include

# 指定编译生成动态库
include $(BUILD_SHARED_LIBRARY)
```


