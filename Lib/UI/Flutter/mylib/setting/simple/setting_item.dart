import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class SettingItem extends StatelessWidget {
  const SettingItem({
    super.key,
    required this.title,
    this.value = '',
    this.onPressed,
    this.nextPage,
  });

  final Widget? nextPage;
  final String title;
  final String value;
  final Function(BuildContext context)? onPressed;

  void _onPressed(BuildContext context) {
    if (nextPage != null) {
      Navigator.push(
        context,
        CupertinoPageRoute(builder: (context) => nextPage!),
      );
    } else {
      onPressed?.call(context);
    }
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: EdgeInsets.symmetric(horizontal: 16.0, vertical: 2.0),
      child: CupertinoButton(
        sizeStyle: CupertinoButtonSize.medium,
        color: Colors.white,
        onPressed: () => _onPressed(context),
        child: Row(children: [Text(title), Spacer()]),
      ),
    );
  }
}
