https://dl.sipeed.com/shareURL/Maix-Zero/M0P/M0P%20Dock

https://wiki.sipeed.com/hardware/zh/maixzero/m0p/m0p_dock.html

# 2026-01-20 14:08:07

sdk v2.13.4 

windows msys2 下修改 project.build:

```sh
# 末尾的 & 变成 ; 按顺序执行，而不是后台 &, 否则输出的 defconfig.cmake 会乱
$(CMAKE) -E echo "set($(var) $(value $(var)))" >> $(BUILD_DIR)/generated/defconfig.cmake.new& \

# 改成 Ninja
cmake_generator = "MSYS Makefiles"
```

