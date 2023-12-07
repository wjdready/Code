
教程: https://www.w3school.com.cn/css/index.asp

## 基本

```html
<!DOCTYPE html>
<html>

<body class="base">

</body>

<style>
    .base {
        background-color: rgb(255, 255, 255);
    }
</style>

</html>
```

## 继承

属性都有一个 `inherit` 值用于继承自父标签的对应属性

## margin

将 margin 属性设置为 auto，以使元素在其容器中水平居中

```css
.box {
    margin: 50px auto;
    border: 1px solid black;
    width: 50px;
}
```

> 例子中并且存在多个元素时，上下间距 50px

## 高度和宽度

`max-width` 用来设置元素最大宽度，当浏览器宽度小于 `max-width` 时，元素宽度会随着浏览器改变而改变。

当浏览器宽度大于 `max-width` 时，则元素宽度不再变化。因为达到了最大宽度。

`min-width` 同样也是随浏览器大小而变化，不同的是当浏览器太小时它会触发滚动条，当浏览器足够大，则不需要滚动条。

两者可同时存在，从而将保证元素的宽度总是在某个范围之间变化。

> 以上讨论对 `heigh` 同样适用


## box 模型

元素(高宽)->内边距->边框->外边距

##### 轮廓 outline

轮廓是在边框之外绘制的一条线，以凸显元素


## display

display 属性规定是否/如何显示元素

每个 HTML 元素都有一个默认的 display 值，具体取决于它的元素类型。大多数元素的默认 display 值为 block 或 inline

块级元素（block element）总是从新行开始，并占据可用的全部宽度（尽可能向左和向右伸展）
行内元素（inline element）不从新行开始，仅占用所需的宽度

##### 覆盖默认的 Display 值

设置元素的 display 属性仅会更改元素的显示方式，而不会更改元素的种类。因此，带有 display: block; 的行内元素不允许在其中包含其他块元素

隐藏元素 - display:none 还是 visibility:hidden？

通过将 display 属性设置为 none 可以隐藏元素。该元素将被隐藏，并且页面将显示为好像该元素不在其中
但是，该元素仍将占用与之前相同的空间。元素将被隐藏，但仍会影响布局

## CSS 定位

position 属性规定应用于元素的定位方法的类型（static、relative、fixed、absolute 或 sticky）

元素其实是使用 top、bottom、left 和 right 属性定位的。但是，除非首先设置了 position 属性，否则这些属性将不起作用。根据不同的 position 值，它们的工作方式也不同。


* position: static (默认使用);

静态定位的元素不受 top、bottom、left 和 right 属性的影响

position: static; 的元素不会以任何特殊方式定位；它始终根据页面的正常流进行定位。

* position: relative;

position: relative; 的元素相对于其正常位置进行定位。

设置相对定位的元素的 top、right、bottom 和 left 属性将导致其偏离其正常位置进行调整。不会对其余内容进行调整来适应元素留下的任何空间。

* position: fixed;
position: fixed; 的元素是相对于视口定位的，这意味着即使滚动页面，它也始终位于同一位置。 top、right、bottom 和 left 属性用于定位此元素。
固定定位的元素不会在页面中通常应放置的位置上留出空隙。

* position: absolute;
position: absolute; 的元素相对于最近的定位祖先元素进行定位（而不是相对于视口定位，如 fixed）。

然而，如果绝对定位的元素没有祖先，它将使用文档主体（body），并随页面滚动一起移动。

注意：“被定位的”元素是其位置除 static 以外的任何元素。

* position: sticky;
position: sticky; 的元素根据用户的滚动位置进行定位。

粘性元素根据滚动位置在相对（relative）和固定（fixed）之间切换。起先它会被相对定位，直到在视口中遇到给定的偏移位置为止 - 然后将其“粘贴”在适当的位置（比如 position:fixed）


##### 重叠元素

在对元素进行定位时，它们可以与其他元素重叠。z-index 属性指定元素的堆栈顺序（哪个元素应放置在其他元素的前面或后面）

注意：如果两个定位的元素重叠而未指定 z-index，则位于 HTML 代码中最后的元素将显示在顶部

## overflow 

overflow 属性指定在元素的内容太大而无法放入指定区域时是剪裁内容还是添加滚动条。

* visible - 默认。溢出没有被剪裁。内容在元素框外渲染
* hidden - 溢出被剪裁，其余内容将不可见
* scroll - 溢出被剪裁，同时添加滚动条以查看其余内容
* auto - 与 scroll 类似，但仅在必要时添加滚动条


## float

CSS float 属性规定元素如何浮动

CSS clear 属性规定哪些元素可以在清除的元素旁边以及在哪一侧浮动

float 属性用于定位和格式化内容，例如让图像向左浮动到容器中的文本那里。

float 属性可以设置以下值之一：

left - 元素浮动到其容器的左侧
right - 元素浮动在其容器的右侧
none - 元素不会浮动（将显示在文本中刚出现的位置）。默认值。
inherit - 元素继承其父级的 float 值
最简单的用法是，float 属性可实现（报纸上）文字包围图片的效果


#### 消除浮动导致父元素高度坍缩的两种方法:

参考浮动解读: https://zhuanlan.zhihu.com/p/34057752

