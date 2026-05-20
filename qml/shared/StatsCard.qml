import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property color cardColor: "#FAFAFA"
    property color textColor: "#212121"
    property color subTextColor: "#666666"

    color: cardColor
    radius: 10
    border.color: "#E0E0E0"
    implicitHeight: col.implicitHeight + 24
    implicitWidth: 320

    ColumnLayout {
        id: col
        anchors.fill: parent
        anchors.margins: 12
        spacing: 6

        Label {
            text: qsTr("Statistics")
            font.bold: true
            font.pixelSize: 16
            color: root.textColor
        }
        GridLayout {
            columns: 2
            columnSpacing: 12
            rowSpacing: 4
            Layout.fillWidth: true

            Label { text: qsTr("Launches:"); color: root.subTextColor }
            Label { text: gameVm.launchCount; color: root.textColor }
            Label { text: qsTr("Games:"); color: root.subTextColor }
            Label { text: gameVm.gamesPlayed; color: root.textColor }
            Label { text: qsTr("Total attempts:"); color: root.subTextColor }
            Label { text: gameVm.totalAttempts; color: root.textColor }
            Label { text: qsTr("Total score:"); color: root.subTextColor }
            Label { text: gameVm.totalScore; color: root.textColor }
            Label { text: qsTr("Best score:"); color: root.subTextColor }
            Label { text: gameVm.bestScore; font.bold: true; color: root.textColor }
        }
        Label {
            text: qsTr("UUID: %1").arg(gameVm.appUuid)
            font.pixelSize: 10
            font.family: "monospace"
            color: root.subTextColor
            wrapMode: Text.WrapAnywhere
            Layout.fillWidth: true
        }
    }
}
