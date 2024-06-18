


利用 path 绘图

```c
QPen pen;
QPainterPath path;

pen.setWidth(3);
pen.setColor(QColor(0xc1cef2));
painter.setPen(pen);

path.moveTo(0, 20);
path.lineTo(400, 20);
path.lineTo(400+20, 20+15);
path.lineTo(400+20+80, 20+15);
path.lineTo(400+20+80+20, 20+15+20);
path.lineTo(400+20+80+20+300, 20+15+20);
painter.drawPath(path);

// 定义圆角矩形的参数
int x = 50;         // 左上角 x 坐标
int y = 20 - 8;     // 左上角 y 坐标
int width = 80;     // 矩形宽度
int height = 50;    // 矩形高度
int radius = 8;     // 圆角半径
path.clear();
path.moveTo(x, y + radius);
path.arcTo(x, y, radius * 2, radius * 2, 150, -60);
path.lineTo(x + width - radius, y);
path.arcTo(x + width - radius * 2, y, radius * 2, radius * 2, 60, -60);
painter.fillPath(path, QBrush(QColor(0xc1cef2)));
```
