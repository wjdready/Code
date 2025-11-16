


```sh
# 创建一个dart项目:
# dart create dart_custom_cli

# pub 子命令的帮助 (主要有4点)
dart pub global --help 

# 1. 激活全局命令
dart pub global activate --source path .

# 2. 取消激活全局命令
dart pub global deactivate dart_custom_cli

# 3. 列出全局命令
dart pub global list

# 4. 运行全局命令 (运行入口 bin/dart_custom_cli.dart )
dart pub global run dart_custom_cli

# 5. 直接输入命令, 
# 确保全局命令在环境变量中, 对于 windows 是 C:\Users\shino\AppData\Local\Pub\Cache\bin
# 并在 pubspec.yaml 中添加
executables:
  dart_custom_cli:

# 输入参数, 比如解析目录是执行时的目录
# String currentPath = Directory.current.path;
# print('当前工作目录: $currentPath');
```
