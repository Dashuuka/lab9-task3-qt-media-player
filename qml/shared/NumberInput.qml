import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Simple numeric stepper used for max-attempts setting.
RowLayout {
    id: root
    property alias value: spin.value
    property int from: 1
    property int to: 100
    SpinBox {
        id: spin
        from: root.from
        to: root.to
        editable: true
        value: gameVm.maxAttempts
        onValueModified: gameVm.setMaxAttempts(value)
    }
}
