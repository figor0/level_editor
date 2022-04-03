cmake_minimum_required(VERSION 3.5)

project(json_parser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)

set(SOURCES general.cpp items_model.cpp)
set(HEADERS general.h items_model.h)

add_library(parser ${SOURCES} ${HEADERS})
#add_executable(json_parser
#  main.cpp ${SOURCES} ${HEADERS}
#)
target_link_libraries(parser Qt5::Core Qt5::Xml Qt5::XmlPatterns)

set(Parser_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
