import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Dialog {
    id: _dialog

    property int minValue: 5
    property int maxValue: 50

    property string acceptText: "Ok"
    property string rejectText: "Cancel"

    property bool rowsError: false
    property bool colsError: false
    property bool isValid: false

    function validate() {
        var r = parseInt(_rowsField.text)
        var c = parseInt(_colsField.text)

        rowsError = isNaN(r) || r < minValue || r > maxValue
        colsError = isNaN(c) || c < minValue || c > maxValue

        isValid = !rowsError && !colsError
    }

    signal acceptClicked(int rows, int cols)

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2
    width: 260
    implicitHeight: _contentRect.implicitHeight + 20

    closePolicy: Popup.NoAutoClose
    background: Item {}

    contentItem: Item {
        Rectangle {
            id: _contentRect
            width: parent.width
            radius: 12
            color: "#404045"

            implicitHeight: _contentColumn.implicitHeight + 20

            Column {
                id: _contentColumn
                width: parent.width
                spacing: 14
                anchors.margins: 10
                anchors.horizontalCenter: parent.horizontalCenter

                Item {
                    height: 4
                }

                Label {
                    width: parent.width
                    text: "Select rows and columns"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18
                    color: "#FFFFFF"
                }

                Column {
                    x: 12
                    width: parent.width - 24
                    spacing: 4

                    Rectangle {
                        width: parent.width
                        height: 32
                        radius: 6
                        border.width: 1
                        border.color: rowsError ? "#FF4D4D" : "#707070"
                        color: "#2E2E33"

                        TextField {
                            id: _rowsField
                            anchors.fill: parent
                            anchors.margins: 6

                            background: null
                            color: "#FFFFFF"

                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator {
                                bottom: minValue
                                top: maxValue
                            }

                            placeholderText: "Rows (5–50)"
                        }
                    }

                    Label {
                        visible: rowsError
                        text: "Enter number from 5 to 50"
                        color: "#FF4D4D"
                        font.pixelSize: 10
                    }
                }

                Column {
                    x: 12
                    width: parent.width - 24
                    spacing: 4

                    Rectangle {
                        width: parent.width
                        height: 32
                        radius: 6
                        border.width: 1
                        border.color: rowsError ? "#FF4D4D" : "#707070"
                        color: "#2E2E33"

                        TextField {
                            id: _colsField
                            anchors.fill: parent
                            anchors.margins: 6

                            background: null
                            color: "#FFFFFF"

                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator {
                                bottom: minValue
                                top: maxValue
                            }
                            placeholderText: "Columns (5–50)"
                        }
                    }

                    Label {
                        visible: colsError
                        text: "Enter number from 5 to 50"
                        color: "#FF4D4D"
                        font.pixelSize: 10
                    }
                }

                Row {
                    spacing: 40
                    anchors.horizontalCenter: parent.horizontalCenter

                    TextButton {
                        height: 18
                        pixelSize: 12
                        enabledColor: "#64CC67"
                        pressedColor: "#31AC37"
                        disabledColor: "#414040"
                        enabledTextColor: "#FFFFFF"
                        pressedTextColor: "#FFFFFF"
                        disabledTextColor: "#717177"
                        text: _dialog.acceptText

                        onClicked: function () {
                            validate()

                            if (!isValid)
                                return

                            _dialog.acceptClicked(parseInt(_rowsField.text),
                                                  parseInt(_colsField.text))
                        }
                    }

                    TextButton {
                        height: 18
                        pixelSize: 12
                        enabledColor: "transparent"
                        pressedColor: "transparent"
                        disabledColor: "transparent"
                        enabledTextColor: "#FFFFFF"
                        pressedTextColor: "#FFFFFF"
                        disabledTextColor: "#404041"
                        text: _dialog.rejectText

                        onClicked: reject()
                    }
                }

                Item {
                    height: 4
                }
            }
        }
    }
}
