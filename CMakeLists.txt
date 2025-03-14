cmake_minimum_required(VERSION 3.20)
project(STM32_IDE VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt6 COMPONENTS Core Widgets Gui Concurrent SerialPort OpenGL Network REQUIRED)
find_package(QScintilla REQUIRED)

add_subdirectory(src)
