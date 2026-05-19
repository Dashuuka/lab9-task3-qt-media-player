cmake_minimum_required(VERSION 3.24)

project(task3_qt_media_player VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2 Multimedia Sql Test)

qt_add_executable(task3_qt_media_player
    src/main.cpp
    src/audiotrack.h
    src/audiotrack.cpp
    src/playlistmodel.h
    src/playlistmodel.cpp
    src/playlistrepository.h
    src/playlistrepository.cpp
    src/filesystemscanner.h
    src/filesystemscanner.cpp
    src/appcontroller.h
    src/appcontroller.cpp
)

qt_add_qml_module(task3_qt_media_player
    URI Lab9MediaPlayer
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/components/PlayerControls.qml
        qml/pages/PlaylistPage.qml
)

target_link_libraries(task3_qt_media_player
    PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::Multimedia Qt6::Sql
)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
