import 'package:flutter/material.dart';

main() {
  runApp(
    MaterialApp(
      home: CounterPage(),
    ),
  );
}

// 1. 创建数据模型
class CounterModel extends ChangeNotifier {
  int _count = 0;
  int get count => _count;

  void increment() {
    _count++;
    notifyListeners();
  }
}

// 2. 在 StatefulWidget 中使用
class CounterPage extends StatefulWidget {
  @override
  _CounterPageState createState() => _CounterPageState();
}

class _CounterPageState extends State<CounterPage> {
  final _counter = CounterModel();

  @override
  void initState() {
    super.initState();
    // 添加监听
    _counter.addListener(_handleCounterChange);
  }

  void _handleCounterChange() {
    // 当计数器变化时，更新UI
    setState(() {});
  }

  @override
  void dispose() {
    // 清理监听器
    _counter.removeListener(_handleCounterChange);
    _counter.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('计数器')),
      body: Center(
        child: Text('当前计数: ${_counter.count}'),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _counter.increment,
        child: Icon(Icons.add),
      ),
    );
  }
}