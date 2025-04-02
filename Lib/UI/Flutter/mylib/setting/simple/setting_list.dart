import 'package:flutter/material.dart';
import 'setting_item.dart';

class SettingList extends StatefulWidget {
  const SettingList({
    super.key,
    this.title = '',
    this.hideLeading = false,
    required this.items,
  });

  final String title;
  final List<SettingItem> items;
  final bool hideLeading;

  @override
  State<SettingList> createState() => _SettingListState();
}

class _SettingListState extends State<SettingList> {
  int currentExpandedItemIndex = 0;

  @override
  Widget build(BuildContext context) {
    return Navigator(
      onGenerateRoute: (settings) {
        return MaterialPageRoute(builder: (context) => _buildItemList());
      },
    );
  }

  Widget? getLeading(BuildContext context) {
    if (widget.hideLeading) {
      return null;
    }

    return IconButton(
      icon: Icon(Icons.arrow_back),
      onPressed: () {
        Navigator.pop(context);
      },
    );
  }

  Widget _buildItemList() {
    return Scaffold(
      appBar: AppBar(title: Text(widget.title), leading: getLeading(context)),
      body: ListView.builder(
        itemBuilder: (context, index) {
          return widget.items[index];
        },
        itemCount: widget.items.length,
      ),
    );
  }
}
