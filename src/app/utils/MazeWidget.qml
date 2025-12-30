import QtQuick
import QtQuick.Controls

Rectangle {
    id: _mazeWindow
    width: 500
    height: 500
    color: "white"
    border.color: "black"
    border.width: 2
    radius: 12

    property real cellWidth: (width - 4) / mazeModel.cols
    property real cellHeight: (height - 4) / mazeModel.rows

    Repeater {
        model: mazeModel

        delegate: Item {
            id: _cellDelegate

            property int row: Math.floor(index / mazeModel.cols)
            property int col: index % mazeModel.cols

            x: 2 + col * _mazeWindow.cellWidth
            y: 2 + row * _mazeWindow.cellHeight
            width: _mazeWindow.cellWidth
            height: _mazeWindow.cellHeight

            Rectangle {
                x: parent.width / 2 - width / 2
                y: parent.height / 2 - height / 2
                width: parent.width - 5
                height: parent.height - 5
                radius: width / 2
                color: {
                    if (_cellDelegate.row === startRow
                            && _cellDelegate.col === startCol)
                        return "#90EE90" // light green
                    if (_cellDelegate.row === endRow
                            && _cellDelegate.col === endCol)
                        return "#FFB6C1" // light pink
                    return "transparent"
                }
            }

            Rectangle {
                visible: rightWall && _cellDelegate.col < mazeModel.cols - 1
                width: 2
                height: parent.height
                anchors.right: parent.right
                color: "black"
            }

            Rectangle {
                visible: bottomWall && _cellDelegate.row < mazeModel.rows - 1
                height: 2
                width: parent.width
                anchors.bottom: parent.bottom
                color: "black"
            }
        }
    }

    Canvas {
        id: _pathCanvas
        anchors.fill: parent
        visible: mazeSolver.hasSolution

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            var path = mazeSolver.path
            if (path.length < 2)
                return

            ctx.strokeStyle = "#E74C3C" // red
            ctx.lineWidth = 2
            ctx.lineCap = "round"
            ctx.lineJoin = "round"

            ctx.beginPath()

            var cellW = _mazeWindow.cellWidth
            var cellH = _mazeWindow.cellHeight

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
                _pathCanvas.requestPaint()
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: selectingStart || selectingEnd

        onClicked: function (mouse) {
            var cellW = _mazeWindow.cellWidth
            var cellH = _mazeWindow.cellHeight
            var col = Math.floor((mouse.x - 2) / cellW)
            var row = Math.floor((mouse.y - 2) / cellH)

            if (row < 0 || row >= mazeModel.rows || col < 0
                    || col >= mazeModel.cols)
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
