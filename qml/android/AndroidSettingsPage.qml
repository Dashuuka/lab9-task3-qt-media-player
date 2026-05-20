import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../shared"

Page {
    background: Rectangle { color: "#FAFAFA" }
    PlatformTheme { id: theme }

    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: col.implicitHeight
        clip: true

        ColumnLayout {
            id: col
            width: parent.width
            anchors.margins: 16
            spacing: 16

            Item { Layout.preferredHeight: 16 }

            Label {
                text: qsTr("Settings")
                font.pixelSize: 24; font.bold: true
                Layout.leftMargin: 16
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.leftMargin: 16; Layout.rightMargin: 16
                Layout.preferredHeight: langCol.implicitHeight + 24
                radius: theme.androidRadius
                color: "#FFFFFF"
                border.color: "#E0E0E0"
                ColumnLayout {
                    id: langCol
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8
                    Label { text: qsTr("Language"); font.bold: true }
                    LanguageMenu { Layout.fillWidth: true }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.leftMargin: 16; Layout.rightMargin: 16
                Layout.preferredHeight: attemptCol.implicitHeight + 24
                radius: theme.androidRadius
                color: "#FFFFFF"
                border.color: "#E0E0E0"
                ColumnLayout {
                    id: attemptCol
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8
                    Label { text: qsTr("Max attempts"); font.bold: true }
                    NumberInput { Layout.fillWidth: true }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.leftMargin: 16; Layout.rightMargin: 16
                Layout.preferredHeight: infoCol.implicitHeight + 24
                radius: theme.androidRadius
                color: "#FFFFFF"
                border.color: "#E0E0E0"
                ColumnLayout {
                    id: infoCol
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8
                    Label { text: qsTr("Application info"); font.bold: true }
                    Label {
                        text: settingsMgr.formattedSettings()
                        font.pixelSize: 12
                        font.family: "monospace"
                        wrapMode: Text.WrapAnywhere
                        Layout.fillWidth: true
                    }
                }
            }

            Item { Layout.preferredHeight: 24 }
        }
    }
}
