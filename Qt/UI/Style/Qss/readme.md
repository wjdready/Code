
# Qss

首先参考 [css 选择器](https://www.runoob.com/cssref/css-selectors.html), 注意不同点 

参考[这篇博客](https://www.cnblogs.com/bclshuai/p/9809679.html), 他列出了很多属性

这个[开源项目](https://github.com/mumble-voip/mumble-theme), 给出了很多主题, 可以参考里面属性的使用

官方的 [stylesheet 参考文档](https://doc.qt.io/qt-5/stylesheet-reference.html), 列出了所有的相关信息和描述

官方 [qss 语法](https://doc.qt.io/qt-5/stylesheet-syntax.html) 说明

1. Qt 中的 QSS 的基本语法格式：
    对象名称 {属性名1 : 属性值1; 属性名2 : 属性值2; ...}

2. Qt 中特有的属性：
    qproperty-xxx: 属性值;      //表示设置 Qt 对象属性值，其中 xxx 是某个属性名

3. Qt 中的图形对象属性：

```php
    color: color;              //表示前景色
    background-color: color;   //表示背景色
    border-width: double;      //表示边框宽度
    border-radius: double;     //表示圆角半径
    border-style: style;       //表示边框样式
    font-family: string;       //表示字体
    font-size: int;            //表示字体大小
    font-weight: int;          //表示字体粗细
    font-style: style;         //表示字体样式
```

* CSS 和 QSS 的不同之处：
  1. CSS 属性更丰富，QSS 仅针对 Qt 对象。
  2. CSS 只能对 HTML 元素进行定义，而 QSS 可以对任意 Qt 对象指定样式。
  3. CSS 使用 class 或 id 来指定某个元素的样式，而 QSS 使用对象的名称来指定样式。
  4. QSS 实现了一些特殊功能，比如可以将属性值设置为多重值. 可以生成图形边框等等。


#### QSS 选择器
  
##### 1. 通配选择器

对所有的对象实例进行属性设置

```css
* {
    background-color: red;
}
```

##### 2. 类型选择器

匹配所有 QPushButton 类的实例, 包括继承自 QPushButton 的子类的实例

```css
QPushButton {
    background-color: red;
}
```

#### 3. 类选择器

匹配所有 QPushButton 类的实例, 但不包括继承自 QPushButton 的子类的实例

```css
/* 这相当于 *[class ~= "QPushButton"] */
.QPushButton {
    background-color: red;
}
```

#### 4. 属性选择器

只匹配 QPushButton 中属性 flat 值为 false 的对象实例

```css
QPushButton[flat = "false"] {
    background-color: red;
}
```
#### 5. id 选择器

比如, UI 上有一个 QPushButton 的对象实例, 名字叫 btnEntry, 那么单独对这个 button 进行样式设置时可以这样:

```css
QPushButton#btnEnter {
    background-color: red;
}

QPushButton[objectName = btnEnter] {
    background-color: red;
}
```

这两种写法是等效的, 因为, 这里的 id 实际上就是 objectName 指定的值, 所以可以也可以直接使用属性设置的方法来进行设置。

当然属性设置的话可以更灵活一些, 比如支持一些正则运算符:

```css
/* 匹配 btn 开头的 QPushButton 实例 */
QPushButton[objectName ^= btn] {
    background-color: red;
}

/* 匹配包含 btn 子串的 QPushButton 实例 */
QPushButton[objectName *= btn] {
    background-color: red;
}


/* 匹配 Send 结尾的 QPushButton 实例 */
QPushButton[objectName $= Enter] {
    background-color: red;
}
```


##### 6. 多个选择器

多个选择器用 `,` 分开, 每个选择器独立进行匹配, 但使用同一个属性配置

```css
/* 匹配所有 QPushButton和 QLabel 实例 */
QPushButton, QLabel {
    background-color: red;
}

/* 匹配 QPushButton 中名为 btn1 和 btn2 的实例 */
QPushButton#btn1, #btn2 {
    background-color: red;
}

QPushButton[objectName ^= btn], QLabel[objectName *= lab] {
    background-color: red;
}
```

##### 7. 后代选择器

```css
/* 匹配 QFrame 对象实例内的所有后代中类型为 QPushButton 的实例 */
QFrame QPushButton {
    background-color: red;
}

/* 匹配指定 QFrame 对象内的所有后代中类型为 QPushButton 的实例 */
QFrame[objectName = myfram] QPushButton {
    background-color: red;
}
```

这个对于自己定义一些继承自 QWidget 的控件是非常有帮助的, 比如使用 QListWidget 时经常需要定义 ListItem, 对于 ListItem 里面自定义对象的筛选就可以这样做:

```css
QWidget[objectName = ListItem] QLabel {
    background-color: red;
}
```

##### 8. 子选择器

```css
/* 匹配所有父亲是 QFrame 的所有 QPushButton 对象实例 */
QFrame > QPushButton {
    background-color: red;
}

/* 匹配所有父亲类型是 QFrame, 且父亲名称是 myfram 的所有 QPushButton 对象实例 */
QFrame[objectName = myfram] > QPushButton {
    background-color: red;
}
```

#### 9. 子控件选择器

[官方详情文档](https://doc.qt.io/qt-5/stylesheet-reference.html#list-of-sub-controls) 中列出了所有支持的子控件

一个 QWidget 可能包含有多个子控件, 他们具有树形层次结构, 比如 QComboBox 除了最外层的盒子模型外还包含, 下拉 (drop-down) 子控件，和向下箭头子控件 (down-arrow), 对这些子控件进行样式设置可以通过 `::` 符号来选择, 比如

```css
/* 设置向下箭头背景为红色 */
QComboBox::drop-arrow {
    background-color: red;
}

/* 设置下拉框鼠标悬停时为白色 */
QComboBox::drop-down :hover {
    background-color: white;
}
```

##### 10. 伪状态

[官方详情文档](https://doc.qt.io/qt-5/stylesheet-reference.html#list-of-pseudo-states) 中列出了所有的伪状态

伪状态是用来添加一些 '选择器' 的特殊效果(特殊状态), 比如可以用来设置控件在鼠标悬停或点击等非默认状态的属性

```css
/* 当鼠标悬停时, 设置背景为红色 */
QPushButton#btnEnter:hover  {
    background-color: red;
}

/* 当鼠标点击时, 设置背景为红色 */
QPushButton#btnEnter:pressed {
    background-color:green;
}

/* 鼠标悬停但没按下 */
QPushButton:hover:!pressed { 
    color: blue; 
}
```

#### 另外

element + element 好像不生效? 

对于 css 来说, 比如 `div+p` 表示选择所有紧跟在 `<div>` 元素之后的第一个 `<p>` 元素

不过这个对 QT 来说, 意义不大, 因为每个对象的顺序可能是不一定的

### QT 特有的属性

一般来说, QT 特有属性都以 qproperty 开头:

```css
qproperty-xxx: 属性值; 
```

一下列出一些常见的特有属性:

```css

QWidget {
    qproperty-pixmap: url(pixmap.png);
    qproperty-titleColor: rgb(100, 200, 100);
    qproperty-iconSize: 20px 20px;
    qproperty-alternatingRowColors: true;
    selection-background-color: darkblue; /*  */
    opacity: 223; /* 不透明度, 0 ~ 255, 仅支持 QToolTip */
}
```

