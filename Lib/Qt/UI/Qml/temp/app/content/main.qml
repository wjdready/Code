import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import Qt.labs.platform 1.1

Window {
    visible: true
    width: 640
    height: 480

    StatusBar {

    }

    // Image {
    //     source: "image/emoji_celebrate.png"
    // }

    MyButton {
        anchors.centerIn: parent
    }
}