首先浮动元素会脱离文档流并向左/向右浮动，直到碰到父元素或者另一个浮动元素, 而带有 clear 属性的元素其左边/右边不允许挨着浮动元素, 如果挨着就换行

```html
<style>
    .cont {
        border: 2px solid green;
        /* 方法1. 父容器设置 overflow, 避免孩子内容溢出 */
        /* overflow: auto; */
    }

    /* 方法 2 的更优雅解决方法, 利用 ::after 在父元素末尾插入空内容, 其原理和方法 2 一样, table 保证了其为块元素 */
    /* .cont::after {
        content: "";
        clear: both;
        display: table;
    } */

    .box {
        background-color: orange;
        width: 50px;
        height: 50px;
        margin: 20px;
        float: left;
    }
</style>

<div class="cont">
    <div class="box"></div>
    <div class="box"></div>
    <div class="box"></div>
    <!-- 方法2: 在末尾添加一个带 clear 属性的空元素，其会被安排在浮动元素下一行，从而父元素将高度计算到这个地方 -->
    <div style="clear: both;"></div>
</div>
```

#### 利用浮动进行布局

```html
<style>
    * {
        box-sizing: border-box;
    }

    .header,
    .footer {
        background-color: grey;
        color: white;
        padding: 15px;
    }

    .column {
        float: left;
        padding: 15px;
    }

    .clearfix::after {
        content: "";
        clear: both;
        display: table;
    }

    .menu {
        width: 25%;
    }

    .content {
        width: 75%;
    }

    .menu ul {
        list-style-type: none;
        margin: 0;
        padding: 0;
    }

    .menu li {
        padding: 8px;
        margin-bottom: 8px;
        background-color: #33b5e5;
        color: #ffffff;
    }

    .menu li:hover {
        background-color: #0099cc;
    }
</style>

<body>

    <div class="header">
        <h1>Shanghai</h1>
    </div>

    <div class="clearfix">
        <div class="column menu">
            <ul>
                <li>The Flight</li>
                <li>The City</li>
                <li>The Island</li>
                <li>The Food</li>
            </ul>
        </div>

        <div class="column content">
            <h1>The City</h1>
            <p>aa</p>
            <p>bb</p>
        </div>

    </div>

    <div class="footer">
        <p>Footer Text</p>
    </div>

</body>
```

## display: inline-block

与 display: inline 相比，主要区别在于 display: inline-block 允许在元素上设置宽度和高度。

同样，如果设置了 display: inline-block，将保留上下外边距/内边距，而 display: inline 则不会。

与 display: block 相比，主要区别在于 display：inline-block 在元素之后不添加换行符，因此该元素可以位于其他元素旁边。

利用 inline-block 实现的导航栏

```html
<style>
    .nav {
        background-color: rgb(66, 65, 65);
        list-style-type: none;
        text-align: center;
        margin: auto;
        padding: 0;
        font-size: 0;
        max-width: fit-content;
    }

    .nav li {
        display: inline-block;
        font-size: 20px;
        padding: 20px;
        background-color: bisque;
        /* margin-left: -5px; */
    }

    .nav li:hover {
        background-color: rgb(75, 74, 73);
    }

</style>

<div class="nav">
    <li><a href="#">Home</a></li>
    <li><a href="#">About</a></li>
    <li><a href="#">Contact</a></li>
</div>
```

需要注意的是因为 inline-block 会在一行内，为了格式方便我们是在 html 中的换行会引入到 div 中，从而导致 inline-block 元素之间存在空格

故上面消除空格的方法就是将父级字体设置为 0。此外已知空格大小为 5px 似乎可以为每一个元素都向左移动 5px 来覆盖看不见的空格

参考: https://zhuanlan.zhihu.com/p/629638853

## 对齐

要使块元素（例如 `<div>` ）水平居中，请使用 margin: auto;。
设置元素的宽度将防止其延伸到容器的边缘。
然后，元素将占用指定的宽度，剩余空间将在两个外边距之间平均分配：

```html
<style>
    .center {
        margin: auto;
        width: 60%;
        border: 3px solid #73AD21;
        padding: 10px;
    }
</style>

<div class="center">
    <p>Hello World!</p>
</div>
```

> 如果未设置 width 属性（或将其设置为 100％），则居中对齐无效

居中文本:
如果仅需在元素内居中文本，请使用 `text-align: center;`

居中图像
如需居中图像，请将左右外边距设置为 auto，并将其设置为块元素：

左和右对齐 - 使用 `position: absolute;` 配合 top、bottom、left 和 right
> 注意：绝对定位的元素将从正常流中删除，并可能出现元素重叠

垂直对齐: 使用 padding 让上下填充相等
使用 line-height 设置行高
使用 Flexbox

## flexbox 弹性布局

参考: https://blog.csdn.net/greendx/article/details/129527774

display: flex
flex-direction: 设置方向
flex-wrap: 容器内的项目是否自动换行, 默认不自动换行
flex-flow: 是 flex-direction 和 flex-wrap 的简写
justify-content: 用于容器内项目对齐方式
align-items: 内部项目的水平轴对齐方式
align-content: 多个水平轴的对齐方式

