

Lua 中有 8 个基本类型分别为：nil、boolean、number、string、userdata、function、thread 和 table。


| 类型     | 描述                                                                                                                            |
| -------- | ------------------------------------------------------------------------------------------------------------------------------- |
| nil      | 这个最简单，只有值nil属于该类，表示一个无效值（在条件表达式中相当于false）。                                                    |
| boolean  | 包含两个值：false和true。                                                                                                       |
| number   | 表示双精度类型的实浮点数                                                                                                        |
| string   | 字符串由一对双引号或单引号来表示                                                                                                |
| function | 由 C 或 Lua 编写的函数                                                                                                          |
| userdata | 表示任意存储在变量中的C数据结构                                                                                                 |
| thread   | 表示执行的独立线路，用于执行协同程序                                                                                            |
| table    | Lua 中的表（table）其实是一个"关联数组"（associative arrays），                                                                 |
|          | 数组的索引可以是数字、字符串或表类型。在 Lua 里，table 的创建是通过"构造表达式"来完成，最简单构造表达式是{}，用来创建一个空表。 |


## table

Lua 中的表（table）其实是一个"关联数组"（associative arrays），数组的索引可以是数字或者是字符串。

```lua
-- 默认自动创建了 key 为数字的索引
local table = {"apple", "pear", "orange", "grape"}

table["fruit"] = "banana"
table.test = "test"

-- 索引
print(table["fruit"])
print(table.fruit)

table[100] = "ok"
for i = 1, #table do
    print(table[i])
end

for k, v in pairs(table) do
    print(k .. " : " .. v)
end
```

## 函数

函数本身也是一个值，可以赋值给变量

```lua
local function add(a, b)
    return a + b
end

addfunc = add

c = addfunc(1, 2)

print(c)

function doFunc(thefun, ...)
    c = thefun(...)
    print(c)
end

doFunc(add, 1, 2)

-- 传入匿名函数
doFunc(
    function (a, b) 
        return a + b
    end, 
1, 2)
```


## thread 线程

在 Lua 里，最主要的线程是协同程序（coroutine）。它跟线程（thread）差不多，拥有自己独立的栈、局部变量和指令指针，可以跟其他协同程序共享全局变量和其他大部分东西。

线程跟协程的区别：线程可以同时多个运行，而协程任意时刻只能运行一个，并且处于运行状态的协程只有被挂起（suspend）时才会暂停。

## userdata（自定义类型）
userdata 是一种用户自定义数据，用于表示一种由应用程序或 C/C++ 语言库所创建的类型，可以将任意 C/C++ 的任意数据类型的数据（通常是 struct 和 指针）存储到 Lua 变量中调用。

## 模块与C包

Lua提供了一个名为require的函数用来加载模块。要加载一个模块，只需要简单地调用就可以了

`require("模块名")`


执行 require 后会返回一个由模块常量或函数组成的 table，并且还会定义一个包含该 table 的全局变量。

```lua
require("module")
print(module.constant)
module.func3()

-- 或者给加载的模块定义一个别名变量
local m = require("module")
print(m.constant)
m.func3()
```


require 会搜索 package.path 中指定的路径来加载模块。LUA_PATH 环境变量可以往里面添加路径。

C 包可以直接加载 C 动态库


## luajit

```lua
-- 加载 ffi 库
local ffi = require("ffi")

-- 定义 user32.dll 中的 MessageBox 函数
ffi.cdef[[
    int MessageBoxA(void* hWnd, const char* lpText, const char* lpCaption, unsigned int uType);
]]

-- 获取 user32.dll 库
local user32 = ffi.load("user32")

-- 定义消息框的文本、标题和样式
local text = "这是一个消息框"
local caption = "消息"
local style = 0

-- 调用 MessageBox 函数显示消息框
user32.MessageBoxA(nil, text, caption, style)
```

## 面向对象


Lua 中面向对象
我们知道，对象由属性和方法组成。

Lua 中的类可以通过 table + function 模拟出来。

至于继承，可以通过 metetable 模拟出来（不推荐用，只模拟最基本的对象大部分实现够用了）。

在 Lua 中，最基本的结构是 table，我们可以使用表（table）来创建对象。

```lua
ClassName = {}  -- 创建一个表作为类
```