import QtQuick
import QtQuick.Controls
import QtTest
import GuessNumberXTest

TestCase {
    id: tc
    name: "LocalizerTests"
    width: 300
    height: 200
    visible: true
    when: windowShown

    Component {
        id: menuComp
        LanguageMenu { width: 200 }
    }

    function test_0_combo_lists_supported() {
        const c = createTemporaryObject(menuComp, tc)
        verify(c)
        compare(c.model.length, localizer.supportedLocales.length)
    }

    function test_2_unknown_locale_rejected() {
        const before = localizer.currentLocale
        verify(!localizer.setLocale("xx"))
        compare(localizer.currentLocale, before)
    }

    function test_3_display_name_is_human() {
        verify(localizer.displayName("en").length > 0)
        verify(localizer.displayName("ru").indexOf("Рус") === 0)
        verify(localizer.displayName("be").length > 0)
    }
}
