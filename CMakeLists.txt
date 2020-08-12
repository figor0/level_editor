cmake_minimum_required(VERSION 3.5)

project(level_editor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
set(HEADERS
    LevelModel.h
    InputModel.h
    TypesModel.h
)
set(SOURCES
    LevelModel.cpp
    InputModel.cpp
    TypesModel.cpp
)

add_subdirectory(libs/parser)
include_directories(${Parser_INCLUDE})

add_executable(level_editor
        main.cpp
        ${SOURCES}
        ${HEADERS}
        qml.qrc
)

target_link_libraries(level_editor
  PRIVATE Qt5::Core Qt5::Quick parser)
