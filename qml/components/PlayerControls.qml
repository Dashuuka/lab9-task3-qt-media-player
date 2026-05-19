import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    property var controller

    RowLayout {
        anchors.fill: parent
        Button { objectName: "previousButton"; text: "⏮"; onClicked: controller.previous() }
        Button { objectName: "playPauseButton"; text: controller.playing ? "⏸" : "▶"; onClicked: controller.togglePlayback() }
        Button { objectName: "stopButton"; text: "⏹"; onClicked: controller.stop() }
        Button { objectName: "nextButton"; text: "⏭"; onClicked: controller.next() }
        Label { objectName: "positionLabel"; text: controller.formatTime(controller.position) }
        Slider {
            objectName: "positionSlider"
            Layout.fillWidth: true
            from: 0
            to: Math.max(1, controller.duration)
            value: controller.position
            onMoved: controller.seek(value)
        }
        Label { objectName: "durationLabel"; text: controller.formatTime(controller.duration) }
    }
}
