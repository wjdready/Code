
import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

ApplicationWindow  {
    visible: true
    width: 720  
    height: 480

    CButton {
        anchors.centerIn: parent
        text: "按钮"
    }
}
