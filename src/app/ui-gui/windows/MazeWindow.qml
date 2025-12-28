import QtQuick
import QtQuick.Controls

Item {
    id: mazeWindow

    Rectangle {
        id: mazeField
        width: 500
        height: 500
        anchors.centerIn: parent
        color: "white"
        border.color: "black"
        border.width: 2

        property real cellWidth: (width - 4) / mazeModel.cols   // account for outer border
        property real cellHeight: (height - 4) / mazeModel.rows

        Repeater {
            model: mazeModel

            delegate: Item {
                id: cellDelegate
                
                property int row: Math.floor(index / mazeModel.cols)
                property int col: index % mazeModel.cols

                x: 2 + col * mazeField.cellWidth
                y: 2 + row * mazeField.cellHeight
                width: mazeField.cellWidth
                height: mazeField.cellHeight

                // right wall
                Rectangle {
                    visible: rightWall
                    width: 2
                    height: parent.height + 2
                    anchors.right: parent.right
                    anchors.top: parent.top
                    color: "black"
                }

                // bottom wall
                Rectangle {
                    visible: bottomWall
                    height: 2
                    width: parent.width + 2
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    color: "black"
                }
            }
        }
    }

    Button {
        text: "Back"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        onClicked: stackView.pop()
    }
}
