

## Hello World

从 UnityHub 中创建空白工程. 往场景放置一个 2D Sprite, 然后创建一个脚本

```c#
public class Move : MonoBehaviour
{
    void Start()
    {
        print("Hello World");
    }

    void Update()
    {
        // 移动位置
        Vector2 pos = transform.position;
        pos.x += 0.1f;
        transform.position = pos;
    }
}
```

点击 `2D Sprite` 对象，在对象属性栏里面，Add Component 里面添加脚本，或者直接将脚本拖过去。


## 工程目录


| 文件夹名称      | 描述                                                                                                           |
| --------------- | -------------------------------------------------------------------------------------------------------------- |
| Assets/         | 存储项目的所有资源文件（如脚本、场景、材质、预制件等），这是项目的核心内容，必须添加到版本控制中               |
| Library/        | 存储 Unity 的本地缓存数据，包括导入的资源、预编译的脚本等，这是 Unity 自动生成的缓存文件夹，通常不需要版本控制 |
| Logs/           | 存储 Unity 的日志文件，这是运行时生成的文件，与项目无关                                                        |
| Obj/            | 存储编译过程中生成的临时文件，通常不需要版本控制                                                               |
| Packages        | 包含 Unity 包管理器（UPM）相关的文件，例如 manifest.json 和 packages-lock.json，用于定义和锁定项目依赖的包。   |
|                 | 应该添加到版本控制中，以确保团队成员使用相同的依赖包版本。                                                     |
| ProjectSettings | 存储项目的全局设置（如图形设置、输入设置、构建设置等）。这些文件定义了项目的配置，应该添加到版本控制中。       |
| UserSettings    | 存储用户特定的设置，例如 IDE 偏好、调试配置等。这些是用户本地的设置，与项目本身无关。可根据情况添加到版本管理  |
| *.csproj        | 这是 Unity 自动生成的 C# 项目文件，主要用于 IDE（如 Visual Studio 或 Rider）进行代码编辑和调试。不需要版本控制 |
| *.sln           | Unity 自动生成的解决方案文件，用于 IDE 打开整个 Unity 项目。不需要版本控制                                     |


## 编辑器选择 VSCode

安装插件: Unity，它会自动安装依赖的插件: `C# Dev Kit`，`C#`，`.NET Install Tool` 开箱即用

只需在 Unity 中配置默认的编辑器为 VSCode 即可

在 `.vscode/settings.json` 中去掉 `files.exclude` 就可以看到全部文件

#### 单步调试

在 Debug 栏，点击 `Attach to Unity`，设置断点，然后在 Unity 中启动游戏即可


