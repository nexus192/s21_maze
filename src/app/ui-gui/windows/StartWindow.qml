import QtQuick
import QtQuick.Controls

Item {
    id: _window
    signal generateClicked
    signal loadClicked

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Generate maze"
            onClicked: () => _window.generateClicked()
        }

        Button {
            text: "Load from file"
            onClicked: {

                // позже
            }
        }
    }
}
