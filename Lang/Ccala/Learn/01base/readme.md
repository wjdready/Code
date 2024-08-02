

```scala
object HelloWorld {
    def main(args: Array[String]): Unit = {
        val byteValue: Byte = 127
        println(s"Hello, world! $byteValue")
    }
}
```

变量类似于 rust 冒号后面才是类型, 但也可省略和自动推导

var VariableName : DataType [=  Initial Value]

val 是可不变变量, var 可可变变量

```sh
scalac HelloWorld.sc
scala HelloWorld
```

## 数据类型

Scala 没有像 Java 中的原生类型（primitive types）那样的概念。尽管 Scala 的基本数据类型（如 Int、Double 等）在语法上看起来类似于 Java 的原生类型，但它们实际上是对象。这意味着你可以在这些类型上调用方法

Unit 类型只有一个实例值，用字面量 () 表示，类似于 Java 中的 void。

## Scala 方法与函数

函数本身就是继承了 Trait 的类的对象

```scala
class Test {
  def m(x: Int) = x + 3
  val f = (x: Int) => x + 3
}
```

函数定义格式

```scala
def functionName ([参数列表]) : [return type] = {
   function body
   return [expr]
}
```

## Scala 闭包

在 Scala 中，闭包（Closure）是一种函数，它能够捕获并包含其定义环境中的自由变量。

闭包是一个函数，返回值依赖于声明在函数外部的一个或多个变量。

闭包可以访问函数外部的变量并在函数内部使用这些变量，即使在变量的作用域已经超出其定义环境时，闭包仍然可以访问这些变量。

闭包的关键在于它可以"关闭"函数外部的变量，使得这些变量在函数内仍然可用。

```scala

// 这样定义的函数变量 multiplier 成为一个"闭包"，因为它引用到函数外面定义的变量，
// 定义这个函数的过程是将这个自由变量捕获而构成一个封闭的函数。
//  外部变量的值改变时，闭包内的计算结果也相应改变
var factor = 3
val multiplier = (i:Int) => i * factor
```

##### 闭包作为返回值

```scala
object ClosureExample2 {
  def main(args: Array[String]): Unit = {
    def makeAdder(adder: Int): Int => Int = {
      (x: Int) => x + adder
    }

    val addFive = makeAdder(5)
    val addTen = makeAdder(10)

    println(addFive(3))  // 输出 8
    println(addTen(3))   // 输出 13
  }
}
```

##### 闭包与函数式编程

闭包在函数式编程中是一个重要的概念，广泛用于高阶函数、柯里化等技术中。

以下实例，highOrderFunction 函数接受一个函数作为参数。传递的闭包捕获了外部变量 externalVar。

```scala
object ClosureExample4 {
  def main(args: Array[String]): Unit = {
    def highOrderFunction(f: Int => Int, x: Int): Int = f(x)

    val externalVar = 5
    val closure = (x: Int) => x + externalVar

    println(highOrderFunction(closure, 10))  // 输出 15
  }
}
```

## 字符串

在 Scala 中，字符串的类型实际上是 Java String，它本身没有 String 类。

```scala
var s = "Hello"
s += " World " + 12
println(s"$s")
printf("%.1f 0x%02X\n", 3.14, 145)
```

## 数组

Scala 语言中提供的数组是用来存储固定大小的同类型元素

```scala
var z:Array[String] = new Array[String](3)

// 通过索引方法访问数组元素
for (i <- z.indices) {  
    z(i) = s"$i"  
}

// 切片
val sub = z.slice(1, 3)

// 这种方法返回的数组项不可写
for (x <- sub) {
    println(s"$x")
}
```


## 集合 

```scala
// 定义整型 List
val x = List(1,2,3,4)

// 定义 Set
val x = Set(1,3,5,7)

// 定义 Map
val x = Map("one" -> 1, "two" -> 2, "three" -> 3)

// 创建两个不同类型元素的元组
val x = (10, "Runoob")

// 定义 Option
val x:Option[Int] = Some(5)
```

## 迭代器

Scala Iterator（迭代器）不是一个集合，它是一种用于访问集合的方法。

迭代器 it 的两个基本操作是 next 和 hasNext。

调用 it.next() 会返回迭代器的下一个元素，并且更新迭代器的状态。

调用 it.hasNext() 用于检测集合中是否还有元素。

```scala
 val it = Iterator("Baidu", "Google", "Runoob", "Taobao")
     
while (it.hasNext){
    println(it.next())
}

```


## 类和对象

```scala
import java.io._

class Point(xc: Int, yc: Int) {
   var x: Int = xc
   var y: Int = yc

   def move(dx: Int, dy: Int) {
      x = x + dx
      y = y + dy
      println ("x 的坐标点: " + x);
      println ("y 的坐标点: " + y);
   }
}

object Test {
   def main(args: Array[String]) {
      val pt = new Point(10, 20);

      // 移到一个新的位置
      pt.move(10, 10);
   }
}
```


#### 继承

Scala 只允许继承一个父类

```scala
class Point(xc: Int, yc: Int) {
   var x: Int = xc
   var y: Int = yc

   def move(dx: Int, dy: Int) {
      x = x + dx
      y = y + dy
      println ("x 的坐标点: " + x);
      println ("y 的坐标点: " + y);
   }
}

class Location(override val xc: Int, override val yc: Int,
   val zc :Int) extends Point(xc, yc){
   var z: Int = zc

   def move(dx: Int, dy: Int, dz: Int) {
      x = x + dx
      y = y + dy
      z = z + dz
      println ("x 的坐标点 : " + x);
      println ("y 的坐标点 : " + y);
      println ("z 的坐标点 : " + z);
   }
}
```




#### 单例

在 Scala 中，是没有 static 这个东西的，但是它也为我们提供了单例模式的实现方法，那就是使用关键字 object。

```scala
import java.io._

class Point(val xc: Int, val yc: Int) {
   var x: Int = xc
   var y: Int = yc
   def move(dx: Int, dy: Int) {
      x = x + dx
      y = y + dy
   }
}

object Test {
   def main(args: Array[String]) {
      val point = new Point(10, 20)
      printPoint

      def printPoint{
         println ("x 的坐标点 : " + point.x);
         println ("y 的坐标点 : " + point.y);
      }
   }
}
```

##  Trait(特征)

Scala Trait(特征) 相当于 Java 的接口，实际上它比接口还功能强大。

与接口不同的是，它还可以定义属性和方法的实现。

一般情况下Scala的类只能够继承单一父类，但是如果是 Trait(特征) 的话就可以继承多个，从结果来看就是实现了多重继承。

```scala
trait Equal {
  def isEqual(x: Any): Boolean
  def isNotEqual(x: Any): Boolean = !isEqual(x)
}

class Point(xc: Int, yc: Int) extends Equal {
  var x: Int = xc
  var y: Int = yc
  def isEqual(obj: Any) =
    obj.isInstanceOf[Point] &&
    obj.asInstanceOf[Point].x == x
}

object Test {
   def main(args: Array[String]) {
      val p1 = new Point(2, 3)
      val p2 = new Point(2, 4)
      val p3 = new Point(3, 3)

      println(p1.isNotEqual(p2))
      println(p1.isNotEqual(p3))
      println(p1.isNotEqual(2))
   }
}
```


## 模式匹配

```scala
object Test {
   def main(args: Array[String]) {
      println(matchTest(3))

   }
   def matchTest(x: Int): String = x match {
      case 1 => "one"
      case 2 => "two"
      case _ => "many"
   }
}
```