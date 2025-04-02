import 'package:flutter/material.dart';

void main(List<String> args) {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(title: 'Demo', home: HomePage());
  }
}

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {

  int counter = 12;

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      body: ShareData(
        counter: counter,
        child: Column(
          children: [
            DependentChild(),
            // 使用 const 时，setState 方法不会触发 build
            // 而 updateShouldNotify 又返回 false, 所以不会触发更新
            const DependentChild(),
          ],
        ),
      ),

      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.add),
        onPressed: () {
          setState(() {
            counter++;
          });
        },
      ),
    );
  }
}

class DependentChild extends StatelessWidget {
  const DependentChild({super.key});

  @override
  Widget build(BuildContext context) {
    // 通过当前孩子 Widget 的 contex 调用 dependOnInheritedWidgetOfExactType<T>
    // 该方法会向上查找属于 T 类型的父亲, 从而找到 ShareData 实例, 并获取里面的数据
    final data = ShareData.of(context);
    return Text('计数\n${data?.counter}');
  }
}


/* InheritedWidget 可用于在 widget 树中共享数据的场景 */
class ShareData extends InheritedWidget {
  final int counter;
  const ShareData({super.key, required super.child, this.counter = 123});

  // 便捷接口, 孩子可以通过自身的 contex 调用 dependOnInheritedWidgetOfExactType<T>
  // 从而找到父亲 ShareData 实例, 并获取里面的数据
  static ShareData? of(BuildContext context) {
    return context.dependOnInheritedWidgetOfExactType<ShareData>();
  }

  @override
  bool updateShouldNotify(ShareData oldWidget) {
    print('updateShouldNotify ${oldWidget.counter} ${counter}');
    // const 修饰的 Widget setState 后不会触发 build, 但会触发 updateShouldNotify
    // 而 updateShouldNotify 又返回 false, 所以不会触发更新
    return false;
  }
}
