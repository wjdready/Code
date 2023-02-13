## template

该模板支持 **cmake** 和 **make** 两种构建方案。

`main` 文件夹为必须，且里面得包含含有 `app_main` 函数的源文件。

`components` 是可选的组件文件夹，该文件夹下可以包含多个组件，每个组件一个文件夹。而每个组件都有自己的 cmake 和 make 文件用来构建该组件。

在 `IDF_PATH` 下的 make 或 cmake 会自己调用 components 文件夹下的组件来连接到应用，只需管好单个组件能否成功编译，连接时就自动会连接上去了。


### cmake 
* 更改工程名：在工程文件夹下
  ```cmake
  project(template) # 改成你自己的工程名
  ```
* 编译、下载、监视命令为
  ```shell
  idf.py build flash monitor
  ```
  你可以只执行其中的一个或两个，清除可用 `globalclean`
* CMakeLists.txt 文件
  * 下面是用来设置源文件和头文件路径
    ```Cmake
    set(COMPONENT_SRCS "func.cpp"
                        "main.cpp")
    set(COMPONENT_ADD_INCLUDEDIRS ".")
    ```
  * 如果懒得一个一个输入源文件，直接输入源文件的文件夹
    ```Cmake
    set(COMPONENT_SRCDIRS ".")
    ``` 
  * 如果要依赖某个组件库
    ```Cmake
    set(COMPONENT_PRIV_REQUIRES nvs_flash)
    ```


### make 
* 更改工程名：在工程文件夹下
  ```make
  PROJECT_NAME := template # 改成你自己的工程名
  ```
* 编译、下载、监视命令为
  ```shell
  make build flash monitor
  ```
  你可以只执行其中的一个或两个
* Makefile 文件
  * 下面是用来设置源文件和头文件路径
    ```make
    COMPONENT_SRCS := func.cpp main.cpp
    COMPONENT_ADD_INCLUDEDIRS := .
    ```
  * 如果懒得一个一个输入源文件，直接输入源文件的文件夹
    ```make
    COMPONENT_SRCDIRS := .
    ``` 
  * 如果要依赖某个组件库
    ```make
    COMPONENT_PRIV_REQUIRES := nvs_flash
    ```
  