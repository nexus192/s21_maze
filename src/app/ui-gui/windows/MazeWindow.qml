import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: mazeWindow

    FileDialog {
        id: saveDialog
        title: "Save maze"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Maze files (*.txt)", "All files (*)"]
        onAccepted: {
            mazeParser.saveMazeAsync(selectedFile, mazeModel)
        }
    }

    Connections {
        target: mazeParser
        
        function onSavingFinished(success, errorMsg) {
            if (!success) {
                saveErrorDialog.text = errorMsg
                saveErrorDialog.open()
            }
        }
    }

    Dialog {
        id: saveErrorDialog
        title: "Error saving maze"
        property alias text: saveErrorLabel.text
        standardButtons: Dialog.Ok
        anchors.centerIn: parent
        
        Label {
            id: saveErrorLabel
        }
    }

    Rectangle {
        id: mazeField
        width: 500
        height: 500
        anchors.centerIn: parent
        color: "white"
        border.color: "black"
        border.width: 2

        property real cellWidth: (width - 4) / mazeModel.cols
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

                Rectangle {
                    visible: rightWall
                    width: 2
                    height: parent.height + 2
                    anchors.right: parent.right
                    anchors.top: parent.top
                    color: "black"
                }

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

    Row {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        spacing: 10

        Button {
            text: "Back"
            onClicked: stackView.pop()
        }

        Button {
            text: "Save"
            onClicked: saveDialog.open()
        }
    }
}
