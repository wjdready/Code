* config XXXX 或者 menuconfig XXXX 表示一个配置节点的开始, XXXX 是将要形成的宏定义名称, 例如
  ```kconfig
  config BSP_USING_GPIO
    bool "Enable GPIO"
    default y
  ```
  > bool 生成的类型是布尔型, 对于 bool 型, 如果 true, 则生成该宏定义, 如果 flase 则不生成. 比如上例中选择为 true 后, 生成的 .h 中将有宏定义:
  ```
  #define BSP_USING_GPIO
  ```
  > default 表示该配置的默认值
* depends on XX
  表示一个配置是否可配置取决于 XX 是否配置了, 例如
  ```
    config A
        bool "Enable A"
        depends on B
    config B
        bool "Enable B"
  ```
  > 若 B 没有配置, 则 A 将没有出现在配置中, 只有当 B 配置了, 才能配置 A, 其实等同于
  ```
    if B
        config A
            bool "Enable A"
    endif
    config B
        bool "Enable B"
  ```
* select
  表示当一个配置被配置后, 开启另一个配置
  ```
  config A
    bool "Enable A"
    select C
  config C
    bool "Enable C"
  ```
  > 若配置 A, 则 C 也被配置了, 而且是强制性的, 这时 C 不可选.

* menuconfig 可用来设置配置子菜单
  例如下面是综合例子
  ```
    menuconfig A
      bool "Enable A"
      select C
      config B
        bool "Enable B"
        depends on A
    config C
      bool "Enable C"
  ```
> A 配置里面还可以进去选择配置 B, 前提是 B 要设置 depends on A.
