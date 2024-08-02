

platformio.ini

使用调试功能要添加 `debug_tool = stlink`

```sh
[env:weact_mini_h743vitx]
platform = ststm32
board = weact_mini_h743vitx
framework = stm32cube
debug_tool = stlink
```
