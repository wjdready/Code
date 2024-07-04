
import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import Qt.labs.platform 1.1

// 自定义状态栏组件  
Rectangle {  
    id: customStatusBar  
    width: parent.width  
    height: 30  
    color: "#333333"  

    // 在状态栏中添加文本  
    Text {  
        id: statusText  
        text: "自定义状态栏"  
        anchors.centerIn: parent  
        color: "white"  
    }

    // 可以添加更多的自定义元素，比如按钮  
    Button {
        id: statusButton  
        text: "点击我"
        height: 25
        anchors.right: parent.right  
        anchors.rightMargin: 10  
        anchors.verticalCenter: parent.verticalCenter  
        onClicked: {  
            statusText.text = "按钮被点击了"  
        }  
    }  
}
