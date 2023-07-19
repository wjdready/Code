
```s
cd myprojects
mkdir build
cd build
export PICO_SDK_PATH=../../pico-sdk

# cmake 后会在 build 生成各个 example 目录
cmake .. -G "MSYS Makefiles"

# 然后再进入对于目录进行构建即可
cd arduboy
make -j4
```
