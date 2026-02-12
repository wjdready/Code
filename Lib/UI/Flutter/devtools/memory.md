Flutter 内存管理
https://medium.com/@fluttergems/mastering-dart-flutter-devtools-memory-view-part-7-of-8-e7f5aaf07e15

创建dart对象, 当不再有任何地方引用时, 会被gc自动回收. 然而这仅限于 dart 对象, 如果是通过 malloc.allocate 分配的内存, 不会被gc自动回收. 需要自行 通过 malloc.free 释放内存.
例如
```dart
import 'dart:ffi';

// ptr 用完后如果不执行 malloc.free(ptr), 会导致内存泄漏
final Pointer<Uint8> ptr = malloc.allocate(1024);

// dartData 如果没有任何变量引用, 则会自动被 GC 清理
final Uint8List dartData = Uint8List(1024);

// 还有一种情况, asTypedList 仅仅只是创建了一个视图, 不会分配新的内存, 当 dartData2 不被引用时 GC 只会清理 Uint8List 占用的内存
// 不会清理 malloc 分配的内存, 还需注意手动清理掉
final Uint8List dartData2 = ptr.asTypedList(1024);

// 对于从 ptr 返回的 Uint8List, 建议使用 fromList 创建一个克隆, 然后随即 free 掉 malloc 分配的内存
final Uint8List dartManagedList = Uint8List.fromList(ptr.asTypedList(1024));
malloc.free(ptr);

// 还要注意一些内置方法也需要手动释放内存, 比如将 dart 字符串转为 c 字符串时, 会分配新的内存, 需要手动释放
final Pointer<Utf8> cPtr = "Hello".toNativeUtf8();
malloc.free(cPtr);
```

另外 windows 管家清理内存后, 资源管理器回显示内存已经变小, 但其实只是将该应用的内存计数清零, 并重新计算, 应用实际消耗内存并没有实际变小, 
这会导致资源管理器中显示内存大小和应用实际内存不符合, 所以开发时, 不建议使用管家进行清理内存.

Flutter 自带的 DevTools 内存工具可以看到 GC 能够管理的 Dart 内存占用量, 以及应用总的堆内存占用量 RSS, 
通过长期运行应用并观察 GC 管理的内存占用量以及快照中 dart 对象的数量, 可以分析出 dart 部分内存是否有泄漏.

```dart
class MemoryLeakObject {
  final String text;

  MemoryLeakObject(this.text);
}

List<MemoryLeakObject> leakObjects = [];

class MemoryLeaksScreen extends StatelessWidget {
  const MemoryLeaksScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return ElevatedButton(
      child: const Text('Add 1M Leaky Objects'),
      onPressed: () {
        var i = 0;
        while (i < 1000000) {
          leakObjects.add(MemoryLeakObject('Id: ${leakObjects.length}'));
          i++;
        }
      },
    );
  }
}
```

通过观察 RSS 内存占用量, 可以分析出 malloc 部分内存是否有内存泄漏. 但是具体是哪里泄漏比较难排查.
当然这仅仅只适用于泄漏比较快的时候, 如果泄漏小, 很难看出差异.

