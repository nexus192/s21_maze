import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import "../utils"

Item {
    id: startWindow

    FileDialog {
        id: _fileDialog
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
                _stackView.push("MazeWindow.qml")
            } else {
                _errorDialog.massegeText = errorMsg
                _errorDialog.open()
            }
        }
    }

    StatusDialog {
        id: _errorDialog
        onAcceptClicked: () => reject()
    }

    SelectRowColDialog {
        id: _selectRowColDialog
        onAcceptClicked: function (rows, cols) {
            mazeModel.generate(rows, cols)
            _selectRowColDialog.reject()
            _stackView.push("MazeWindow.qml")
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Generate maze"
            onClicked: () => _selectRowColDialog.open()
        }

        Button {
            text: "Load from file"
            onClicked: () => _fileDialog.open()
        }
    }
}
