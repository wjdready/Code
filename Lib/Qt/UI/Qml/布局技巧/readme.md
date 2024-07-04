


参考:
https://blog.csdn.net/weixin_45551020/article/details/132503798



```js
// Layout 使用参考: https://stackoverflow.com/questions/30604751/how-to-design-a-multi-level-fluid-layout-in-qml
ColumnLayout {
    spacing: 0
    anchors.fill: parent
    Rectangle {
        implicitHeight: 10
        Layout.fillHeight: true
        Layout.fillWidth: true
        color: "red"
    }
    RowLayout {
        spacing: 0
        Layout.preferredHeight: 40
        Rectangle {
            implicitWidth: 100
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "darkGreen"
        }
        Rectangle {
            implicitWidth: 80
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "lightGreen"
        }
    }
    RowLayout {
        spacing: 0
        Layout.preferredHeight: 40
        Rectangle {
            implicitWidth: 40
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "darkBlue"
        }
        Rectangle {
            implicitWidth: 20
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "blue"
        }
        Rectangle {
            implicitWidth: 40
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "lightBlue"
        }
    }
}
```
