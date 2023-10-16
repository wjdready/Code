

一个典型的着色器有下面的结构:

```c
#version version_number

in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

layout是一个关键字，用于在着色器程序中指定变量的布局和属性。
在顶点着色器中，使用layout关键字可以指定输入和输出变量的位置、类型和其他属性。
在片段着色器中，layout关键字可以用来指定输出变量的位置和类型。
通过使用layout关键字，我们可以精确地控制着色器程序中变量的布局和属性，以便与其他部分的渲染管线正确地进行数据交换。

```c
layout(location = 0) in vec3 aPos; // 设置变量位置为0
layout(location = 1, smooth) in vec3 aNormal; // 设置变量位置为1，并使用平滑限定符
layout(location = 2, index = 0) in vec2 aTexCoord; // 设置变量位置为2，并使用索引属性为0
layout(location = 0) out vec4 FragColor; // 设置输出变量位置为0
```
location = 0  用于指定着色器程序中输入或输出变量的位置。具体来说， location = 0  表示该输入或输出变量位于索引0的位置。这意味着该变量将与索引0相关联，以便在渲染管线的其他阶段正确地使用和处理。

