
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

编辑器中选择工具->新建C++类->选择父类为 Actor

MyActor.h

```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class DEMO4_API AMyActor : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AMyActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    // 每次旋转的角度
    float RotationSpeed;

    // 静态网格体组件
    // UPROPERTY(VisibleAnywhere) 宏使得 AActor 可以在实例化后对 MeshComponent 类型进行修改
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;
};
```

MyActor.cpp

```cpp
#include "MyActor.h"

AMyActor::AMyActor()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 初始化旋转速度为每帧1度
    RotationSpeed = 1.0f;

    // 创建静态网格体组件并将其附加到根组件
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 加载一个默认的静态网格体资源
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (MeshAsset.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshAsset.Object);
    }
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator NewRotation = GetActorRotation();
    // 每秒旋转多少度
    NewRotation.Yaw += RotationSpeed * DeltaTime * 60.0f; 
    // 设置新的旋转
    SetActorRotation(NewRotation);
}
```

然后点击编辑器右下角的编译按钮, 这时 MyActor 就可以在编辑器中显示了绑定的网格模型了, 将其拖入场景并运行就可以看到效果

## 4. 使用 VSCode 开发

1. 安装插件 `C/C++ IntelliSense` 和 `C#`, 然后再设置中设置默认编辑器为 VSCode

2. 代码提示和补全
   VSCode 上实现智能提示和代码补全主要依靠 UE 生成的 compileCommands.json 文件，这个文件由于 UE 工具自动生成，包含项目中每个源文件的编译命令信息, 包括头文件的路径、编译选项等。

   若发现无法转跳或补全, 则需要在UE中选择: 工具->刷新 VSCode 项目

3. 编译
   在不使用 VSCode 时，可以使用 UE5 右下角的编译按钮进行编译, 使用 VSCode 时，如果需要在 VSCode 上进行编译，需要在编译按钮旁边将 LiveCoding 关闭

   UE5 帮我们生成了多个 task 和 lunch 配置 (在 .code-workspace 里面)，在 VSCode 中选择终端->运行任务, 然后选择对应的内容进行编译

4. 调试
   其实 Editor 本身就是一个特殊的游戏模式，运行 Editor 其实就已经启动了 exe, 然后在 Editor 中运行时，Editor 会动态加载用户部分游戏 dll 模块, 每次任务编译其实是生成和更新该用户部分 dll

   所以 lunch 有 Editor 模式和 Game 模式, 通常为了方便都使用 Editor 模式, 这会编译生成 Editor 版本的 exe, 然后加载它，这会打开新的 Editor

   这时候调试器其实已经工作了，只需要在自己的程序上打断点，然后运行游戏，就可以触发断点。停止游戏，继续开发时，不要关闭调试，那样会关闭整个 Editor, 而是选择继续运行即可

   通常引擎不包含核心部分的调试信息，需要在启动器启动时设置，勾上调试信息，这会下载几十G的调试信息，不过用户一般只需要关注自己游戏逻辑部分调试信息即可，除非研究和修改引擎源码

## 探究

* 构建系统似乎使用了 C# 作为配置语言, 每个模块的 .build.cs 文件是用 C# 编写的模块构建规则

