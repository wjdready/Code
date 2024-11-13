


## 调试心得

可以先生成一个简单的虚拟声卡 `mysimplecard`，然后使用 i2cset 的方式配置 adc， 并测试硬件, 当摸清楚 adc 配置后再写成驱动

