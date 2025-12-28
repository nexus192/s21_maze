import QtQuick

Item {
    Rectangle {
        anchors.fill: parent
        color: "white"

        Repeater {
            model: mazeModel

            delegate: Item {
                width: 20
                height: 20
                x: index % mazeModel.cols * width
                y: Math.floor(index / mazeModel.cols) * height

                Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                    border.color: "black"
                }

                Rectangle {
                    visible: rightWall
                    width: 2
                    height: parent.height
                    anchors.right: parent.right
                    color: "black"
                }

                Rectangle {
                    visible: bottomWall
                    height: 2
                    width: parent.width
                    anchors.bottom: parent.bottom
                    color: "black"
                }
            }
        }
    }
}
