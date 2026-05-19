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
    add_test(
        NAME task3_qml_ui_tests
        COMMAND Qt6::qmltestrunner -input ${CMAKE_CURRENT_SOURCE_DIR}/qml -import ${CMAKE_SOURCE_DIR}/qml
    )
    set_tests_properties(task3_qml_ui_tests PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    )
endif()
