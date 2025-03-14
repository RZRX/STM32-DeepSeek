# Корневой CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(STM32_IDE VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Widgets Network Charts REQUIRED)
find_package(QScintilla REQUIRED)
find_package(STM32CubeF4 REQUIRED) # Поддержка конкретной серии

add_subdirectory(src)
add_subdirectory(resources)
add_subdirectory(plugins)

# Настройка аппаратной специфики
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    STM32F407xx
    USE_HAL_DRIVER
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    STM32::CMSIS
    STM32::HAL
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY examples DESTINATION share/stm32-ide)
