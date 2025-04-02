

# .NET

类似于 JVM，.NET 提供了公共语言运行时环境 (CLR)，它将公共中间语言（CIL）代码即时（JIT）编译成机器代码，并运行这些代码

.NET 支持的语言主要是 C#，但也支持 `VB.NET`，`F#` 等，这些语言被编译成中间语言（CIL），然后由 CLR 来执行。

.NET Framework 是 .NET 的初始实现，最开始 .NET Framework 只支持 Windows，而 mono 是一个社区的跨平台实现

后来微软出了个 .NET Core 跨平台了，但是由于 .NET Core 和 mono、.NET Framework 是不同的

虽然 mono 能跑大部分的 .NET Framework 程序集，但是 .NET Core 不行；而 mono 也不能跑 .NET Core 的程序集，.NET Core 也不能跑 mono 和 .NET Framework 的程序集。

由于 .NET 对库函数的引用类似动态链接库，程序集内并不包含库函数的实现，只包含库函数的签名，然后运行的时候才去加载对应的有实现的程序集完成“链接”过程最后调用，于是 .NET Standard 就应运而生了。

- .NET Standard 参考三个实现的情况，划定了一组 API 的子集，这组 API 在 .NET Framework、mono 和 .NET Core 上都有实现
- 然后使 .NET Framework、mono 和 .NET Core 都能加载 .NET Standard 程序集
- 这样当用户调用 .NET Standard 里的 API 的时候，会把调用转发到当前运行时的基础库的实现上。

这样一来，只要用户的代码基于 .NET Standard 编写，就能同时在 .NET Framework、mono、.NET Core 上跑了。

而如果要使用各自平台独有的 API 的话，则不能基于 .NET Standard 来编写代码，而需要基于 .NET Framework、.NET Core 或者 mono 来编写代码。

后来到了 .NET Standard 2.1 的时候，由于 .NET Framework 掉了队，不再新增新的功能，于是 .NET Standard 2.1 干脆不支持 .NET Framework 了，只支持 mono 和 .NET Core。

再后来 mono 和 .NET Core 完成了基础库的统一，变成了新的 .NET，于是 .NET Standard 的使命也结束了，只剩下一个统一的 .NET。

目前 .NET 5+ 已成为跨平台开发的标准方案

| C#版本  | .NET版本             | 发布日期   | 特性                             |
| ------- | -------------------- | ---------- | -------------------------------- |
| C# 1.0  | .NET Framework 1.0   | 2002-02-13 | 委托、事件                       |
| C# 1.1  | .NET Framework 1.1   | 2003-04-24 | APM（异步编程模型）              |
| C# 2.0  | .NET Framework 2.0   | 2005-11-07 | 泛型、匿名方法、迭代器、可空类型 |
| C# 3.0  | .NET Framework 3.0   | 2007-11-06 | 隐式类型                         |
|         | .NET Framework 3.5   | 2007-11-19 | 对象集合初始化、Lambda表达式等   |
| C# 4.0  | .NET Framework 4.0   | 2010-04-12 | 动态绑定、命名和可选参数等       |
| C# 5.0  | .NET Framework 4.5   | 2012-08-15 | 异步和等待、调用方信息           |
| C# 6.0  | .NET Framework 4.6   | 2015-07-20 | 静态导入                         |
|         | .NET Core 1.0        | 2016-06-27 |                                  |
| C# 7.0  | .NET Framework 4.6.2 | 2016-08-02 | 元组                             |
| C# 7.1  | .NET Framework 4.7   | 2017-04-05 |                                  |
|         | .NET Core 2.0        | 2016-08-14 |                                  |
| C# 7.2  | .NET Framework 4.7.1 | 2017-10-17 |                                  |
| C# 7.3  | .NET Framework 4.7.2 | 2018-04-30 |                                  |
|         | .NET Core 2.1        | 2018-05-30 |                                  |
|         | .NET Core 2.2        | 2018-12-04 |                                  |
| C# 8.0  | .NET Framework 4.8   | 2019-04-18 |                                  |
|         | .NET Core 3.0        | 2019-09-23 |                                  |
|         | .NET Core 3.1        | 2019-12-03 |                                  |
| C# 9.0  | .NET 5               | 2020-09-04 |                                  |
|         | .NET 5               | 2020-10-13 |                                  |
| C# 10.0 | .NET 6               | 2021-11-09 |                                  |
| C# 11.0 | .NET 7               | 2022-11-08 |                                  |
| C# 12.0 | .NET 8               | 2023-11-08 |                                  |
| C# 13.0 | .NET 9               | 2024-11-09 |                                  |

# Hello World!

```c#
using System;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Hello World!");
    }
}
```

# .NET SDK 

从 https://dotnet.microsoft.com/zh-cn/ 下载 .NET SDK 安装包，安装后配置环境变量

```sh
# 首先查看帮助，即可知道怎么使用
dotnet -h

# 创建 dotnet 工程
dotnet new console -o HelloWorld
cd HelloWorld
dotnet run

# 构建
dotnet build -c Release

# 发布
dotnet publish -c Release -o publish
# 或者指定发布的 TargetFramework 和 RuntimeIdentifiers
dotnet publish -c Release -f net8.0 -r win-x64

# 添加依赖
dotnet add package Newtonsoft.Json
dotnet add package System.IO.Ports
```


### dotnet workload

#### 1. maui

