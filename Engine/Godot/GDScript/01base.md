


场景由多个节点组成, 节点以树的形式管理。每一个节点都可以附加脚本用来控制节点

每个节点脚本本身就是一个对象，对内部数据访问可通过 self 来访问


```sh
# 继承 Sprite2D
extends Sprite2D

var a = 1
var b:int = 2

# 实现该虚函数，初始化节点树时被调用
func _enter_tree():
	print(a + self.b + add(1))
	var arr = [1, 3, 1]
	print(findmax(arr))

# 每帧执行
func _process(delta):
	self.position.x += 1

# 自定义函数, 参数可以指定默认值以及返回值类型
func add(a, b:int = 1) -> int:
	return a + b

func findmax(arr):
	var max = arr[0]
	for a in arr:
		if max < a:
			max = a
	return max
```


## 内置单例

在帮助->搜索帮助: @GlobalScope 中可以找到 Godot 中内置的所有全局范围的常量和函数。

#### Input 单例

get_action_strength 函数接受一个动作名称作为参数，并返回一个介于 0 和 1 之间的值

动作名称可以在 项目->项目设置->输入映射中添加新动作, 每个动作可以绑定多个物理按键，从而支持多个输入设备。

```sh
func _physics_process(delta):
	var step = 500 * delta

	var left = Input.get_action_strength("move_left")
	var right = Input.get_action_strength("move_right")
	var up = Input.get_action_strength("move_up")
	var down = Input.get_action_strength("move_down")

	self.position.x += (right - left) * step
	self.position.y += (down - up) * step
```
