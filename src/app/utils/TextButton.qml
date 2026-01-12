import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Rectangle {
    id: _button

    property string text

    property color enabledColor
    property color pressedColor
    property color disabledColor

    property color enabledTextColor
    property color pressedTextColor
    property color disabledTextColor

    property int pixelSize: 18
    property int spacing: 12
    property int margin: 24

    readonly property real contentWidth: 2 * margin + _itemsRow.width

    signal clicked

    width: contentWidth
    activeFocusOnTab: true
    radius: 12
    color: {
        if (enabled && !_mouseArea.containsPress) {
            return enabledColor
        } else if (enabled && _mouseArea.containsPress) {
            return pressedColor
        } else {
            return disabledColor
        }
    }

    Row {
        id: _itemsRow
        x: _button.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        spacing: _button.spacing

        Label {
            id: _buttonText
            font.pixelSize: _button.pixelSize
            color: {
                if (enabled && !_mouseArea.containsPress) {
                    return _button.enabledTextColor
                } else if (enabled && _mouseArea.containsPress) {
                    return _button.pressedTextColor
                } else {
                    return _button.disabledTextColor
                }
            }
            text: _button.text
        }
    }

    MouseArea {
        id: _mouseArea
        width: parent.width
        height: parent.height
        cursorShape: Qt.PointingHandCursor
        onClicked: () => _button.clicked()
    }
}
