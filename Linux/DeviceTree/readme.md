

# 设备树

设备树在内核中放在 /sys/firmware 目录

该目录下 `devicetree` 文件夹是内核解析设备树后生成的文件树, `fdt` 则是设备树编译后的源文件

```sh
ls /sys/firmware

# -I(--in-format) 输入文件格式, -O(--out-format) 输出文件格式
# 这时可通过 grep 进行筛查, 比如 | grep hdmi -C 5
sudo dtc -I dtb -O dts /sys/firmware/fdt | grep hdmi -C 5
```


