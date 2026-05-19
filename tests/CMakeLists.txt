set(TASK3_CORE_SOURCES
    ../src/audiotrack.h
    ../src/audiotrack.cpp
    ../src/filesystemscanner.h
    ../src/filesystemscanner.cpp
    ../src/playlistmodel.h
    ../src/playlistmodel.cpp
    ../src/playlistrepository.h
    ../src/playlistrepository.cpp
)

qt_add_executable(task3_qt_unit_tests
    test_playlist_unit.cpp
    ${TASK3_CORE_SOURCES}
)
target_include_directories(task3_qt_unit_tests PRIVATE ../src)
target_link_libraries(task3_qt_unit_tests PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME task3_qt_unit_tests COMMAND task3_qt_unit_tests)

qt_add_executable(task3_qt_integration_tests
    test_playlist_integration.cpp
    ${TASK3_CORE_SOURCES}
)
target_include_directories(task3_qt_integration_tests PRIVATE ../src)
target_link_libraries(task3_qt_integration_tests PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME task3_qt_integration_tests COMMAND task3_qt_integration_tests)

qt_add_executable(task3_qt_media_player_tests
    test_playlist.cpp
    ${TASK3_CORE_SOURCES}
)
target_include_directories(task3_qt_media_player_tests PRIVATE ../src)
target_link_libraries(task3_qt_media_player_tests PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME task3_qt_media_player_tests COMMAND task3_qt_media_player_tests)

if(TARGET Qt6::qmltestrunner)
    set(QML_TEST_OUTPUT "${CMAKE_BINARY_DIR}/tests/qml-ui-test-result.txt")

    add_test(
        NAME task3_qml_ui_tests
        COMMAND ${CMAKE_COMMAND} -E env
<<<<<<< HEAD
            "QT_QPA_PLATFORM=offscreen"
=======
>>>>>>> d8a61eb156593b0ce690d5a340ea52bdae18fb8c
            "QT_QUICK_BACKEND=software"
            "QSG_RHI_BACKEND=software"
            "QML2_IMPORT_PATH=${CMAKE_SOURCE_DIR}/qml"
            "QT_QML_IMPORT_PATH=${CMAKE_SOURCE_DIR}/qml"
<<<<<<< HEAD
            "$<TARGET_FILE:Qt6::qmltestrunner>"
            -input "${CMAKE_CURRENT_SOURCE_DIR}/qml"
            -import "${CMAKE_SOURCE_DIR}/qml"
            -o -,txt
=======
            "QML_IMPORT_TRACE=1"
            "$<TARGET_FILE:Qt6::qmltestrunner>"
            -input "${CMAKE_CURRENT_SOURCE_DIR}/qml"
            -import "${CMAKE_SOURCE_DIR}/qml"
            -o "${QML_TEST_OUTPUT},txt"
>>>>>>> d8a61eb156593b0ce690d5a340ea52bdae18fb8c
    )

    set_tests_properties(task3_qml_ui_tests PROPERTIES
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
endif()
