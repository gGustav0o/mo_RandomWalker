cmake_minimum_required(VERSION 3.21)
project(RandomWalker LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
)

# Eigen
find_package(Eigen3 REQUIRED)

# Подключаем include-пути
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Список исходников
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    core/*.cpp
    core/*.hpp
)
set(HEADERS
    global.hpp
    MetaAnnotations.hpp
)
set(SOURCES
    main.cpp
    ${CORE_SOURCES}
    qml.qrc
)

# Добавление исполняемого файла
qt_add_executable(RandomWalker
    ${SOURCES}
    ${HEADERS}
)

# Линковка библиотек
target_link_libraries(RandomWalker
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Eigen3::Eigen
)
