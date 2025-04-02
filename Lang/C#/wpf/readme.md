

## 创建 net472 项目

由于 Windows 后续版本不再包含 .NET 运行时，`.NET Framework` 也只更新到了 4.8.1

如果只想实现小工具，要求打包体积最小，就只能使用 net472 版本运行时

dotnet 无法直接创建 net472 版本的 wpf, 可先创建 .net core3.0 的版本，然后手动更改成 net472

```sh
dotnet new wpf -n MyWpfApp -f netcoreapp3.0
```

修改 `.csproj` 文件 `TargetFramework` 为 `net472`