```sh
dotnet workload search maui
dotnet workload install maui

# 在当前文件夹, 创建 maui 工程, -o 指定文件夹
dotnet new maui 

# 根据 .csproj 文件中 中的 TargetFramework 来选择运行环境
# 当然如果在 vs 上则可以直接选择并点击运行
# vs 打开 .csproj 文件，然后 vs 会创建 sln 文件, 用于 c# 工程的管理
dotnet run -f net9.0-windows10.0.19041.0
```


## 调用动态库

### 1. 调用托管 DLL

托管 DLL 是用 .NET 语言（如 C#）编写的，可以直接通过引用程序集来使用。

在 Visual Studio 中，右键点击项目 -> 添加引用 -> 浏览并选择目标 DLL 文件。
或者使用命令行：`dotnet add reference path\to\your.dll`
然后在代码中引入 DLL 的命名空间并调用其方法。

假设 DLL 中有一个类 MathLibrary，包含一个方法 Add：

```c#
using MathLibrary; // 引用 DLL 的命名空间

class Program
{
    static void Main(string[] args)
    {
        int result = MathLibrary.Add(5, 3);
        Console.WriteLine($"结果是: {result}");
    }
}
```

### 2. 调用非托管 DLL

非托管 DLL 是用 C/C++ 等非 .NET 语言编写的，需要使用 P/Invoke（平台调用）来调用。

导入 DLL： 使用 [DllImport] 属性指定 DLL 文件和方法。
声明方法： 声明与 DLL 中方法签名匹配的静态方法。

假设有一个非托管 DLL NativeLibrary.dll，其中包含一个方法 int Add(int a, int b)：

```c#
using System;
using System.Runtime.InteropServices;

class Program
{
    // 导入非托管 DLL
    [DllImport("NativeLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern int Add(int a, int b);

    static void Main(string[] args)
    {
        int result = Add(5, 3);
        Console.WriteLine($"结果是: {result}");
    }
}
```

注意：
确保 DLL 文件在程序运行时可以被找到（例如放在项目的输出目录或系统路径中）。
如果是 64 位程序，请确保 DLL 是 64 位版本（32 位程序同理）。

### 3. 动态加载 DLL
如果需要在运行时动态加载 DLL，可以使用 Assembly.LoadFrom（托管 DLL）或 LoadLibrary（非托管 DLL）。

示例（托管 DLL 动态加载）：
根据你的 DLL 类型选择合适的方法即可。


```c#
using System;
using System.Reflection;

class Program
{
    static void Main(string[] args)
    {
        Assembly assembly = Assembly.LoadFrom("path\\to\\YourLibrary.dll");
        Type type = assembly.GetType("YourLibrary.ClassName");
        object instance = Activator.CreateInstance(type);
        MethodInfo method = type.GetMethod("MethodName");
        method.Invoke(instance, new object[] { /* 参数 */ });
    }
}
```

## 创建 classlib

```c#
// 生成 classlib 工程
// dotnet new classlib -n MyLib

/*
    要生成文档，在 .csproj 中添加
    <PropertyGroup>
    <GenerateDocumentationFile>true</GenerateDocumentationFile>
    </PropertyGroup>
*/

namespace MyLib;

/// <summary>
/// This is a sample class.
/// </summary>
public class Class1
{
    /// <summary>
    /// Adds two integers and returns the result.
    /// </summary>
    /// <param name="a">first value</param>
    /// <param name="b">second value</param>
    /// <returns>the result</returns>
    public int add(int a, int b)
    {
        return a + b;
    }
}
```

build 之后就会生成 xml 的使用文档了，提供 dll 和 xml 就能直接在其他程序中使用该库。

在引用该库时，后台代码解析器会通过 xml 文件生成一个 .cs 文件放在临时目录，转跳后就会进入临时目录的临时文件了

引用托管 dll 库:

```xml
<ItemGroup>
  <!-- 使用本地 dll -->
  <Reference Include="MyLib">
      <HintPath>path\to\MyLib.dll</HintPath>
      <!-- Private true (默认)时将自动拷贝到输出目录 -->
      <Private>true</Private>
  </Reference>
</ItemGroup>
```

##  为什么 WPF 生成的应用程序不需要运行时可以运行，而 .NET Core 应用程序不能？

WPF 应用程序：
    依赖于系统自带的 .NET Framework，无需额外安装运行时。
    老版本 Windows 10 默认包含 .NET Framework 4.x，因此可以直接运行。

.NET Core/5+/6+ 应用程序：
    需要额外的 .NET Core/5+/6+ 运行时，而老版本 Windows 10 并未预装这些运行时。
    如果未打包运行时（SelfContained=false），应用程序无法运行。

.NET Framework 已经停止更新，最新仍然停留在 4.8.1

可以通过 `TargetFramework` 使用 `.NET Framework`

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net472</TargetFramework>
    <RuntimeIdentifiers>win-x64</RuntimeIdentifiers>
  </PropertyGroup>

</Project>
```


## .NET Standard (已过时)

.NET Standard 是一个 跨平台的 API 规范，用于编写可以在多个 .NET 实现（如 .NET Framework、.NET Core、Mono 等）上运行的共享库。

它本身并不是一个运行时，因此无法直接生成可执行文件（.exe）。如果你需要输出 .exe 文件，必须将 .NET Standard 类库作为依赖项，使用支持生成可执行文件的目标框架（如 .NET Core 或 .NET Framework）来实现。

