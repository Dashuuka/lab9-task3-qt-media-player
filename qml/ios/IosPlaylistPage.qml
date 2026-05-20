import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../shared"

Page {
    background: Rectangle { color: "#FFFFFF" }
    PlatformTheme { id: theme }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 8

        Label {
            text: qsTr("Library")
            font.pixelSize: 28; font.bold: true
            color: theme.iosText
        }

        ListView {
            id: list
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 0
            model: settingsMgr.historyJson()

            delegate: Item {
                width: list.width
                height: 60
                Rectangle {
                    anchors.fill: parent
                    color: ma.pressed ? "#EAEAEA" : "#FFFFFF"
                    Behavior on color { ColorAnimation { duration: 120 } }
                }
                MouseArea { id: ma; anchors.fill: parent }
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    spacing: 12
                    Rectangle {
                        width: 44; height: 44; radius: 6
                        color: modelData.won ? theme.iosTint : "#A0A0A0"
                        Label { anchors.centerIn: parent; text: modelData.secret; color: "white"; font.bold: true }
                    }
                    ColumnLayout {
                        spacing: 2
                        Layout.fillWidth: true
                        Label {
                            text: modelData.won
                                  ? qsTr("Win — %1 attempts").arg(modelData.attempts)
                                  : qsTr("Loss — number was %1").arg(modelData.secret)
                            color: theme.iosText
                            font.pixelSize: 15
                        }
                        Label {
                            text: modelData.ts
                            font.pixelSize: 11
                            color: theme.iosSecondaryText
                        }
                    }
                    Label {
                        text: "+" + modelData.score
                        font.bold: true
                        color: modelData.won ? theme.iosTint : theme.iosSecondaryText
                        Layout.rightMargin: 8
                    }
                }
                Rectangle {
                    height: 1; color: theme.iosTabBorder
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.leftMargin: 60; anchors.bottom: parent.bottom
                }
            }

            Connections {
                target: settingsMgr
                function onSettingsChanged() { list.model = settingsMgr.historyJson() }
            }

            Label {
                anchors.centerIn: parent
                text: qsTr("Nothing here yet")
                color: theme.iosSecondaryText
                visible: list.count === 0
            }
        }

        Button {
            text: qsTr("Clear")
            flat: true
            Layout.alignment: Qt.AlignRight
            onClicked: settingsMgr.resetStats()
            contentItem: Label {
                text: parent.text
                color: theme.iosTint
                font.pixelSize: 16
            }
            background: Rectangle { color: "transparent" }
        }
    }
}
