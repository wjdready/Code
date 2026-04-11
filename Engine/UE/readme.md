
[官方文档](https://dev.epicgames.com/documentation/zh-cn/unreal-engine)

[UE C++ 开发入门文档](https://dev.epicgames.com/documentation/zh-cn/unreal-engine/programming-with-cplusplus-in-unreal-engine)

## 虚幻引擎

### 1. 编辑界面
编辑器快捷键：

单鼠标操作: 

鼠标左键 -- 左右旋转视角 + 前后移动
鼠标右键 -- 上下左右旋转视角
鼠标中键 -- 上下左右移动

鼠标键盘操作:
按住鼠标左键 + WASD  -- 前后左右移动 + 旋转视角, QE 是上下移动，ZC 是镜头拉近拉远

F 聚焦某个物体, 然后这时按住 Alt + 鼠标左键

Ctrl + Alt + 鼠标框选，可在场景中选中多个物体
Alt + 鼠标操作 可以快速复制

### 2. 工程

文件类型:

| 文件/目录         | git | 说明                                             |
| ----------------- | --- | ------------------------------------------------ |
| DerivedDataCache/ | NO  | 存储派生数据缓存，用于加速编辑器的加载和编译过程 |
| Intermediate/     | NO  | 存储中间文件，包括临时文件、编译输出等           |
| Saved/            | NO  | 存储自动保存的文件、日志文件、临时文件等         |
| Binaries/         | NO  | 存储编译生成的二进制文件                         |
| .vs/              | NO  | vs 生成用于存储用户特定的配置文件和缓存文件      |
|                   |     |                                                  |
| Config/           | YES | 存储项目的配置文件，包括游戏设置、输入设置等     |
| Content/          | YES | 存储所有的内容资源，包括材质、纹理、蓝图、关卡等 |
| Source/           | YES | 存储源代码文件                                   |
| .uproject         | YES | 项目文件，包含了项目的基本信息和配置             |
| Build/            | YES | 存储构建配置文件                                 |
| Plugins           | YES | 存储项目使用的插件                               |
| .sln              | YES | vs 解决方案                                      |
| .vsconfig         | YES | vs 配置文件                                      |


文件格式

| 格式                           | 说明                                                   |
| ------------------------------ | ------------------------------------------------------ |
| .uproject                      | 项目文件，包含有关项目的配置信息、插件信息等           |
| .umap                          | 场景文件，包含游戏场景中的所有元素，如地形、型、灯光等 |
| .uasset                        | UE4 的资源文件，包括纹理、材质、蓝图、声音等资源       |
| .uexp                          | 二进制资源导出文件，包含已导出的资源的二进制数据       |
| .uprojectdirs                  | 项目目录文件，包含项目所需的附加目录信息               |
| .uplugin                       | 插件文件，包含有关插件的元信息和配置                   |
| .uclass、.ufunction、.uenum    | 存储蓝图、函数和枚举等蓝图相关的数据                   |
| .umaterial、.umaterialinstance | 材质文件和材质实例文件，用于定义游戏中的材质           |

注意: .uasset 无法直接导入, 因为资源和别的文件一般会有关联信息

## 3. HelloWorld

创建一个可以旋转的 Actor, 放在 Source 里面, 编译后在内容菜单里面找到 C++ 类, 拖入场景, 右边属性栏中设置 `静态网络物体` 

MyActor.h

```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MyActor.generated.h"

UCLASS()
class AMyActor : public AActor
{
    GENERATED_BODY()

public:
    AMyActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationSpeed;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;
};
```

MyActor.cpp

```cpp
#include "MyActor.h"

AMyActor::AMyActor()
{
    PrimaryActorTick.bCanEverTick = true;
    RotationSpeed = 1.0f;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void AMyActor::BeginPlay()
{
    Super::BeginPlay();
}

void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime * 60.0f, 0.0f));
}
```

## 4. 使用 VSCode 开发

1. 设置默认编辑器为 VSCode

   安装插件 `C/C++ IntelliSense` 和 `C#`, 然后在 `编辑->编辑器偏好设置->通用->源代码` 中设置默认编辑器为 VSCode

   接着 `工具->刷新 VSCode 项目` 将会生成 `.code-workspace` 文件和 `.vscode` 文件夹, `.code-workspace` 主要提供 task 和 lunch 配置, `.vscode` 则主要提供代码提示和补全功能

   使用时直接打开 `.code-workspace` 文件, UE5 生成的工作区包含了一个自身项目和UE5源码, 这样就可以在开发过程中方便地查看源码了

2. 代码提示和补全

   VSCode 上实现智能提示和代码补全主要依靠 UE 生成的 `compileCommands.json` 文件，这个文件由 UE 工具自动生成，包含项目中每个源文件的编译命令信息, 包括头文件的路径、编译选项等

   若发现无法转跳或补全, 则需要在UE中选择: `工具->刷新 VSCode 项目`

3. 编译

   在不使用 VSCode 时，可以使用 UE5 右下角的编译按钮进行编译

   UE5 帮我们生成了多个 task 和 lunch 配置 (在 `.code-workspace` 里面)，在 VSCode 中选择 `终端->运行任务`, 然后选择对应的内容进行编译

   查看 `.code-workspace` 中地 task 可以发现, 编译过程其是调用了脚本 `UE_5.7\\Engine\\Build\\BatchFiles\\Build.bat` 里面地内容

   手动调用该脚本, 得到帮助信息, 发现可以生成各种构建工具的配置, 比如 CMake:
    
        "D:\\ProgramFiles\\Epic Games\\UE_5.7\\Engine\\Build\\BatchFiles\\Build.bat" -CMakefile -Project="$PWD\Demo.uproject"

    不过生成的 CMake 不过是间接调用 `build.bat` 罢了, 所以最终还是需要调用 `build.bat` 脚本, 但是查看 CMake 文件中列出的 `add_custom_target`, 可以更直观的看到命令执行参数
    
    因此可执行如下命令直接编译:

        "D:\\ProgramFiles\\Epic Games\\UE_5.7\\Engine\\Build\\BatchFiles\\Build.bat" -Project="$PWD\Demo.uproject" Demo Win64 DebugGame
    
    控制编译主要有三个参数
        
        目标                    平台            输出类型
        Demo                    Win64           DebugGame (本地调试版本)
        DemoEditor              Andorid         Development (开发测试版本)
        UnrealGame              IOS             Shipping (打包发布版本)
        UnrealEditor            ...             
        ...

    Demo 和 DemoEditor 是 UE5 生成的两个目标, 对应 `Demo.Target.cs` 和 `DemoEditor.Target.cs`, 用来编译编译自己的游戏项目

    UnrealGame 和 UnrealEditor 则是 UE5 源码的两个目标, 在 `Engine\\Source` 目录下有对应 `UnrealEditor.Target.cs` 和 `UnrealGame.Target.cs`

    当然源码自带还有一些目标, 尚且不知其用途, 但总之就是用来生成一些目标文件用的

    值得注意的是项目类型, 编译带 `Editor` 后缀生成的是 dll 文件, 然后作为插件可由 `Engine\\Binaries\\Win64\\UnrealEditor.exe` 运行, 主要用于热更新
    
    通过编辑器命令可以打开项目:

        "D:\\ProgramFiles\\Epic Games\\UE_5.7\\Engine\\Binaries\\Win64\\UnrealEditor.exe" "$PWD\\Demo.uproject"

    不带后缀则生成 exe 文件, 可直接运行进入游戏

4. 调试和运行

   用 VSCode 打开由 UE5 生成的 `.code-workspace`, 在 Debug 栏可以看到有很多 launch 选项, 点击运行即可, 开发过程推荐 launch Editor 模式, 这样可以快速运行调试和热更新

   不过有时 `.code-workspace` 生成的 launch 无法加载程序, 可能是生成器的bug, 这时手动指定正确的程序路径即可, 也不复杂

   值得注意的是如果新建工程就直接运行不带 `Editor` 的模式, 很可能会出现  `global shader library are missing`, 需要先进入 `Editor` 中选择 `平台->烘培内容->烘培内容` 

   烘培结束后就可以正常运行了, 推测是直接运行游戏需要 cook 之后的资源, 不能直接使用编辑器状态的资源

   发现调试模式正的很方便, 单步调试会自动转跳到工作区的 UE5 源码部分, 这对调试和理解工作原理方便的帮助很大 

   推荐下载引擎时选择 `选项->勾选输入调试用符号` 这样在

## 构建系统

构建系统使用 C# 作为配置语言, 全新的构建系统叫 UBT

构建需要有一个 Target (继承自 TargetRules), 构建脚本通常会去项目的 Source 目录查找 `.Target.cs` 文件, 如果列出的目标名刚好匹配上则执行编译

通常目标只是作为顶层, 其可以依赖多个 Module (继承自 ModuleRules) , 使用 `ExtraModuleNames.Add` 来添加模块

模块的编译配置文件使用 `.Build.cs` 结尾, 模块之间的依赖可以使用 `PublicDependencyModuleNames.AddRange` 来指定, 并且自动递归地编译依赖

## 插件

比如编译 Paper2D 插件, 可以在不创建工程的情况下编译, 方法是直接使用 UnrealEditor 目标来编译即可

    "D:\ProgramFiles\Epic Games\UE_5.7\Engine\Build\BatchFiles\build.bat" UnrealEditor Win64  DebugGame -Plugin="$PWD\Paper2D.uplugin"

当然也可以使用版本的方式, 但观察编译输出, 其实也是简介调用 UBT 来编译插件

    "D:\ProgramFiles\Epic Games\UE_5.7\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="$PWD\Paper2D.uplugin" -Package=Paper2D

