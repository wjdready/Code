
## 开始

创建工程后, 会生成以下文件:

```sh
# 编辑器配置文件, 用于编辑器界面相关配置, .godot/ 默认会被放到 gitignore 中
.godot/
.editorconfig

# git 相关文件
.gitattributes
.gitignore

# 图标资源和导入文件
icon.svg
icon.svg.import

# 项目文件, 包含项目的所有配置文件
project.godot
```

其中最为重要的文件是 project.godot, 使用类似 ini 语法

```ini
config_version=5

[application]

config/name="第一次"
config/features=PackedStringArray("4.6", "Forward Plus")
config/icon="res://icon.svg"

[physics]

3d/physics_engine="Jolt Physics"

[rendering]

rendering_device/driver.windows="d3d12"
```

## 场景

每个项目必须有一个主场景, 在项目文件 project.godot 中配置 `run/main_scene="uid://dt6lgcnatjlmb"` 

场景由多个节点组成, 节点以树的形式管理。每一个节点都可以附加脚本用来控制节点

每个节点脚本本身就是一个对象，对内部数据访问可通过 self 来访问

每个脚本在创建时都会自动生成一个唯一的 id, 放在同脚本路径的 scriptname.gd.uid 文件中

每个场景和节点在创建时都会自动分配一个唯一的 id, 用于标识，包括场景中引用的资源。

例如如下场景, Sprite2D2 附加了脚本 sprite_2d_2.gd

    Node2D
    - Sprite2D
        -Timer
    - Sprite2D2 <-> sprite_2d_2.gd

```sh
[gd_scene format=3 uid="uid://dt6lgcnatjlmb"]

[ext_resource type="Script" uid="uid://cxxnx6g5t4ig" path="res://sprite_2d_2.gd" id="1_qdhxh"]

[node name="Node2D" type="Node2D" unique_id=1806152231]

[node name="Sprite2D" type="Sprite2D" parent="." unique_id=1264552522]

[node name="Timer" type="Timer" parent="Sprite2D" unique_id=654496620]

[node name="Sprite2D2" type="Sprite2D" parent="." unique_id=272764692]
script = ExtResource("1_qdhxh")
```

## 脚本 

godot 语言类似于 python, 通过缩进表示代码块的范围

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

## VSCode 

下载的 godot.exe 其可以当作命令本身，可以根据参数直接运行或是打开编辑器, 以下是一些常用的命令

```sh
# 运行当前游戏或者为空
godot --path .

# 打开编辑器
godot --editor

# 运行指定场景
godot --scene scenes/scene.tscn
```

官方出了一款插件 godot-tools 可以直接在 VSCode 中调试代码

安装完成后在 Debug 栏创建 launch.json 然后运行配置的 launch 项即可

不过其 LSP 服务需要 editor 本身在后台保持开启才有效


