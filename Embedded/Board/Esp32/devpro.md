
### [2023-02-26 20:32:00] 重拾

#### 开发环境

参考[官方文档](https://idf.espressif.com/zh-cn/index.html)

Windows 平台直接提供安装器, 使用起来也越来越方便, 以前比较复杂, 现在还提供镜像加速, 完全傻瓜式了, 只需按照提示来即可.

需要注意的是平台安装软件会先检查环境中是否有 Python, 如果没有它会一起下载， 否则直接使用环境中的 Python, 这时应当保证自己的 Python 版本是 3.8 以上的. 因为我发现 IDF 版本 5.0 需要用到 Python 的异步功能, 而 Python 3.8 以下没有, 没有也是可以编译的就是每次都需要输入 --no-hints

对于更换 Python 版本后, 只需点击 idf 目录下的 `install.bat` 即可重新配置对应版本的 Python 虚拟环境, 或者按提示执行 `idf_tools.py install-python-env`.

在环境变量中添加如下 esp.bat 脚本, 每次在 CMD 命令行中输入 esp 即可快速导出开发环境, 同时避免了环境变量的污染.

```
C:\ProgramFiles\Library\Espressif\frameworks\esp-idf-v5.0.1\export.bat
```

#### idf.py 功能

[官方文档](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-py.html)

idf.py --help

```
add-dependency               Add dependency to the manifest file. For now we only support adding dependencies from the component
                            registry.
all                          Aliases: build. Build the project.
app                          Build only the app.
app-flash                    Flash the app only.
bootloader                   Build only bootloader.
bootloader-flash             Flash bootloader only.
build-system-targets         Print list of build system targets.
clean                        Delete build output files from the build directory.
confserver                   Run JSON configuration server.
create-component             Create a new component.
create-manifest              Create manifest for specified component.
create-project               Create a new project.
create-project-from-example  Create a project from an example.
delete-version               (Deprecated) Deprecated! New CLI command: "compote component delete". Delete specified version of the
                            component from the component registry.
docs                         Open web browser with documentation for ESP-IDF
efuse-common-table           Generate C-source for IDF's eFuse fields.
efuse-custom-table           Generate C-source for user's eFuse fields.
encrypted-app-flash          Flash the encrypted app only.
encrypted-flash              Flash the encrypted project.
erase-flash                  Erase entire flash chip.
erase-otadata                Erase otadata partition.
flash                        Flash the project.
fullclean                    Delete the entire build directory contents.
gdb                          Run the GDB.
gdbgui                       GDB UI in default browser.
gdbtui                       GDB TUI mode.
menuconfig                   Run "menuconfig" project configuration tool.
monitor                      Display serial output.
openocd                      Run openocd from current path
pack-component               (Deprecated) Deprecated! New CLI command: "compote component pack". Create component archive and store it
                            in the dist directory.
partition-table              Build only partition table.
partition-table-flash        Flash partition table only.
post-debug                   Utility target to read the output of async debug action and stop them.
python-clean                 Delete generated Python byte code from the IDF directory
read-otadata                 Read otadata partition.
reconfigure                  Re-run CMake.
save-defconfig               Generate a sdkconfig.defaults with options different from the default ones
set-target                   Set the chip target to build.
show-efuse-table             Print eFuse table.
size                         Print basic size information about the app.
size-components              Print per-component size information.
size-files                   Print per-source-file size information.
uf2                          Generate the UF2 binary with all the binaries included
uf2-app                      Generate an UF2 binary for the application only
upload-component             (Deprecated) Deprecated! New CLI command: "compote component upload". Upload component to the component
                            registry. If the component doesn't exist in the registry it will be created automatically.
upload-component-status      (Deprecated) Deprecated! New CLI command: "compote component upload-status". Check the component uploading
                            status by the job ID.
```

查看 API 文档 `idf.py docs -sp api-guides/build-system.html`

##### 1. 创建新的项目

```shell
idf.py create-project good
cd good
idf.py build
```

##### 2. 列出当前支持的芯片

```
idf.py --list-targets
```


#### 3. 使用 Kconfig

[官方对 Kconfig 的介绍](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html)

如要在自己的项目中自定义 Kconfig. 可在项目的 main 文件夹下创建 Kconfig.projbuild 文件, 文件内容遵循 Kconfig 语法, 比如:

```kconfig
menu "THIS Project config"

    config USING_SPISD
        bool "Enable SPI SD Card"
        default n

    config SPISD_MOSI
        int "MOSI GPIO number"
        default 15 if IDF_TARGET_ESP32
        default 35 if IDF_TARGET_ESP32S2
        default 35 if IDF_TARGET_ESP32S3
        default 4  # C3 and others
        depends on USING_SPISD
 
endmenu
```

执行 `idf.py menuconfig` 就会在菜单顶层出现我们创建的菜单 `THIS Project config -->`. 

需要注意的是生成的宏和 linux kernel 的不一样, ESP 的宏会自动添加前缀 `CONFIG_`, 因此实际写菜单选项时不要再添加了...

例如上面的选项选中后会在 sdkconfig 文件中生成如下内容:

```shell
#
# THIS Project config
#
CONFIG_USING_SPISD=y
CONFIG_SPISD_MOSI=15
# end of THIS Project config
```

生成的头文件放在 `build/config/sdkconfig.h`, 然后就可以在 C 中引用了...


##### 4. 创建组件

[参考文档](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/build-system.html?highlight=idf_component_register#component-requirements)

IDF 框架中支持组件的概念, 而且官方包的所有内容也都是基于组件的形式提供的, 在工程目录中有以下三个组件相关的变量可以设置:

* COMPONENT_DIRS：组件的搜索目录，默认为 `IDF_PATH/components`、 `PROJECT_DIR/components`、和 `EXTRA_COMPONENT_DIRS`。如果您不想在这些位置搜索组件，请覆盖此变量。
* EXTRA_COMPONENT_DIRS：用于搜索组件的其它可选目录列表。路径可以是相对于项目目录的相对路径，也可以是绝对路径。
* COMPONENTS：要构建进项目中的组件名称列表，默认为 `COMPONENT_DIRS` 目录下检索到的所有组件。使用此变量可以“精简”项目以缩短构建时间。请注意，如果一个组件通过 `COMPONENT_REQUIRES` 指定了它依赖的另一个组件，则会自动将其添加到 `COMPONENTS` 中，所以 `COMPONENTS` 列表可能会非常短。


###### 使用 idf_component_register 将一个组件注册到构建系统

```
idf_component_register([[SRCS src1 src2 ...] | [[SRC_DIRS dir1 dir2 ...] [EXCLUDE_SRCS src1 src2 ...]]
                       [INCLUDE_DIRS dir1 dir2 ...]
                       [PRIV_INCLUDE_DIRS dir1 dir2 ...]
                       [REQUIRES component1 component2 ...]
                       [PRIV_REQUIRES component1 component2 ...]
                       [LDFRAGMENTS ldfragment1 ldfragment2 ...]
                       [REQUIRED_IDF_TARGETS target1 target2 ...]
                       [EMBED_FILES file1 file2 ...]
                       [EMBED_TXTFILES file1 file2 ...]
                       [KCONFIG kconfig]
                       [KCONFIG_PROJBUILD kconfig_projbuild]
                       [WHOLE_ARCHIVE])
```

其参数解释如下

* `SRCS` - 组件的源文件，用于为组件创建静态库；如果没有指定，组件将被视为仅配置组件，从而创建接口库。
* `SRC_DIRS、` `EXCLUDE_SRCS` - 用于通过指定目录来 glob 源文件 (.c、.cpp、.S)，而不是通过 SRCS 手动指定源文件。请注意，这受 CMake 中通配符的限制。 在 EXCLUDE_SRCS 中指定的源文件会从被 glob 的文件中移除。
* `INCLUDE_DIRS` - 相对于组件目录的路径，该路径将被添加到需要当前组件的所有其他组件的 include 搜索路径中。
* `PRIV_INCLUDE_DIRS` - 必须是相对于组件目录的目录路径，它仅被添加到这个组件源文件的 include 搜索路径中。
* `REQUIRES` - 组件的公共组件依赖项。
* `PRIV_REQUIRES` - 组件的私有组件依赖项；在仅用于配置的组件上会被忽略。
* `LDFRAGMENTS` - 组件链接器片段文件。
* `REQUIRED_IDF_TARGETS` - 指定该组件唯一支持的目标。
* `KCONFIG` - 覆盖默认的 Kconfig 文件。
* `KCONFIG_PROJBUILD` - 覆盖默认的 Kconfig.projbuild 文件。
* `WHOLE_ARCHIVE` - 如果指定了此参数，链接时会在组件库的前后分别添加 -Wl,--whole-archive 和 -Wl,--no-whole-archive。这与设置 WHOLE_ARCHIVE 组件属性的效果一致。
* `EMBED_FILES` - 嵌入组件的二进制文件
* `EMBED_TXTFILES` - 嵌入组件的文本文件


###### 在工程目录下创建组件

```shell
mkdir components
cd components
idf.py create-component demo
```

components 目录下创建了一个 demo 组件模板, 其内容如下

```
- demo/
    - include/
        - demo.h
    - CMakeLists.txt
    - demo.c
```

其中 CMakeLists.txt 内容为
```cmake
idf_component_register (
    SRCS "demo.c"
    INCLUDE_DIRS "include"
)
```

然后在主项目中引用组件只需在 `idf_component_register` `中添加 PRIV_REQUIRES demo` 或 `REQUIRES demo`. 其中, 前者表示私有依赖, 后者则为公共依赖. 

那什么时候改写成公共依赖, 什么时候该写成私有依赖呢? 当我们使用依赖的头文件时, 将它的 `#include` 语句放在头文件中, 则说明对该依赖是公共的, 若只写到源文件中, 则说明对该依赖是私有的.


如果不喜欢上述的文件编排, 比如想把源文件和头文件都放在一起也是可以的, 只需在`idf_component_register` 中说清楚头文件和源文件即可. 比如

```
- demo/
    - src/
        - demo.h
        - demo.c
    - CMakeLists.txt
```

```cmake
idf_component_register (
    SRCS "src/demo.c"
    INCLUDE_DIRS "src"
)
```

因为注册组件时, 构建系统只认 CMakeLists 给出的 SRCS 和 INCLUDE_DIRS 等相关信息, 而不会管文件夹具体的内容.


### [2023-03-01 23:30:54]

##### idf.py 切换 target chip

例如:

```shell
# 参数为空会列出支持的所有芯片
idf.py  set-target

idf.py  set-target esp32s3
```

#### 使用 PSRAM

在 menuconfig --> Component config --> ESP PSRAM 

选择 `Support for external, SPI-connected RAM` 即可使用, 不过有一些参数可供配置:

##### 1. Mode (QUAD/OCT) of SPI RAM chip in use (Quad Mode PSRAM)

用于配置 SPI PSRAM 是 OSPI 还是 QSPI 通信方式. 官方默认的 8 Pin SPIRAM 是使用 QSPI, 不知道是否真的支持 OSPI...

##### 2.  Type of SPIRAM chip in use (Auto-detect)

```
(X) Auto-detect
( ) ESP-PSRAM16 or APS1604
( ) ESP-PSRAM32
( ) ESP-PSRAM64 , LY68L6400 or APS6408
```

也就是说, 其实支持的芯片就是列出的这几个了, 所以要根据这里列出的芯片进行选型?

##### 3. [*] Allow external memory as an argument to xTaskCreateStatic

也就是说, 可以将 freertos 的静态任务创建在 SPI RAM 吗? 好像还不错...

##### 4. [ ] Cache fetch instructions from SPI RAM

Cache 从 PSI RAM 中取指令, 也就说代码是可以直接运行在 SPI RAM 上的, 然后用开启 Cache 加快执行速度.

##### 5. [ ] Cache load read only data from SPI RAM

Cache 可以从 SPI RAM 加载只读数据, 我觉得如果勾选上面的 `Cache fetch instructions from SPI RAM` 那这个肯定也要勾选了啊, 因为既然要从 SPI RAM 执行指令, 那么必然要把固件从 flash 复制到 RAM, 不论是从 SD 卡的 SDIO Nand Flash 还是 SPI Nor flash, 那么不可避免原固件肯定有只读数据区和代码区, 那么采用 Cache 的方式就能加快只读区域访问速度

一般来说可写区域要么是堆栈, 要么是一块静态内存区. 不知支不支持 SPI RAM 写 Cache, 它也没这个选项...

##### 6. Set RAM clock speed (40Mhz clock speed)

写入速度支持 `40MHz, 80MHz, 120MHz`, 如果稳定的话, 那肯定是越快越好, 然而当我设置 120MHz 时根本编译不通过, 选 80 MHz 下载进去初始化 SPIRAM 失败.

