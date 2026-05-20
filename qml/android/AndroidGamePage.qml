import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../shared"

Page {
    background: Rectangle { color: "#FAFAFA" }
    PlatformTheme { id: theme }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        GameCore {
            id: core
            Layout.fillWidth: true
            Layout.fillHeight: true
            bgColor: theme.androidBg
            cardColor: "#EDE7F6"
            textColor: "#212121"
            accentColor: theme.androidPrimary
            accentPressed: "#3700B3"
            errorColor: theme.error
            controlHeight: theme.androidTouchSize
            controlRadius: theme.androidRadius
            largeTouch: true
        }

        StatsCard {
            Layout.fillWidth: true
            cardColor: "#FFFFFF"
            textColor: "#212121"
            subTextColor: "#666666"
        }
    }
}
