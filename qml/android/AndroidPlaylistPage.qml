import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../shared"

Page {
    background: Rectangle { color: "#FAFAFA" }
    PlatformTheme { id: theme }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: qsTr("Game History")
            font.pixelSize: 22
            font.bold: true
        }

        ListView {
            id: list
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: settingsMgr.historyJson()
            spacing: 6

            delegate: Rectangle {
                width: list.width
                height: 56
                radius: theme.androidRadius
                color: index % 2 === 0 ? "#FFFFFF" : "#F5F5F5"
                border.color: "#E0E0E0"
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 12
                    Rectangle {
                        width: 36; height: 36; radius: 18
                        color: modelData.won ? theme.primary : theme.error
                        Label {
                            anchors.centerIn: parent
                            text: modelData.secret
                            color: "white"
                            font.bold: true
                        }
                    }
                    ColumnLayout {
                        spacing: 2
                        Layout.fillWidth: true
                        Label {
                            text: modelData.won
                                  ? qsTr("Won in %1 attempts").arg(modelData.attempts)
                                  : qsTr("Lost (number was %1)").arg(modelData.secret)
                            font.bold: true
                        }
                        Label {
                            text: modelData.ts
                            font.pixelSize: 11
                            opacity: 0.6
                        }
                    }
                    Label {
                        text: qsTr("+%1").arg(modelData.score)
                        font.bold: true
                        color: modelData.won ? theme.primary : "#9E9E9E"
                    }
                }
            }

            Connections {
                target: settingsMgr
                function onSettingsChanged() { list.model = settingsMgr.historyJson() }
            }

            Label {
                anchors.centerIn: parent
                text: qsTr("No games played yet")
                opacity: 0.5
                visible: list.count === 0
            }
        }

        Button {
            text: qsTr("Clear History")
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            onClicked: settingsMgr.resetStats()
        }
    }
}
