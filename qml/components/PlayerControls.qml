import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    RowLayout {
        anchors.fill: parent
        Button { text: "⏮"; onClicked: appController.previous() }
        Button { text: appController.playing ? "⏸" : "▶"; onClicked: appController.togglePlayback() }
        Button { text: "⏹"; onClicked: appController.stop() }
        Button { text: "⏭"; onClicked: appController.next() }
        Label { text: appController.formatTime(appController.position) }
        Slider {
            Layout.fillWidth: true
            from: 0
            to: Math.max(1, appController.duration)
            value: appController.position
            onMoved: appController.seek(value)
        }
        Label { text: appController.formatTime(appController.duration) }
    }
}
