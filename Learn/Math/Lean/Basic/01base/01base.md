

```lean
#eval 1 + 2 * 5
#eval String.append "it is " ( if 1 > 2 then "yes" else "no" )
-- 默认是自然数
#check (1 - 2)
#check (1 - 2 : Int)

-- 函数定义
def add1 (n : Nat) : Nat := n + 1
#eval add1 7

-- 多个参数的函数
def maximum (n : Nat) (k : Nat) : Nat :=
    if n < k then k else n

def maximum (n k : Nat) : Nat :=
  if n < k then k else n

#eval maximum 123 32

-- 将输出函数的签名
#check maximum

-- 添加 () 将输出函数具有的类型 Nat -> Nat -> Nat
#check (maximum)

-- 多参数函数本质上是单参数函数的柯里化, 比如下面得到的类型是 Nat -> Nat 
#check (maximum 3)

-- 定义类型, 然后 Str 就可以像 String 一样使用
def Str : Type := String

-- abbrev 定义时 Lean 不会展开它，而是简单地将其替换为其定义的表达式
abbrev N : Type := Nat
def x : N := 38

```


## 结构体

```lean
structure Point where
  x : Float
  y : Float
deriving Repr

def origin : Point := { x := 0.0, y := 0.0 }

-- 计算两点间的距离
def distance (p1 : Point) (p2 : Point) : Float :=
  Float.sqrt (((p2.x - p1.x) ^ 2.0) + ((p2.y - p1.y) ^ 2.0))

#eval distance { x := 1.0, y := 2.0 } { x := 5.0, y := -1.0 }

-- 结构体常量
#check ({ x := 0.0, y := 0.0 } : Point)

--当然也可以在花括号内进行类型注释
#check ({ x := 0.0, y := 0.0 } : Point)

-- 更新部分值, Lean 无法修改某个定义的值，只能获取后返回新的值
def zeroX (p : Point) : Point :=
    { p with x := 0 }

def fourAndThree : Point :=
  { x := 4.3, y := 3.4 }

#eval zeroX fourAndThree
#eval fourAndThree

-- 使用构造函数
#check Point.mk 1.5 2.8

-- 在结构体命名空间中定义函数, 该函数接收一个函数, 对自身 p 进行处理
def Point.modifyBoth (f : Float → Float) (p : Point) : Point :=
  { x := f p.x, y := f p.y }

#eval fourAndThree.modifyBoth Float.floor
```

