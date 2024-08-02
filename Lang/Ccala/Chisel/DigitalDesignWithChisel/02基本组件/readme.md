

类型系统:

三大类型:
UInt, SInt 和 Bool 

```scala
// 常量
0.U                 // 无符号型常量
-3.S                // 有符号型常量
3.U(4.W)            // 指定宽度, 一个可能的错误 3.U(4) 忘记加 W, 则表示提取第4位

"hff".U             // 十六进制表示
"o377".U            // 八进制
"b1111_1111".U      // 二进制表示
'A'.U               // 字符常量
true.B              // 布尔常量

// 组合逻辑
val and = a & b
val w = Wire(UInt ()) // 可以先定义一个线型
w := a & b            // 然后再进行赋值
val sign = x(31)      // 提取某一位
val sign = x(7, 0)    // 指定提取位置
val word = h ## l     // ## 运算符将两个拼接起来

// 多路选择器
val r = Mux(sel, a, b)

// 寄存器
val reg = RegInit (0.U(8.W))    // 定义一个8位寄存器，在重置时用0初始化：
reg := d                        // 更新寄存器

// 计数器
val cntReg = RegInit (0.U(8.W))
cntReg := Mux(cntReg === 9.U, 0.U, cntReg + 1.U)
// 等价于
when (cntReg === 9.U) { cntReg := 0.U  } .otherwise {cntReg := cntReg + 1.U }
```


## 结构

Chisel提供了两种结构来对信号进行分: Bundle 和 Vec



