import QtQuick 2.15  
import QtQuick.Controls 2.14

Button {  
    id: button  
    height: 60
    width: 120
    background: Rectangle {
        anchors.fill: parent
        color: "#4CAF50"
        radius: 5
        scale: pressed ? 0.95 : 1.0
    }
}
