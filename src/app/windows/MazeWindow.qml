import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import "../utils"

Item {
    id: mazeWindow

    property int startRow: -1
    property int startCol: -1
    property int endRow: -1
    property int endCol: -1
    property bool selectingStart: false
    property bool selectingEnd: false

    Rectangle {
        id: _buttenBlock
        x: 20
        y: parent.height / 2 - height / 2
        width: 200
        height: 500
        radius: 12
        color: "#1D1D1D"

        Column {
            x: parent.width / 2 - width / 2
            y: parent.height / 2 - height / 2
            spacing: 24

            TextButton {
                height: 32
                enabledColor: "#414040"
                pressedColor: "#414040"
                disabledColor: "#414040"
                enabledTextColor: "#FFFFFF"
                pressedTextColor: "#FFFFFF"
                disabledTextColor: "#717177"
                text: "Back"
                onClicked: {
                    mazeSolver.clearPath()
                    stackView.pop()
                }
            }

            TextButton {
                height: 32
                enabledColor: "#414040"
                pressedColor: "#414040"
                disabledColor: "#414040"
                enabledTextColor: "#FFFFFF"
                pressedTextColor: "#FFFFFF"
                disabledTextColor: "#717177"
                text: "Save"
                onClicked: _saveDialog.open()
            }

            TextButton {
                height: 32
                enabledColor: "#414040"
                pressedColor: "#414040"
                disabledColor: "#414040"
                enabledTextColor: "#FFFFFF"
                pressedTextColor: "#FFFFFF"
                disabledTextColor: "#717177"
                text: selectingStart ? "Click start..." : "Set Start"
                onClicked: {
                    selectingStart = !selectingStart
                    selectingEnd = false
                }
            }

            TextButton {
                height: 32
                enabledColor: "#414040"
                pressedColor: "#414040"
                disabledColor: "#414040"
                enabledTextColor: "#FFFFFF"
                pressedTextColor: "#FFFFFF"
                disabledTextColor: "#717177"
                text: selectingEnd ? "Click end..." : "Set End"
                onClicked: {
                    selectingEnd = !selectingEnd
                    selectingStart = false
                }
            }

            TextButton {
                height: 32
                enabledColor: "#414040"
                pressedColor: "#414040"
                disabledColor: "#414040"
                enabledTextColor: "#FFFFFF"
                pressedTextColor: "#FFFFFF"
                disabledTextColor: "#717177"
                text: "Clear"
                onClicked: {
                    startRow = -1
                    startCol = -1
                    endRow = -1
                    endCol = -1
                    mazeSolver.clearPath()
                }
            }
        }
    }

    MazeWidget {
        x: _buttenBlock.width + 40
        y: 20
    }

    FileDialog {
        id: _saveDialog
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

    Label {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        text: {
            if (selectingStart)
                return "Click a cell to set START point"
            if (selectingEnd)
                return "Click a cell to set END point"
            if (mazeSolver.hasSolution)
                return "Path found: " + mazeSolver.path.length + " cells"
            if (startRow >= 0 && endRow >= 0)
                return "No path exists!"
            return ""
        }
        color: mazeSolver.hasSolution ? "green" : (startRow >= 0
                                                   && endRow >= 0 ? "red" : "gray")
    }
}
