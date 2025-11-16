
## API 目录

Scripting API
 - [Scripting API Objects](https://www.altium.com/documentation/altium-designer/script-api-objects?version=21)
   API Object 概念说明，包含 API 使用示例可下载
 - [Altium Design Software API](https://www.altium.com/documentation/altium-designer/altium-design-software-api?version=21)
    介绍各个模块API的基本概念和使用
    - Using the Altium Design Software API
    - Using the Client and Server API
    - Using the Schematic API
    - Using the PCB API
    - Using the WorkSpace Manager API
    - Using the Integrated Library API
 - [Altium Design Software API Reference](https://www.altium.com/documentation/altium-designer/altium-design-software-api-reference?version=21)
    详细介绍各个API的使用方法
    - Technical Reference - System API
    - Technical Reference - Schematic API
    - Technical Reference - PCB API
    - Technical Reference - Workspace Manager API
    - Technical Reference - Integrated Library AP

API 示例基本都是 vb 脚本，但支持 js，只需将 vb 转为 js 即可


#### 示例

### 1. Hello World

首先 创建 `script project`, 通过 `File->New->Script->Script Project`

然后创建脚本文件，但是 AD 不支持直接创建 js, 可以先创建 pas 文件，然后右击重命名成 js

```js
// hello.js
function main()
{
    showMessage("Hello World")
}
```

要运行脚本, 在脚本编辑界面时，直接运行菜单栏上的 `Run` 即可, 还可以打断点和单步调试

在任何地方都可以点击菜单栏 `File->Run Script` 进行运行，按 Ctrl 再点击时可以绑定快捷键, 推荐 `alt+x`

### 2. 交互式UI

AD 不支持直接创建 js 的 dfm 布局(form)文件, 只能先创建 pas 文件的 form, 然后再重命名保存，再删除 pas 文件. 保留 dfm 文件, 之后还需关闭 project 再加载才能生效.

不过也可以直接在 VSCode 中创建和 js 文件同名的 dfm 文件, 填入这些, 重新加载工程，则脚本会自动绑定界面

hello.dfm

```dfm
object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 193
  ClientWidth = 302
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
end
```

绑定界面后, 脚本编辑界面的左下方就会有 `code|form` 的切换按钮, 点击 form 则可编辑 UI 文件。右边有控件栏 (Tool Palette)可以拖动进来 (没有则点击右下角panels开启)

控件放在界面后，双击控件则会自动在对于的脚本文件插入控件的回调函数，例如点击事件等, 只需处理这部分逻辑即可

例如实现: 点击按钮，更改 label 的值

```js
var count = 0

function Button1Click(Sender) {
    count = count + 1;
    Form1.Label1.Caption = count.toString();
}
```

## 实用插件和参考

- 可视化交互式BOM表 https://blog.csdn.net/qlexcel/article/details/146430221

