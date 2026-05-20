import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../shared"

// iOS-style: TabBar bottom, flat buttons, Apple Music-pink accent,
// smooth page transitions.
Page {
    id: root
    PlatformTheme { id: theme }
    background: Rectangle { color: theme.iosBg }

    header: ToolBar {
        height: 44
        background: Rectangle {
            color: theme.iosBg
            Rectangle {
                width: parent.width; height: 1
                anchors.bottom: parent.bottom
                color: theme.iosTabBorder
            }
        }
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12; anchors.rightMargin: 12
            Label {
                text: pages.currentIndex === 0
                      ? qsTr("Now Playing")
                      : (pages.currentIndex === 1 ? qsTr("Library") : qsTr("Settings"))
                font.pixelSize: 17
                font.bold: true
                color: theme.iosText
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    footer: TabBar {
        id: tabBar
        height: 60
        background: Rectangle {
            color: theme.iosTabBg
            Rectangle {
                width: parent.width; height: 1
                anchors.top: parent.top
                color: theme.iosTabBorder
            }
        }

        TabButton {
            text: qsTr("Play")
            icon.source: "qrc:/qt/qml/GuessNumberX/icons/home.svg"
            display: AbstractButton.TextUnderIcon
            font.pixelSize: 11
            contentItem: Column {
                spacing: 2
                Image {
                    source: "qrc:/qt/qml/GuessNumberX/icons/home.svg"
                    sourceSize.width: 24; sourceSize.height: 24
                    width: 24; height: 24
                    anchors.horizontalCenter: parent.horizontalCenter
                    opacity: tabBar.currentIndex === 0 ? 1 : 0.5
                }
                Label {
                    text: qsTr("Play")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: tabBar.currentIndex === 0 ? theme.iosTint : theme.iosSecondaryText
                    font.pixelSize: 11
                }
            }
        }
        TabButton {
            text: qsTr("Library")
            contentItem: Column {
                spacing: 2
                Image {
                    source: "qrc:/qt/qml/GuessNumberX/icons/playlist.svg"
                    sourceSize.width: 24; sourceSize.height: 24
                    width: 24; height: 24
                    anchors.horizontalCenter: parent.horizontalCenter
                    opacity: tabBar.currentIndex === 1 ? 1 : 0.5
                }
                Label {
                    text: qsTr("Library")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: tabBar.currentIndex === 1 ? theme.iosTint : theme.iosSecondaryText
                    font.pixelSize: 11
                }
            }
        }
        TabButton {
            text: qsTr("Settings")
            contentItem: Column {
                spacing: 2
                Image {
                    source: "qrc:/qt/qml/GuessNumberX/icons/settings.svg"
                    sourceSize.width: 24; sourceSize.height: 24
                    width: 24; height: 24
                    anchors.horizontalCenter: parent.horizontalCenter
                    opacity: tabBar.currentIndex === 2 ? 1 : 0.5
                }
                Label {
                    text: qsTr("Settings")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: tabBar.currentIndex === 2 ? theme.iosTint : theme.iosSecondaryText
                    font.pixelSize: 11
                }
            }
        }
    }

    StackLayout {
        id: pages
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        IosGamePage {}
        IosPlaylistPage {}
        IosSettingsPage {}
    }

    // Smooth page transitions (cross-fade)
    Behavior on opacity { NumberAnimation { duration: 200 } }
}
