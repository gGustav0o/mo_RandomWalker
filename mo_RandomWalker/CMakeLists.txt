cmake_minimum_required(VERSION 3.21)
project(RandomWalker LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==== Попытка найти Eigen3 ====
find_package(Eigen3 QUIET)

# ==== Если не найден — вызываем Conan ====
if (NOT Eigen3_FOUND)
    message(STATUS "Eigen3 не найден. Подключаем через Conan...")

    # Определим профиль Conan в зависимости от сборки
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(_conan_profile msvc-release)
    else()
        set(_conan_profile msvc-debug)
    endif()

    # Вызываем Conan только если toolchain не существует
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
        message(STATUS "Выполняем: conan install с профилем ${_conan_profile}")
        execute_process(
            COMMAND conan install ${CMAKE_SOURCE_DIR}
                    --output-folder=${CMAKE_BINARY_DIR}
                    --build=missing
                    --profile:build=${_conan_profile}
            RESULT_VARIABLE result
        )
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Ошибка при вызове conan install!")
        endif()
    endif()

    # Подключаем сгенерированные файлы Conan
    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL RESULT_VARIABLE _toolchain)
    if(NOT _toolchain)
        message(FATAL_ERROR "conan_toolchain.cmake не найден. Проверьте Conan.")
    endif()

    include(${CMAKE_BINARY_DIR}/conan_deps.cmake REQUIRED)
endif()

# Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
)

# === Если не найден — подключим через Conan ===
if (NOT Eigen3_FOUND)
    message(STATUS "Eigen3 не найден — будет использоваться Conan")

    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL RESULT_VARIABLE _toolchain)
    if(NOT _toolchain)
        message(FATAL_ERROR "conan_toolchain.cmake не найден. Убедитесь, что вызван conan install.")
    endif()

    include(${CMAKE_BINARY_DIR}/conan_paths.cmake OPTIONAL)
    include(${CMAKE_BINARY_DIR}/conan_deps.cmake REQUIRED)
endif()

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
