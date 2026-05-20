import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Platform-agnostic gameplay column: title, image+animations,
// input row, error label, progress. Visual style is supplied
// by the host page via the theme/colors properties.
Item {
    id: root

    // Visual hooks
    property color bgColor: "#FFFFFF"
    property color cardColor: "#F4F4F4"
    property color textColor: "#212121"
    property color accentColor: "#4CAF50"
    property color accentPressed: "#388E3C"
    property color errorColor: "#F44336"
    property int controlHeight: 44
    property int controlRadius: 8
    property bool flatStyle: false           // iOS-style flat buttons
    property bool largeTouch: false          // Android: 56px touch targets
    property bool simpleProgress: false      // Linux: classic progress bar
    property bool showInlineDialog: false    // when true, dialog inline-card instead of popup

    implicitWidth: 360
    implicitHeight: 480

    signal newGameRequested()

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            if (!gameVm.gameFinished) submitBtn.clicked()
            event.accepted = true
        } else if (event.key === Qt.Key_R && (event.modifiers & Qt.ControlModifier)) {
            gameVm.newGame()
            event.accepted = true
        } else if (event.key === Qt.Key_F1) {
            // Help — show secret hint range
            statusOverride.text = qsTr("Hint: try a number near the middle of 0..99")
            event.accepted = true
        }
    }

    focus: true

    Rectangle {
        anchors.fill: parent
        color: root.bgColor
    }

    Dialog {
        id: resultDialog
        modal: true
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.9, 420)
        property string dialogTitle: ""
        property string dialogMessage: ""
        title: dialogTitle
        standardButtons: Dialog.Ok
        contentItem: ColumnLayout {
            spacing: 12
            Label {
                text: resultDialog.dialogMessage
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.margins: 12
                color: root.textColor
            }
        }
        onAccepted: root.newGameRequested()
    }

    Connections {
        target: gameVm
        function onShowResultDialog(title, message) {
            if (root.showInlineDialog) {
                inlineBanner.title = title
                inlineBanner.message = message
                inlineBanner.visible = true
                inlineFade.start()
            } else {
                resultDialog.dialogTitle = title
                resultDialog.dialogMessage = message
                resultDialog.open()
            }
        }
        function onInputError(message) {
            errorLabel.text = message
            errorAnim.start()
        }
        function onAnimationRequested() {
            anims.play(gameVm.animationType)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Title / status
        Label {
            id: statusOverride
            text: gameVm.statusText
            font.pixelSize: 22
            font.bold: true
            color: root.textColor
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
        }

        // Animated image
        Item {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 130
            Layout.preferredHeight: 130
            Image {
                id: gameImage
                anchors.centerIn: parent
                width: 110; height: 110
                source: gameVm.imageSource
                fillMode: Image.PreserveAspectFit
                sourceSize.width: 110
                sourceSize.height: 110
            }
            GameAnimations {
                id: anims
                target: gameImage
            }
        }

        // Inline banner (used when showInlineDialog true, e.g. iOS)
        Rectangle {
            id: inlineBanner
            visible: false
            opacity: 0
            property string title: ""
            property string message: ""
            Layout.fillWidth: true
            Layout.preferredHeight: bannerCol.implicitHeight + 24
            radius: root.controlRadius
            color: root.cardColor
            border.color: "#E0E0E0"
            ColumnLayout {
                id: bannerCol
                anchors.fill: parent
                anchors.margins: 12
                spacing: 4
                Label { text: inlineBanner.title; font.bold: true; color: root.textColor }
                Label { text: inlineBanner.message; wrapMode: Text.WordWrap; Layout.fillWidth: true; color: root.textColor }
            }
            NumberAnimation { id: inlineFade; target: inlineBanner; property: "opacity"; from: 0; to: 1; duration: 280 }
            Timer {
                interval: 3000; running: inlineBanner.visible
                onTriggered: { inlineBanner.visible = false; inlineBanner.opacity = 0 }
            }
        }

        // Input row
        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            TextField {
                id: guessInput
                objectName: "guessInput"
                Layout.fillWidth: true
                Layout.preferredHeight: root.largeTouch ? 56 : root.controlHeight
                placeholderText: qsTr("Enter number (0-99)")
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                validator: IntValidator { bottom: 0; top: 99 }
                enabled: !gameVm.gameFinished
                font.pixelSize: root.largeTouch ? 20 : 16
                horizontalAlignment: TextInput.AlignHCenter
                background: Rectangle {
                    radius: root.controlRadius
                    color: root.cardColor
                    border.color: guessInput.activeFocus ? root.accentColor : "#D0D0D0"
                    border.width: guessInput.activeFocus ? 2 : 1
                }
                padding: 8
                onAccepted: submitBtn.clicked()
            }
            Button {
                id: submitBtn
                objectName: "submitBtn"
                text: gameVm.gameFinished ? qsTr("New") : qsTr("Submit")
                font.pixelSize: 16
                font.bold: true
                Layout.preferredWidth: root.largeTouch ? 130 : 110
                Layout.preferredHeight: root.largeTouch ? 56 : root.controlHeight
                background: Rectangle {
                    radius: root.controlRadius
                    color: submitBtn.pressed ? root.accentPressed : root.accentColor
                    border.width: root.flatStyle ? 1 : 0
                    border.color: root.accentColor
                    Behavior on color { ColorAnimation { duration: 120 } }
                    layer.enabled: !root.flatStyle
                }
                contentItem: Text {
                    text: submitBtn.text
                    color: root.flatStyle ? root.accentColor : "white"
                    font: submitBtn.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: {
                    errorLabel.text = ""
                    if (gameVm.gameFinished) {
                        gameVm.newGame()
                        guessInput.text = ""
                    } else {
                        gameVm.submitGuess(guessInput.text)
                        guessInput.text = ""
                    }
                }
            }
        }

        Label {
            id: errorLabel
            objectName: "errorLabel"
            text: ""
            color: root.errorColor
            font.pixelSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            visible: text.length > 0
            SequentialAnimation {
                id: errorAnim
                NumberAnimation { target: errorLabel; property: "x"; to: errorLabel.x + 8; duration: 50 }
                NumberAnimation { target: errorLabel; property: "x"; to: errorLabel.x - 8; duration: 50 }
                NumberAnimation { target: errorLabel; property: "x"; to: errorLabel.x + 4; duration: 50 }
                NumberAnimation { target: errorLabel; property: "x"; to: errorLabel.x; duration: 50 }
            }
        }

        Label {
            text: gameVm.attemptsText
            font.pixelSize: 14
            color: root.textColor
            opacity: 0.7
            Layout.alignment: Qt.AlignHCenter
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            from: 0; to: gameVm.maxAttempts; value: gameVm.currentAttempts
            background: Rectangle {
                implicitHeight: root.simpleProgress ? 6 : 10
                radius: root.simpleProgress ? 2 : 5
                color: "#E0E0E0"
            }
            contentItem: Item {
                implicitHeight: root.simpleProgress ? 6 : 10
                Rectangle {
                    width: progressBar.visualPosition * parent.width
                    height: parent.height
                    radius: root.simpleProgress ? 2 : 5
                    color: progressBar.value >= gameVm.maxAttempts * 0.7 ? root.errorColor : root.accentColor
                    Behavior on width { NumberAnimation { duration: 250 } }
                    Behavior on color { ColorAnimation { duration: 250 } }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
