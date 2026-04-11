

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



