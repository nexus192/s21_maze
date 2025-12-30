import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Dialog {
    id: _dialog

    property bool withReject: false
    property string acceptText: "Ok"
    property string rejectText: "Cancel"
    property string massegeText

    signal acceptClicked
    signal rejectClicked

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2
    width: 200
    height: _content.height

    closePolicy: Popup.NoAutoClose

    background: Item {}

    contentItem: Item {
        Rectangle {
            width: parent.width
            height: _content.height + 20
            radius: 12
            color: "#404045"
            Column {
                id: _content
                width: parent.width
                spacing: 20

                Item {
                    width: parent.width
                    height: 1
                }

                Label {
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 24
                    wrapMode: Text.WordWrap
                    color: "#FFFFFF"
                    text: _dialog.massegeText
                }

                Row {
                    x: parent.width / 2 - width / 2
                    spacing: 50
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
                        onClicked: () => _dialog.acceptClicked()
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
                        visible: _dialog.withReject
                        text: _dialog.rejectText
                        onClicked: () => _dialog.rejectClicked()
                    }
                }
            }
        }
    }
}
