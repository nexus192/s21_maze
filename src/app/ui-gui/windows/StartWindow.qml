import QtQuick
import QtQuick.Controls

Item {
    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Generate maze"
            onClicked: {
                mazeModel.generate(20, 20)
                stackView.push("MazeWindow.qml")
            }
        }

        Button {
            text: "Load from file"
            onClicked: {
                // later
            }
        }
    }
}
