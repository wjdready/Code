

csproj 文件是 C# 项目的核心配置文件，用于定义项目的结构、依赖项、编译选项等。

以下是常见节点及其作用的详解：

`<Project>`

定义项目的根节点，包含所有其他节点

## 1. `<PropertyGroup>`

常见属性：

```xml
<PropertyGroup>
  <!-- 指定输出类型：Exe（可执行文件）或 Library（类库-默认） -->
  <OutputType>Exe</OutputType>

  <!-- 目标框架，例如 .NET 7 -->
  <TargetFramework>net7.0</TargetFramework>

  <!-- 自动引入常用的命名空间，减少手动添加 using 语句的需求
       常见自动引入的命名空间（以 .NET 6+ 为例）：
         System
         System.Collections.Generic
         System.Linq
         System.Threading.Tasks
       等其他常用命名空间 -->
  <ImplicitUsings>enable</ImplicitUsings>

  <!-- 启用或禁用 (默认) C# 的可空引用类型功能，作用效果：
       启用后，编译器会强制检查可能的空引用问题。
       非可空类型（如 string）默认不允许为 null，
       除非显式声明为可空类型（如 string?）-->
  <Nullable>enable</Nullable>

  <!-- 根命名空间 -->
  <RootNamespace>MyNamespace</RootNamespace>

  <!-- 定义生成的程序集（.dll 或 .exe 文件）的名称 -->
  <AssemblyName>MyAssembly</AssemblyName>

  <!-- 指定目标运行时，例如 win-x64, linux-x64, osx-x64 -->
  <RuntimeIdentifier>win-x64</RuntimeIdentifier>

  <!-- ============================= 发布相关 ============================== -->

  <!-- 启用单文件发布 -->
  <PublishSingleFile>true</PublishSingleFile>

  <!-- 启用 AOT 编译 (不能和启用单文件发布一起使用) -->
  <PublishAot>true</PublishAot>

  <!-- 是否包含运行时 (经实践，似乎默认true) -->
  <SelfContained>true</SelfContained>

  <!-- 去除未使用的程序集 -->
  <PublishTrimmed>true</PublishTrimmed>

  <!-- 压缩单文件 -->
  <EnableCompressionInSingleFile>true</EnableCompressionInSingleFile>

  <!-- 优化首选项 -->
  <OptimizationPreference>size</OptimizationPreference>
</PropertyGroup>
```

更多优化大小策略, 参考: https://blog.csdn.net/csdnnews/article/details/135399235

```sh
dotnet publish -p:PublishAot=true -p:OptimizationPreference=Size -p:StackTraceSupport=false -p:InvariantGlobalization=true -p:UseSystemResourceKeys=true 
```

## 2. `<ItemGroup>`

定义项目中的文件、引用和其他资源

```xml
<ItemGroup>
  <!-- 包含源代码文件 -->
  <Compile Include="Program.cs" />

  <!-- NuGet 包引用 -->
  <PackageReference Include="Newtonsoft.Json" Version="13.0.1" />

  <!-- 引用其他项目 -->
  <ProjectReference Include="..\Library\Library.csproj" />

  <!-- 非代码文件 -->
  <None Include="appsettings.json" CopyToOutputDirectory="Always" />

  <!-- 使用本地 dll -->
  <Reference Include="MyLib">
      <HintPath>path\to\MyLib.dll</HintPath>
      <!-- Private true (默认)时将自动拷贝到输出目录 -->
      <Private>true</Private>
  </Reference>
</ItemGroup>
```

## 3. `<Target>`

定义自定义构建任务

Message 可能需要 `dotnet build -v diag` 才能看到

```xml
<Target Name="CustomTask" AfterTargets="Build">
  <Message Importance="high" Text="Build completed!" />
  <Copy SourceFiles="appsettings.json" DestinationFolder="bin\Release" />
  <Delete Files="bin\Debug\temp.txt" />
  <Exec Command="echo Hello, World!" />
  <MakeDir Directories="bin\Release\Logs" />
  <RemoveDir Directories="bin\Debug\Temp" />
  <WriteLinesToFile File="bin\Release\output.txt" Lines="Build successful!" Overwrite="true" />
</Target>
```

| Target 属性      | 描述                                                   |
| ---------------- | ------------------------------------------------------ |
| Name             | 任务的名称（必须唯一）                                 |
| BeforeTargets    | 指定在某个目标之前执行任务                             |
| AfterTargets     | 指定在某个目标之后执行任务                             |
| DependsOnTargets | 指定任务依赖的其他目标，确保依赖目标先执行, 比如 clean |
| Condition        | 指定条件，只有满足条件时才会执行任务                   |


## 4. `<Import>`

导入其他 MSBuild 文件

```xml
<Import Project="$(MSBuildToolsPath)\Microsoft.CSharp.targets" />
```

## 5. `<ItemDefinitionGroup>`

定义默认的编译选项

```xml
<ItemDefinitionGroup>
  <Compile>
    <!-- 设置警告级别 -->
    <WarningLevel>4</WarningLevel>
  </Compile>
</ItemDefinitionGroup>
```

## 6. `<PropertyGroup>` 条件属性

根据条件设置不同的属性

```xml
<PropertyGroup Condition="'$(Configuration)' == 'Debug'">
  <DefineConstants>DEBUG;TRACE</DefineConstants>
</PropertyGroup>
```

## 7. `<UsingTask>`

注册自定义任务

```xml
<UsingTask TaskName="MyTask" AssemblyFile="MyTask.dll" />
```

