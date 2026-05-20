import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../shared"

// Classic desktop UI: MenuBar + ToolBar + central pane.
// Supports drag-and-drop of files (filename digits seed game).
Page {
    id: root
    PlatformTheme { id: theme }
    background: Rectangle { color: theme.linuxBg }

    // Drop target — covers the central area
    DropArea {
        anchors.fill: parent
        onEntered: function(drag) { dropOverlay.visible = drag.hasUrls; drag.accept(Qt.CopyAction) }
        onExited: dropOverlay.visible = false
        onDropped: function(drop) {
            dropOverlay.visible = false
            if (drop.hasUrls && drop.urls.length > 0) {
                dropHandler.processUrl(drop.urls[0].toString())
            }
        }
    }

    // Global shortcuts (work without focus on the menubar)
    Shortcut { sequence: "Ctrl+N"; onActivated: gameVm.newGame() }
    Shortcut { sequence: "Ctrl+Q"; onActivated: Qt.quit() }
    Shortcut { sequence: "Ctrl+1"; onActivated: tabs.currentIndex = 0 }
    Shortcut { sequence: "Ctrl+2"; onActivated: tabs.currentIndex = 1 }
    Shortcut { sequence: "Ctrl+3"; onActivated: tabs.currentIndex = 2 }
    Shortcut { sequence: "F1";     onActivated: helpDialog.open() }

    Dialog {
        id: helpDialog
        title: qsTr("Keyboard shortcuts")
        anchors.centerIn: parent
        standardButtons: Dialog.Close
        contentItem: ColumnLayout {
            spacing: 6
            Label { text: "Ctrl+N — " + qsTr("new game") }
            Label { text: "Enter — " + qsTr("submit guess") }
            Label { text: "Ctrl+1/2/3 — " + qsTr("switch tabs") }
            Label { text: "Ctrl+Q — " + qsTr("quit") }
            Label { text: "F1 — " + qsTr("this dialog") }
        }
    }

    Dialog {
        id: aboutDialog
        title: qsTr("About")
        anchors.centerIn: parent
        standardButtons: Dialog.Close
        contentItem: ColumnLayout {
            spacing: 6
            Label { text: qsTr("Guess the Number — Cross-Platform"); font.bold: true }
            Label { text: qsTr("Lab 9 / Task 3 — Qt 6 / QML") }
            Label { text: qsTr("Drop any file with digits in its name to seed the game.") }
        }
    }

    header: Column {
        // Classic desktop menu bar (embedded so it works inside a Page)
        MenuBar {
            width: parent.width
            Menu {
                title: qsTr("&Game")
                Action { text: qsTr("&New game"); onTriggered: gameVm.newGame() }
                MenuSeparator {}
                Action { text: qsTr("Reset &statistics"); onTriggered: settingsMgr.resetStats() }
                MenuSeparator {}
                Action { text: qsTr("E&xit"); onTriggered: Qt.quit() }
            }
            Menu {
                title: qsTr("&View")
                Action { text: qsTr("&Play"); onTriggered: tabs.currentIndex = 0 }
                Action { text: qsTr("&History"); onTriggered: tabs.currentIndex = 1 }
                Action { text: qsTr("&Settings"); onTriggered: tabs.currentIndex = 2 }
            }
            Menu {
                title: qsTr("&Language")
                Repeater {
                    model: localizer.supportedLocales
                    MenuItem {
                        text: localizer.displayName(modelData)
                        checkable: true
                        checked: localizer.currentLocale === modelData
                        onTriggered: localizer.setLocale(modelData)
                    }
                }
            }
            Menu {
                title: qsTr("&Help")
                Action { text: qsTr("Keyboard &shortcuts"); onTriggered: helpDialog.open() }
                Action { text: qsTr("&About"); onTriggered: aboutDialog.open() }
            }
        }
        ToolBar {
            width: parent.width
            background: Rectangle { color: theme.linuxToolbar; border.color: theme.linuxBorder }
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 6
                spacing: 4
                ToolButton {
                    text: qsTr("New")
                    icon.source: "qrc:/qt/qml/GuessNumberX/icons/dice.svg"
                    onClicked: gameVm.newGame()
                    ToolTip.text: qsTr("Start a new game (Ctrl+N)")
                    ToolTip.visible: hovered
                    ToolTip.delay: 600
                }
                ToolButton {
                    text: qsTr("History")
                    icon.source: "qrc:/qt/qml/GuessNumberX/icons/playlist.svg"
                    onClicked: tabs.currentIndex = 1
                }
                ToolButton {
                    text: qsTr("Settings")
                    icon.source: "qrc:/qt/qml/GuessNumberX/icons/settings.svg"
                    onClicked: tabs.currentIndex = 2
                }
                Item { Layout.fillWidth: true }
                Label {
                    text: qsTr("Best: %1   Games: %2").arg(gameVm.bestScore).arg(gameVm.gamesPlayed)
                    color: "#444"
                }
                LanguageMenu { Layout.preferredWidth: 130 }
            }
        }
        TabBar {
            id: tabs
            width: parent.width
            TabButton { text: qsTr("Play") }
            TabButton { text: qsTr("History") }
            TabButton { text: qsTr("Settings") }
        }
    }

    StackLayout {
        id: stack
        anchors.fill: parent
        currentIndex: tabs.currentIndex

        // Play
        Item {
            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16

                GameCore {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width * 0.6
                    Layout.minimumWidth: 320
                    bgColor: theme.linuxBg
                    cardColor: theme.linuxSurface
                    textColor: "#212121"
                    accentColor: theme.linuxAccent
                    accentPressed: "#0D47A1"
                    errorColor: theme.error
                    controlHeight: 32
                    controlRadius: 4
                    simpleProgress: true
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    spacing: 12
                    StatsCard {
                        Layout.fillWidth: true
                        cardColor: theme.linuxSurface
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: dropHint.implicitHeight + 24
                        radius: 4
                        color: theme.linuxSurface
                        border.color: theme.linuxBorder
                        ColumnLayout {
                            id: dropHint
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 6
                            Label {
                                text: qsTr("Drag-and-drop")
                                font.bold: true
                            }
                            Label {
                                text: qsTr("Drop a file with digits in its filename to seed a custom game.")
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                                color: "#555"
                            }
                            Label {
                                id: dropMsg
                                color: theme.linuxAccent
                                visible: text.length > 0
                                wrapMode: Text.WrapAnywhere
                                Layout.fillWidth: true
                            }
                        }
                    }
                    Item { Layout.fillHeight: true }
                }
            }
        }

        // History
        Item {
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 8
                Label { text: qsTr("Game History"); font.bold: true; font.pixelSize: 16 }
                ListView {
                    id: histView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: settingsMgr.historyJson()
                    delegate: Rectangle {
                        width: histView.width; height: 32
                        color: index % 2 === 0 ? theme.linuxSurface : "#FAFAFA"
                        border.color: theme.linuxBorder
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 8; anchors.rightMargin: 8
                            spacing: 12
                            Label { text: "#" + (histView.count - index); font.family: "monospace"; Layout.preferredWidth: 32 }
                            Label { text: modelData.won ? "✓" : "✗"; color: modelData.won ? theme.primary : theme.error; font.bold: true }
                            Label {
                                text: qsTr("secret %1, %2 attempts").arg(modelData.secret).arg(modelData.attempts)
                                Layout.fillWidth: true
                            }
                            Label { text: "+" + modelData.score; font.family: "monospace" }
                            Label { text: modelData.ts; color: "#888"; font.pixelSize: 11 }
                        }
                    }
                    Connections {
                        target: settingsMgr
                        function onSettingsChanged() { histView.model = settingsMgr.historyJson() }
                    }
                }
            }
        }

        // Settings
        Item {
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 16

                GroupBox {
                    title: qsTr("Language")
                    Layout.fillWidth: true
                    RowLayout {
                        Label { text: qsTr("Interface language:") }
                        LanguageMenu { Layout.preferredWidth: 200 }
                    }
                }

                GroupBox {
                    title: qsTr("Gameplay")
                    Layout.fillWidth: true
                    RowLayout {
                        Label { text: qsTr("Max attempts:") }
                        NumberInput { Layout.preferredWidth: 200 }
                    }
                }

                GroupBox {
                    title: qsTr("Application info")
                    Layout.fillWidth: true
                    Label {
                        text: settingsMgr.formattedSettings()
                        font.family: "monospace"
                        font.pixelSize: 12
                        wrapMode: Text.WrapAnywhere
                        Layout.fillWidth: true
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
    }

    Connections {
        target: dropHandler
        function onFileDropped(url, seed) {
            dropMsg.text = qsTr("File loaded: seed=%1").arg(seed)
            gameVm.newGame()
            // submit the seed once to give the user instant feedback
            gameVm.submitGuessInt(seed)
        }
        function onDropError(message) {
            dropMsg.text = message
        }
    }

    // Drop overlay
    Rectangle {
        id: dropOverlay
        anchors.fill: parent
        visible: false
        color: "#3300AAFF"
        border.color: theme.linuxAccent
        border.width: 3
        z: 100
        Label {
            anchors.centerIn: parent
            text: qsTr("Drop file to seed game")
            font.pixelSize: 22
            font.bold: true
            color: theme.linuxAccent
        }
    }

    // Status bar
    footer: Rectangle {
        height: 22
        color: theme.linuxToolbar
        border.color: theme.linuxBorder
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 8; anchors.rightMargin: 8
            Label { text: gameVm.attemptsText; font.pixelSize: 11 }
            Item { Layout.fillWidth: true }
            Label { text: qsTr("Lang: %1").arg(localizer.currentLocale); font.pixelSize: 11 }
        }
    }
}
