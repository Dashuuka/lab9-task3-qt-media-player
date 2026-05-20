import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../shared"

Page {
    background: Rectangle { color: "#FFFFFF" }
    PlatformTheme { id: theme }

    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: col.implicitHeight + 32
        clip: true

        ColumnLayout {
            id: col
            width: parent.width
            spacing: 24
            anchors.margins: 16

            Item { Layout.preferredHeight: 8 }

            // Section: Language
            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 16; Layout.rightMargin: 16
                spacing: 8

                Label {
                    text: qsTr("LANGUAGE").toUpperCase()
                    font.pixelSize: 12
                    color: theme.iosSecondaryText
                }
                Rectangle {
                    Layout.fillWidth: true
                    radius: theme.iosRadius
                    color: "#F8F8F8"
                    border.color: theme.iosTabBorder
                    height: 50
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        Label {
                            text: qsTr("App language")
                            color: theme.iosText
                            Layout.fillWidth: true
                        }
                        LanguageMenu { Layout.preferredWidth: 160 }
                    }
                }
            }

            // Section: Game
            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 16; Layout.rightMargin: 16
                spacing: 8
                Label {
                    text: qsTr("GAME").toUpperCase()
                    font.pixelSize: 12
                    color: theme.iosSecondaryText
                }
                Rectangle {
                    Layout.fillWidth: true
                    radius: theme.iosRadius
                    color: "#F8F8F8"
                    border.color: theme.iosTabBorder
                    height: 50
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        Label {
                            text: qsTr("Max attempts")
                            color: theme.iosText
                            Layout.fillWidth: true
                        }
                        NumberInput { Layout.preferredWidth: 140 }
                    }
                }
            }

            // Section: Info
            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 16; Layout.rightMargin: 16
                spacing: 8
                Label {
                    text: qsTr("INFO").toUpperCase()
                    font.pixelSize: 12
                    color: theme.iosSecondaryText
                }
                Rectangle {
                    Layout.fillWidth: true
                    radius: theme.iosRadius
                    color: "#F8F8F8"
                    border.color: theme.iosTabBorder
                    Layout.preferredHeight: infoLabel.implicitHeight + 16
                    Label {
                        id: infoLabel
                        anchors.fill: parent
                        anchors.margins: 8
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: 12
                        font.family: "monospace"
                        text: settingsMgr.formattedSettings()
                    }
                }
            }

            Item { Layout.preferredHeight: 16 }
        }
    }
}
