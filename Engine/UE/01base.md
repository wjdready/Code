

## 资源系统

游戏中资源路径使用 `/Game` 

资源导入进来会被重新封装成 `.uasset` 包括纹理、材质等

图片导入进来会被封装成纹理


#### 设置默认场景

`编辑->项目设置->地图和模式->默认地图` 选择自己本地地图

对应的配置文件和内容

DefaultEngine.ini

```ini
[/Script/EngineSettings.GameMapsSettings]
EditorStartupMap=/Game/场景/NewWorld.NewWorld
GameDefaultMap=/Game/场景/NewWorld.NewWorld
```

#### 打包发布

`编辑-项目设置->打包` 设置编译配置为 `发行` 

也可以在打包时再设置 `平台->打包项目->打包项目`


## 报错

- Using 'git status' to determine working set for adaptive non-unity build

如果这个项目是git项目， ue编译时会优先通过 git status检查哪些文件被修改，然后只编译这些修改过的文件，而不是整个项目，从而节省编译时间。但是编译系统却不支持中文, git 提交有中文，会导致编译失败

我们需要禁用这个，改用文件修改时间（timestamp）来判断哪些文件需要重新编译

cmd /c code "%APPDATA%\\Unreal Engine\\UnrealBuildTool\\BuildConfiguration.xml"

在 Configuration 中添加:

```xml
<SourceFileWorkingSet> 
    <Provider>None</Provider> 
    <RepositoryPath></RepositoryPath> 
    <GitPath></GitPath> 
</SourceFileWorkingSet>
```

