cmake_minimum_required(VERSION 3.21)

project(GuessNumberX VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ── Qt detection ────────────────────────────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 LinguistTools Svg Qml Gui)

qt_standard_project_setup(REQUIRES 6.5)

# ── Sources ─────────────────────────────────────────────────────────────────
set(APP_HEADERS
    src/gamemodel.h
    src/gameviewmodel.h
    src/settingsmanager.h
    src/platforminfo.h
    src/localizer.h
    src/drophandler.h
)

set(APP_SOURCES
    src/main.cpp
    src/gamemodel.cpp
    src/gameviewmodel.cpp
    src/settingsmanager.cpp
    src/platforminfo.cpp
    src/localizer.cpp
    src/drophandler.cpp
)

qt_add_executable(appGuessNumberX ${APP_HEADERS} ${APP_SOURCES})

qt_add_qml_module(appGuessNumberX
    URI GuessNumberX
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/shared/GameCore.qml
        qml/shared/PlatformTheme.qml
        qml/shared/StatsCard.qml
        qml/shared/NumberInput.qml
        qml/shared/GameAnimations.qml
        qml/shared/LanguageMenu.qml
        qml/android/AndroidUI.qml
        qml/android/AndroidGamePage.qml
        qml/android/AndroidSettingsPage.qml
        qml/android/AndroidPlaylistPage.qml
        qml/ios/IosUI.qml
        qml/ios/IosGamePage.qml
        qml/ios/IosSettingsPage.qml
        qml/ios/IosPlaylistPage.qml
        qml/linux/LinuxUI.qml
        qml/web/WebUI.qml
    RESOURCES
        icons/question.svg
        icons/arrow_up.svg
        icons/arrow_down.svg
        icons/trophy.svg
        icons/dice.svg
        icons/settings.svg
        icons/playlist.svg
        icons/home.svg
)

# ── Translations ────────────────────────────────────────────────────────────
set(TS_FILES
    i18n/lab9_en.ts
    i18n/lab9_ru.ts
    i18n/lab9_be.ts
)

qt_add_translations(appGuessNumberX
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/i18n"
)

# ── Platform-specific defines so QML can pick the right layout ──────────────
if(ANDROID)
    target_compile_definitions(appGuessNumberX PRIVATE TARGET_ANDROID=1)
elseif(IOS)
    target_compile_definitions(appGuessNumberX PRIVATE TARGET_IOS=1)
elseif(EMSCRIPTEN)
    target_compile_definitions(appGuessNumberX PRIVATE TARGET_WEB=1)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(appGuessNumberX PRIVATE TARGET_LINUX=1)
elseif(WIN32)
    target_compile_definitions(appGuessNumberX PRIVATE TARGET_LINUX=1)
elseif(APPLE)
    target_compile_definitions(appGuessNumberX PRIVATE TARGET_LINUX=1)
endif()

set_target_properties(appGuessNumberX PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGuessNumberX
    PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::Svg Qt6::Qml Qt6::Gui
)

install(TARGETS appGuessNumberX
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ── Tests (host only; skip in WebAssembly / Android / iOS) ──────────────────
if(NOT EMSCRIPTEN AND NOT ANDROID AND NOT IOS)
    find_package(Qt6 REQUIRED COMPONENTS Test QuickTest)
    enable_testing()

    # Unit tests — core game logic
    qt_add_executable(tst_unit
        tests/unit/tst_unit.cpp
        src/gamemodel.h   src/gamemodel.cpp
        src/settingsmanager.h src/settingsmanager.cpp
        src/platforminfo.h src/platforminfo.cpp
        src/localizer.h src/localizer.cpp
    )
    target_include_directories(tst_unit PRIVATE src)
    target_link_libraries(tst_unit PRIVATE Qt6::Test Qt6::Quick Qt6::Qml Qt6::Gui)
    add_test(NAME tst_unit COMMAND tst_unit)

    # Integration tests — VM + Settings together
    qt_add_executable(tst_integration
        tests/integration/tst_integration.cpp
        src/gamemodel.h   src/gamemodel.cpp
        src/gameviewmodel.h src/gameviewmodel.cpp
        src/settingsmanager.h src/settingsmanager.cpp
        src/platforminfo.h src/platforminfo.cpp
        src/localizer.h src/localizer.cpp
        src/drophandler.h src/drophandler.cpp
    )
    target_include_directories(tst_integration PRIVATE src)
    target_link_libraries(tst_integration PRIVATE Qt6::Test Qt6::Quick Qt6::Qml Qt6::Gui)
    add_test(NAME tst_integration COMMAND tst_integration)

    # Widget/UI tests — QML via QuickTest
    qt_add_executable(tst_widget
        tests/widget/tst_widget.cpp
        src/gamemodel.h   src/gamemodel.cpp
        src/gameviewmodel.h src/gameviewmodel.cpp
        src/settingsmanager.h src/settingsmanager.cpp
        src/platforminfo.h src/platforminfo.cpp
        src/localizer.h src/localizer.cpp
        src/drophandler.h src/drophandler.cpp
    )
    # Re-register shared QML types under a *private* module so test
    # .qml files can `import GuessNumberXTest` and instantiate GameCore etc.
    qt_add_qml_module(tst_widget
        URI GuessNumberXTest
        VERSION 1.0
        NO_PLUGIN
        OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/GuessNumberXTest
        QML_FILES
            qml/shared/GameCore.qml
            qml/shared/GameAnimations.qml
            qml/shared/StatsCard.qml
            qml/shared/NumberInput.qml
            qml/shared/LanguageMenu.qml
            qml/shared/PlatformTheme.qml
        RESOURCES
            icons/question.svg
            icons/arrow_up.svg
            icons/arrow_down.svg
            icons/trophy.svg
    )
    target_compile_definitions(tst_widget PRIVATE
        QUICK_TEST_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/tests/widget/qml\"
    )
    target_include_directories(tst_widget PRIVATE src)
    target_link_libraries(tst_widget PRIVATE Qt6::QuickTest Qt6::Quick Qt6::Qml Qt6::Gui Qt6::Test)
    add_test(NAME tst_widget COMMAND tst_widget -platform offscreen)
endif()
