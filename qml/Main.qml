import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Lab9MediaPlayer

ApplicationWindow {
    id: root
    width: 1040
    height: 680
    visible: true
    title: qsTr("Media Player")

    property string language: "ru"

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            Action { text: qsTr("Open folder"); shortcut: "Ctrl+O"; onTriggered: folderDialog.open() }
            Action { text: qsTr("Add file"); onTriggered: fileDialog.open() }
        }
        Menu {
            title: qsTr("Language")
            Action { text: "RU"; onTriggered: root.language = "ru" }
            Action { text: "EN"; onTriggered: root.language = "en" }
            Action { text: "BE"; onTriggered: root.language = "be" }
        }
    }

    FolderDialog {
        id: folderDialog
        onAccepted: appController.scanFolder(selectedFolder)
    }

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFiles
        nameFilters: ["Audio files (*.mp3 *.wav *.ogg *.m4a *.flac)"]
        onAccepted: {
            for (let i = 0; i < selectedFiles.length; i++) {
                appController.addFile(selectedFiles[i])
            }
        }
    }

    Shortcut { sequence: "Space"; onActivated: appController.togglePlayback() }
    Shortcut { sequence: "Left"; onActivated: appController.seek(Math.max(0, appController.position - 5000)) }
    Shortcut { sequence: "Right"; onActivated: appController.seek(Math.min(appController.duration, appController.position + 5000)) }

    DropArea {
        anchors.fill: parent
        onDropped: function(drop) {
            if (drop.hasUrls) {
                for (let i = 0; i < drop.urls.length; i++) {
                    appController.addFile(drop.urls[i])
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Button { text: qsTr("Open folder"); onClicked: folderDialog.open() }
            Button { text: qsTr("Add file"); onClicked: fileDialog.open() }
            Label {
                Layout.fillWidth: true
                color: "crimson"
                text: appController.errorMessage
                elide: Text.ElideRight
            }
        }

        PlaylistPage {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        PlayerControls {
            Layout.fillWidth: true
        }
    }
}
