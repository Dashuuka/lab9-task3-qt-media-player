cmake_minimum_required(VERSION 3.24)

project(task3_qt_media_player VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2 Multimedia Sql Test QuickTest)
qt_standard_project_setup(REQUIRES 6.5)
if(COMMAND qt_policy)
    qt_policy(SET QTP0004 NEW)
endif()
include(GNUInstallDirs)

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

set_target_properties(task3_qt_media_player PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS task3_qt_media_player
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
