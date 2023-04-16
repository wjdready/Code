

# LVGL 8 布局

参考 [CSS flex 布局](https://www.runoob.com/w3cnote/flex-grammar.html)


LVGL 主要有两种布局, 弹性布局 LV_LAYOUT_FLEX 和 网格布局 LV_LAYOUT_GRID.

在样式中设置布局方式可以通过 

```c
lv_style_set_layout(&style, LV_LAYOUT_FLEX);
```

提示: 可通过 lv_obj_set_style_pad_row 来设置子项目行之间的间隔， lv_obj_set_style_pad_colum 设置列之间间隔

## 1. flex

### 1.1 flex_flow

设置容器内子项的排列方式

```c
lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
```

lv_obj_set_flex_flow 等价于:

```c
void lv_obj_set_flex_flow(lv_obj_t * obj, lv_flex_flow_t flow)
{
    lv_obj_set_style_flex_flow(obj, flow, 0);
    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, 0);
}
```

> 即当调用 lv_obj_set_flex_flow 后, 函数帮我们将布局设置成了 flex 布局。

该属性是 css 中的 flex-direction 和 flex-wrap属性的结合体, 有以下几种可选的排列方式:

```c
LV_FLEX_FLOW_ROW                    // 从左到右水平排列 (不自动换行)
LV_FLEX_FLOW_COLUMN                 // 从上到下垂直排列 (不自动换行)
LV_FLEX_FLOW_ROW_WRAP               // 从左到右水平排列 (自动换行)
LV_FLEX_FLOW_ROW_REVERSE            // 从右到左水平排列 (不自动换行)
LV_FLEX_FLOW_ROW_WRAP_REVERSE       // 从右到左水平排列 (自动换行)
LV_FLEX_FLOW_COLUMN_WRAP            // 从上到下垂直排列 (自动换行)
LV_FLEX_FLOW_COLUMN_REVERSE         // 从下到上垂直排列 (不自动换行)
LV_FLEX_FLOW_COLUMN_WRAP_REVERSE    // 从下到上垂直排列 (自动换行)
```

> WRAP 表示不换行, 默认不自动换行, REVERSE 表示反转

当选择换行后, 如果 flex 父控件空间放不下, 则会自动添加滚动模式.



### 1.2 lv_obj_set_flex_grow

类似于 flex-grow 属性，决定了在容器中有多余空间时子项目应该增长的比例, **是针对子项目设置的**, 默认为 0, 例如:

```
lv_obj_set_flex_grow(child1, 1);
lv_obj_set_flex_grow(child2, 2);
lv_obj_set_flex_grow(child3, 1);
```

则父容器被分为4份，中间的子项目占2份，其余两个各占一份.


### 1.3 lv_style_set_flex_main_place

用于设置弹性盒子（Flexbox）布局中子项目在主轴上的对齐方式, 类似于 css 中的  justify-content 属性

有如下选择可选:

```c
LV_FLEX_ALIGN_START,            // 对齐于子项目排列的起始处
LV_FLEX_ALIGN_END,              // 对齐于子项目排列的结束位置
LV_FLEX_ALIGN_CENTER,           // 对齐于中心
LV_FLEX_ALIGN_SPACE_EVENLY,     // 让每个子项目之间留有相等的空间, 包括项目和边框之间间隔
LV_FLEX_ALIGN_SPACE_AROUND,     // 让每个子项目周围留有相等的空间, 包括项目和边框之间间隔
LV_FLEX_ALIGN_SPACE_BETWEEN     // 最边上的两个项目对齐，项目之间的间隔都相等
```

该属性如果子项目中设置了 lv_obj_set_flex_grow 属性则失效

### 1.4 动态属性

子项目之间的间隔等属性是可以动态改变和刷新的


### 1.5 lv_obj_set_style_base_dir

lv_obj_set_style_base_dir 也可以改变布局的排列顺序, 例如 

```
lv_obj_set_style_base_dir(cont, LV_BASE_DIR_LTR, 0);
```

项目将从左到右排序, 等价于 row 中的 LV_FLEX_ALIGN_END, 但不同的是 `lv_obj_set_style_base_dir` 会将滚动条在内的布局空间也都改变了方向.

### 1.5 lv_obj_set_style_flex_cross_place


等价于 CSS 中 align-items 属性, 定义项目在交叉轴上如何对齐

```c
lv_obj_set_style_flex_track_place(cont_row, LV_FLEX_ALIGN_CENTER, 0);
```

对于多个高度或宽度不一致的项目，lv_obj_set_style_flex_track_place 可以让他们以中心或边缘对齐.

### 1.6 lv_obj_set_style_flex_track_place

类似于 align-content 属性，定义了多根轴线的对齐方式。如果项目只有一根轴线，该属性不起作用。

当存在多个主轴时 (使用了 WRAP 换行), lv_obj_set_style_flex_track_place 将设置多个轴之间的对齐方式.

比如:

```c
lv_obj_set_style_flex_track_place(cont_row, LV_FLEX_ALIGN_CENTER, 0);
```

将一个或多个主轴以中心对齐排列。


