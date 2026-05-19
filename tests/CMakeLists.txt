qt_add_executable(task3_qt_media_player_tests
    test_playlist.cpp
    ../src/audiotrack.h
    ../src/audiotrack.cpp
    ../src/filesystemscanner.h
    ../src/filesystemscanner.cpp
    ../src/playlistmodel.h
    ../src/playlistmodel.cpp
    ../src/playlistrepository.h
    ../src/playlistrepository.cpp
)

target_include_directories(task3_qt_media_player_tests PRIVATE ../src)
target_link_libraries(task3_qt_media_player_tests PRIVATE Qt6::Test Qt6::Sql)
add_test(NAME task3_qt_media_player_tests COMMAND task3_qt_media_player_tests)
