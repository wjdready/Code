

翻译:
https://www.kernel.org/doc/Documentation/devicetree/bindings/sound/simple-card.txt


Simple-Card:

Simple-Card 描述了 SoC 和 codec 之间通过音频 DAI 的连接关系

必需的属性如下:

```js
compatible = "simple-audio-card";
```

可选属性如下:
```js
simple-audio-card,name = "my-sound"; // 用户指定的声卡名称，一个字符串属性
simple-audio-card,widgets = ""
```
