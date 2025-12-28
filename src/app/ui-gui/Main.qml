import QtQuick
import QtQuick.Controls
import "windows"

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true
    title: "Maze Generator"

    property alias stackView: _stackView

    StackView {
        id: _stackView
        anchors.fill: parent
        initialItem: StartWindow {}
    }
}
