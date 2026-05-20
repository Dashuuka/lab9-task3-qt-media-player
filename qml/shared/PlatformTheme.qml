import QtQuick

// Centralised palette + sizing tokens. Each platform's UI
// uses a subset that matches its design language.
QtObject {
    // Common accent
    readonly property color primary: "#4CAF50"
    readonly property color primaryDark: "#388E3C"
    readonly property color error: "#F44336"
    readonly property color warning: "#FFC107"

    // Android (Material)
    readonly property color androidBg: "#FAFAFA"
    readonly property color androidSurface: "#FFFFFF"
    readonly property color androidPrimary: "#6200EE"
    readonly property color androidOnPrimary: "#FFFFFF"
    readonly property color androidSecondary: "#03DAC6"
    readonly property int androidTouchSize: 56
    readonly property int androidRadius: 12

    // iOS (Apple Music style)
    readonly property color iosBg: "#FFFFFF"
    readonly property color iosTint: "#FF2D55"        // Apple Music pink
    readonly property color iosTabBg: "#F8F8F8"
    readonly property color iosTabBorder: "#D0D0D0"
    readonly property color iosText: "#1C1C1E"
    readonly property color iosSecondaryText: "#8E8E93"
    readonly property int iosRadius: 10

    // Linux (Fusion / classic)
    readonly property color linuxBg: "#F0F0F0"
    readonly property color linuxSurface: "#FFFFFF"
    readonly property color linuxAccent: "#1976D2"
    readonly property color linuxBorder: "#B0B0B0"
    readonly property color linuxToolbar: "#E0E0E0"
    readonly property int linuxControlHeight: 28
}
