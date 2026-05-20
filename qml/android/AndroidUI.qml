import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../shared"

// Material 3 Android UI with Drawer for navigation +
// bottom navigation bar of large touch targets.
Page {
    id: root
    Material.theme: Material.Light
    Material.primary: Material.DeepPurple
    Material.accent: Material.Teal

    PlatformTheme { id: theme }

    background: Rectangle { color: theme.androidBg }

    header: ToolBar {
        Material.elevation: 4
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 8
            spacing: 8
            ToolButton {
                text: "☰"
                font.pixelSize: 22
                onClicked: drawer.open()
            }
            Label {
                text: qsTr("Guess the Number")
                font.pixelSize: 18; font.bold: true
                Layout.fillWidth: true
                elide: Label.ElideRight
                color: "white"
            }
            LanguageMenu { Layout.preferredWidth: 130 }
        }
    }

    footer: TabBar {
        id: bottomNav
        Material.elevation: 4
        TabButton {
            text: qsTr("Play")
            icon.source: "qrc:/qt/qml/GuessNumberX/icons/home.svg"
            font.pixelSize: 14
            height: 64
        }
        TabButton {
            text: qsTr("History")
            icon.source: "qrc:/qt/qml/GuessNumberX/icons/playlist.svg"
            font.pixelSize: 14
            height: 64
        }
        TabButton {
            text: qsTr("Settings")
            icon.source: "qrc:/qt/qml/GuessNumberX/icons/settings.svg"
            font.pixelSize: 14
            height: 64
        }
    }

    Drawer {
        id: drawer
        edge: Qt.LeftEdge
        width: Math.min(root.width * 0.78, 320)
        height: root.height

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12
            Rectangle {
                color: theme.androidPrimary
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                radius: theme.androidRadius
                Column {
                    anchors.centerIn: parent
                    spacing: 4
                    Label { text: qsTr("Guess the Number"); color: "white"; font.bold: true; font.pixelSize: 20 }
                    Label { text: qsTr("Best score: %1").arg(gameVm.bestScore); color: "white"; opacity: 0.85 }
                }
            }
            ItemDelegate {
                Layout.fillWidth: true; height: 56
                text: qsTr("Play")
                icon.source: "qrc:/qt/qml/GuessNumberX/icons/home.svg"
                onClicked: { stack.currentIndex = 0; bottomNav.currentIndex = 0; drawer.close() }
            }
            ItemDelegate {
                Layout.fillWidth: true; height: 56
                text: qsTr("History")
                icon.source: "qrc:/qt/qml/GuessNumberX/icons/playlist.svg"
                onClicked: { stack.currentIndex = 1; bottomNav.currentIndex = 1; drawer.close() }
            }
            ItemDelegate {
                Layout.fillWidth: true; height: 56
                text: qsTr("Settings")
                icon.source: "qrc:/qt/qml/GuessNumberX/icons/settings.svg"
                onClicked: { stack.currentIndex = 2; bottomNav.currentIndex = 2; drawer.close() }
            }
            Item { Layout.fillHeight: true }
            Label { text: qsTr("v1.0 — Android"); opacity: 0.5 }
        }
    }

    StackLayout {
        id: stack
        anchors.fill: parent
        currentIndex: bottomNav.currentIndex

        AndroidGamePage { id: gamePage }
        AndroidPlaylistPage { id: histPage }
        AndroidSettingsPage { id: settingsPage }
    }

    // Snackbar-style error feedback
    Rectangle {
        id: snack
        property string text: ""
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter
        width: Math.min(parent.width - 32, 480)
        height: 48
        radius: 8
        color: "#323232"
        opacity: 0
        Label {
            anchors.centerIn: parent
            color: "white"
            text: snack.text
        }
        Behavior on opacity { NumberAnimation { duration: 200 } }
    }
    Timer { id: snackTimer; interval: 2500; onTriggered: snack.opacity = 0 }

    Connections {
        target: gameVm
        function onInputError(msg) {
            snack.text = msg
            snack.opacity = 0.95
            snackTimer.restart()
        }
    }
}
