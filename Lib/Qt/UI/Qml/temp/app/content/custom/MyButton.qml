import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import Qt.labs.platform 1.1

Button {
    id: btn1
    height: 50
    width: 80
    text: "普通按钮"

    background: Rectangle {
        id: buttonBackground
        color: "pink"
        radius: 5
        border.width: 0
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            buttonBackground.color ="#5FB0F3"// 鼠标悬停时的背景颜色
        }

        onExited: {
            buttonBackground.color = "pink"  // 恢复正常的背景颜色

        }

        onClicked: {
            console.log("Clicked")
        }

        onPressed: {
            buttonBackground.color = "red",
            parent.opacity += 0.5
            parent.height -= 10
            parent.width -= 10
        }

        onReleased: {
            parent.opacity -= 0.5
            parent.height += 10
            parent.width += 10
        }
    }
}
