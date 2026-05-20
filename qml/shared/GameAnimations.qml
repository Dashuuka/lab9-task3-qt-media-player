import QtQuick

// Animation behaviours attached to a target item.
// Pass the target image; play the requested animation via `play(type)`.
Item {
    id: root
    property Item target: null
    visible: false

    function play(kind) {
        if (!target) return
        if (kind === "bounce") bounceAnim.start()
        else if (kind === "rotate") rotateAnim.start()
        else if (kind === "fadeIn") fadeAnim.start()
    }

    SequentialAnimation {
        id: bounceAnim
        ParallelAnimation {
            NumberAnimation { target: root.target; property: "scale"; from: 1.0; to: 1.35; duration: 180; easing.type: Easing.OutQuad }
            NumberAnimation { target: root.target; property: "y"; from: 0; to: -25; duration: 180; easing.type: Easing.OutQuad }
        }
        ParallelAnimation {
            NumberAnimation { target: root.target; property: "scale"; from: 1.35; to: 1.0; duration: 220; easing.type: Easing.OutBounce }
            NumberAnimation { target: root.target; property: "y"; from: -25; to: 0; duration: 220; easing.type: Easing.OutBounce }
        }
    }

    RotationAnimation {
        id: rotateAnim
        target: root.target
        from: 0; to: 360
        duration: 800
        direction: RotationAnimation.Clockwise
    }

    NumberAnimation {
        id: fadeAnim
        target: root.target
        property: "opacity"
        from: 0; to: 1
        duration: 500
        easing.type: Easing.OutCubic
    }
}
