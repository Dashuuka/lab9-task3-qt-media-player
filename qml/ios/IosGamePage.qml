import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../shared"

Page {
    background: Rectangle { color: "#FFFFFF" }
    PlatformTheme { id: theme }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        // Apple Music-style "Now Playing" art card
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: Math.min(parent.width - 24, 320)
            Layout.preferredHeight: width
            radius: theme.iosRadius + 4
            color: "#F9F9F9"
            border.color: theme.iosTabBorder
            Image {
                anchors.centerIn: parent
                source: gameVm.imageSource
                width: parent.width * 0.6
                height: width
                fillMode: Image.PreserveAspectFit
            }
            Label {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter
                text: gameVm.statusText
                font.pixelSize: 16; font.bold: true
                color: theme.iosText
            }
        }

        GameCore {
            id: core
            Layout.fillWidth: true
            Layout.fillHeight: true
            bgColor: theme.iosBg
            cardColor: "#F2F2F7"
            textColor: theme.iosText
            accentColor: theme.iosTint
            accentPressed: "#C70039"
            errorColor: theme.error
            controlHeight: 44
            controlRadius: theme.iosRadius
            flatStyle: true
            showInlineDialog: true
        }
    }
}
