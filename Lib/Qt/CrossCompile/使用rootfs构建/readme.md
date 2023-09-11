qmake [手册](https://doc.qt.io/qt-5/qmake-manual.html)

# [2023-09-03 15:29:26]

似乎可以通过更改 QMAKESPEC 更改 qmake -query 中硬编码的问题

qt.conf 似乎是 runtime 的东西, 和 qmake 关系不大, runtime 时读取各库文件位置
