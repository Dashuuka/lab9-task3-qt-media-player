import QtQuick
import QtQuick.Controls

ComboBox {
    id: combo
    model: localizer.supportedLocales
    textRole: ""
    displayText: localizer.displayName(currentText)
    delegate: ItemDelegate {
        width: combo.width
        text: localizer.displayName(modelData)
        highlighted: combo.highlightedIndex === index
    }
    Component.onCompleted: currentIndex = Math.max(0, model.indexOf(localizer.currentLocale))
    onActivated: function(idx) {
        if (idx < 0) return
        const code = model[idx]
        localizer.setLocale(code)
    }
}
