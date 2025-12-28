import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: mazeWindow

    property int startRow: -1
    property int startCol: -1
    property int endRow: -1
    property int endCol: -1
    property bool selectingStart: false
    property bool selectingEnd: false

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

                // cell background for start/end markers
                Rectangle {
                    anchors.fill: parent
                    color: {
                        if (cellDelegate.row === startRow && cellDelegate.col === startCol)
                            return "#90EE90"  // light green
                        if (cellDelegate.row === endRow && cellDelegate.col === endCol)
                            return "#FFB6C1"  // light pink
                        return "transparent"
                    }
                }

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

        // solution path overlay
        Canvas {
            id: pathCanvas
            anchors.fill: parent
            visible: mazeSolver.hasSolution

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                
                var path = mazeSolver.path
                if (path.length < 2) return

                ctx.strokeStyle = "#E74C3C"  // red
                ctx.lineWidth = 2
                ctx.lineCap = "round"
                ctx.lineJoin = "round"

                ctx.beginPath()
                
                var cellW = mazeField.cellWidth
                var cellH = mazeField.cellHeight
                
                // offset for border
                var offsetX = 2
                var offsetY = 2

                for (var i = 0; i < path.length; i++) {
                    var centerX = offsetX + path[i].col * cellW + cellW / 2
                    var centerY = offsetY + path[i].row * cellH + cellH / 2
                    
                    if (i === 0) {
                        ctx.moveTo(centerX, centerY)
                    } else {
                        ctx.lineTo(centerX, centerY)
                    }
                }
                
                ctx.stroke()
            }

            Connections {
                target: mazeSolver
                function onPathChanged() {
                    pathCanvas.requestPaint()
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: selectingStart || selectingEnd

            onClicked: function (mouse) {
                var cellW = mazeField.cellWidth
                var cellH = mazeField.cellHeight
                var col = Math.floor((mouse.x - 2) / cellW)
                var row = Math.floor((mouse.y - 2) / cellH)

                if (row < 0 || row >= mazeModel.rows || col < 0 || col >= mazeModel.cols)
                    return

                if (selectingStart) {
                    startRow = row
                    startCol = col
                    selectingStart = false
                    
                    // re-solve if end already set
                    if (endRow >= 0) {
                        mazeSolver.solveMaze(startRow, startCol, endRow, endCol)
                    }
                } else if (selectingEnd) {
                    endRow = row
                    endCol = col
                    selectingEnd = false
                    
                    if (startRow >= 0) {
                        mazeSolver.solveMaze(startRow, startCol, endRow, endCol)
                    }
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
            onClicked: {
                mazeSolver.clearPath()
                stackView.pop()
            }
        }

        Button {
            text: "Save"
            onClicked: saveDialog.open()
        }

        Button {
            text: selectingStart ? "Click start..." : "Set Start"
            highlighted: selectingStart
            onClicked: {
                selectingStart = !selectingStart
                selectingEnd = false
            }
        }

        Button {
            text: selectingEnd ? "Click end..." : "Set End"
            highlighted: selectingEnd
            onClicked: {
                selectingEnd = !selectingEnd
                selectingStart = false
            }
        }

        Button {
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

    // status label
    Label {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        text: {
            if (selectingStart) return "Click a cell to set START point"
            if (selectingEnd) return "Click a cell to set END point"
            if (mazeSolver.hasSolution) return "Path found: " + mazeSolver.path.length + " cells"
            if (startRow >= 0 && endRow >= 0) return "No path exists!"
            return ""
        }
        color: mazeSolver.hasSolution ? "green" : (startRow >= 0 && endRow >= 0 ? "red" : "gray")
    }
}
