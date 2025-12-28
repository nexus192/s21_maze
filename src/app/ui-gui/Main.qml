import QtQuick
import QtQuick.Controls
import "windows"

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Maze Generator"

    StackView {
        id: _stackView
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        width: parent.width
        height: parent.height
        initialItem: StartWindow {
            onGenerateClicked: function () {
                mazeModel.generate(20, 20)
                _stackView.push(_maze)
            }
        }
    }

    MazeWindow {
        id: _maze
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
    }
}
