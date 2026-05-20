import QtQuick
import QtQuick.Controls
import QtQuick.Window
import "android" as A
import "ios" as I
import "linux" as L
import "web" as W

ApplicationWindow {
    id: window
    width: platformInfo.isMobile ? 400 : 1000
    height: platformInfo.isMobile ? 720 : 700
    minimumWidth: 320
    minimumHeight: 480
    visible: true
    title: qsTr("Guess the Number — Cross-Platform")

    // Platform router. Resolves to a single UI implementation
    // for the current target. On Web, picks Android vs Linux
    // based on viewport size (responsive layout).
    Loader {
        id: uiLoader
        anchors.fill: parent
        sourceComponent: {
            if (platformInfo.isAndroid) return androidPage
            if (platformInfo.isIos) return iosPage
            if (platformInfo.isWeb) {
                return window.width < 720 ? androidPage : linuxPage
            }
            return linuxPage
        }

        // Smooth swap when Web resizes across the breakpoint
        Behavior on opacity { NumberAnimation { duration: 180 } }
    }

    Component { id: androidPage; A.AndroidUI {} }
    Component { id: iosPage; I.IosUI {} }
    Component { id: linuxPage; L.LinuxUI {} }
    Component { id: webPage; W.WebUI {} }

    Connections {
        target: localizer
        function onLocaleChanged() {
            // QML strings retranslate via QQmlEngine::retranslate() automatically;
            // the title binding is re-evaluated.
            window.title = qsTr("Guess the Number — Cross-Platform")
        }
    }
}
