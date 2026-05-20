import QtQuick
import QtQuick.Controls
import "../android" as A
import "../linux" as L

// WebUI is rarely used directly — Main.qml's Loader already
// switches between Android (mobile) and Linux (desktop) at the
// window-width breakpoint. This wrapper exists for reference and
// can host a forced "web-only" mode if needed.
Item {
    id: root
    anchors.fill: parent

    Loader {
        anchors.fill: parent
        sourceComponent: root.width < 720 ? mobileSkin : desktopSkin
    }
    Component { id: mobileSkin; A.AndroidUI {} }
    Component { id: desktopSkin; L.LinuxUI {} }
}
