import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: startWindow

    FileDialog {
        id: fileDialog
        title: "Select maze file"
        nameFilters: ["Maze files (*.txt)", "All files (*)"]
        onAccepted: {
            mazeParser.loadMazeAsync(selectedFile, mazeModel)
        }
    }

    Connections {
        target: mazeParser
        
        function onLoadingFinished(success, errorMsg) {
            if (success) {
                stackView.push("MazeWindow.qml")
            } else {
                errorDialog.text = errorMsg
                errorDialog.open()
            }
        }
    }

    Dialog {
        id: errorDialog
        title: "Error loading maze"
        property alias text: errorLabel.text
        standardButtons: Dialog.Ok
        anchors.centerIn: parent
        
        Label {
            id: errorLabel
        }
    }

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
            onClicked: fileDialog.open()
        }
    }
}
