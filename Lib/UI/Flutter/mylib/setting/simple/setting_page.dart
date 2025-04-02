import 'package:flutter/material.dart';
import 'package:inherited/setting_item.dart';
import 'package:inherited/setting_list.dart';

class SettingPage extends StatefulWidget {
  const SettingPage({super.key});

  @override
  State<SettingPage> createState() => _SettingPageState();
}

class _SettingPageState extends State<SettingPage> {
  int currentIndex = 0;

  bool isHorizontal = false;

  @override
  Widget build(BuildContext context) {
    isHorizontal = false;

    return LayoutBuilder(
      builder: (context, constraints) {
        isHorizontal = constraints.maxWidth > 600 ? true : false;

        items = [
          SettingItem(
            title: '基本设置',
            nextPage: SettingItemPage(hideLeading: isHorizontal),
            onPressed: (_) => setIdx(0),
          ),
          SettingItem(title: '用户设置', onPressed: (_) => setIdx(2)),
          SettingItem(title: '通用设置', onPressed: (_) => setIdx(3)),
          SettingItem(title: '系统设置', onPressed: (_) => setIdx(4)),
        ];

        return isHorizontal ? _buildHorizontal() : _buildVertical();
      },
    );
  }

  void setIdx(int index) => setState(() {
    currentIndex = index;
  });

  List<SettingItem> items = [];

  Widget _buildHorizontal() {
    return Scaffold(
      // appBar: AppBar(title: const Text('设置')),
      body: Center(
        child: Row(
          children: [
            Expanded(
              flex: 1,
              child: Scaffold(
                appBar: AppBar(title: const Text('设置H')),
                body: Column(
                  children: [
                    for (var item in items)
                      SettingItem(title: item.title, onPressed: item.onPressed),
                  ],
                ),
              ),
            ),
            Expanded(
              flex: 3,
              child: items[currentIndex].nextPage ?? Container(),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildVertical() {
    return SettingList(
      title: '设置V',
      items: [
        ...items.map(
          (item) => SettingItem(title: item.title, nextPage: item.nextPage),
        ),
      ],
    );
  }
}

class SettingItemPage extends StatelessWidget {
  const SettingItemPage({super.key, this.hideLeading = false});

  final bool hideLeading;

  @override
  Widget build(BuildContext context) {
    return SettingList(
      title: '网络设置',
      hideLeading: hideLeading,
      items: [
        SettingItem(
          title: '网络设置',
          nextPage: SettingList(
            title: '网络设置',
            items: [
              SettingItem(
                title: 'WIFI设置',
                nextPage: SettingList(
                  title: 'WIFI设置',
                  items: [
                    SettingItem(title: 'WIFI名称'),
                    SettingItem(title: 'WIFI密码'),
                  ],
                ),
              ),
            ],
          ),
        ),
        SettingItem(title: '用户设置'),
        SettingItem(title: '通用设置'),
      ],
    );
  }
}
