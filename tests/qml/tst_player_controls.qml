import QtQuick
import QtQuick.Controls
import QtTest
import "../../qml/components" as Components

TestCase {
    name: "PlayerControlsUiTests"

    QtObject {
        id: fakeController
        property bool playing: false
        property int position: 5000
        property int duration: 120000
        property bool toggled: false
        property int seekValue: -1

        function previous() {}
        function next() {}
        function stop() {}
        function togglePlayback() { toggled = true }
        function seek(value) { seekValue = value }
        function formatTime(value) { return Math.floor(value / 60000) + ":" + ("0" + Math.floor((value / 1000) % 60)).slice(-2) }
    }

    function createControls() {
        var component = Qt.createComponent("../../qml/components/PlayerControls.qml")
        verify(component.status === Component.Ready, component.errorString())
        var item = component.createObject(null, { controller: fakeController })
        verify(item !== null)
        return item
    }

    function test_playPauseButtonIsVisibleForPausedState() {
        var item = createControls()
        var button = findChild(item, "playPauseButton")
        verify(button !== null)
        compare(button.text, "▶")
        item.destroy()
    }

    function test_playPauseButtonCallsController() {
        var item = createControls()
        var button = findChild(item, "playPauseButton")
        button.clicked()
        verify(fakeController.toggled)
        item.destroy()
    }

    function test_progressSliderAndTimeLabelsRenderPlaybackState() {
        var item = createControls()
        var slider = findChild(item, "positionSlider")
        var position = findChild(item, "positionLabel")
        var duration = findChild(item, "durationLabel")
        verify(slider !== null)
        compare(slider.value, 5000)
        compare(position.text, "0:05")
        compare(duration.text, "2:00")
        item.destroy()
    }
}
