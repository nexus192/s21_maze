import QtQuick
import QtQuick.Controls
import "windows"

ApplicationWindow {
    id: _root
    width: 760
    height: 540
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    visible: true
    color: "#252525"
    title: "s21_Maze"

    property alias stackView: _stackView

    StackView {
        id: _stackView
        anchors.fill: parent
        initialItem: StartWindow {}
    }
}
