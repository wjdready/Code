
官方文档[中文](https://docs.flutter.cn/community/china/)

## 入门

到 [flutter 中文网](https://flutter.cn/) 中下载 SDK, 解压后导出 `bin` 路径到环境 

对于 Windows 桌面开发还需依赖 MSVC 环境, 对于 Android 则依赖 ANDROID_SDK_ROOT

```sh
# 创建 flutter 项目
flutter create demo
cd demo

# 运行 windows 端
flutter run -d windows
```

在 lib/main.dart

```dart
import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('中心对齐的按钮'),
        ),
        body: Center(
          child: ElevatedButton(
            onPressed: () {
              debugPrint("按钮按下");
            },
            child: const Text("Button"),
          ),
        ),
      ),
    );
  }
}
```


## 开源 example

https://zhuanlan.zhihu.com/p/654768933


## dart fii 和 C/C++

在 pubspec.yaml 中添加包 `ffi: ^2.0.1` , 在 dart 中引入

```sh
import 'package:ffi/ffi.dart';
import 'dart:ffi';
```

参考:
[和C类型对应的参考](https://dart.dev/interop/c-interop)
[calloc 未定义?](https://stackoverflow.com/questions/74506605/flutter-fii-the-method-calloc-isnt-defined)
[官方的示例](https://github.com/dart-lang/samples/blob/main/ffi/primitives/primitives.dart)
[结构体中包含数组](https://stackoverflow.com/questions/70782570/dart-flutter-ffi-convert-list-to-array)

## 一些问题

触摸滚动支持
https://github.com/flutter/flutter/issues/90366

电脑端全屏显示
https://github.com/leanflutter/window_manager
