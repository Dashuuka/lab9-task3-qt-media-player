import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    ListView {
        id: list
        anchors.fill: parent
        model: playlistModel
        clip: true

        delegate: ItemDelegate {
            width: list.width
            highlighted: index === appController.currentIndex
            onDoubleClicked: appController.playIndex(index)
            contentItem: RowLayout {
                Label {
                    Layout.fillWidth: true
                    text: title
                    elide: Text.ElideRight
                }
                Label {
                    Layout.preferredWidth: 280
                    text: filePath
                    elide: Text.ElideMiddle
                }
                Button {
                    text: "Remove"
                    onClicked: appController.removeAt(index)
                }
            }
        }

        Label {
            anchors.centerIn: parent
            visible: playlistModel.rowCount() === 0
            text: qsTr("Playlist is empty")
        }
    }
}
