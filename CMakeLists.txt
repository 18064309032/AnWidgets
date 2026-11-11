cmake_minimum_required(VERSION 3.16)
project(AnWidgets VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

# MSVC 用 UTF-8 解析源文件（中文注释不炸）
if(MSVC)
    add_compile_options(/utf-8)
endif()

# Qt 查找：优先 CMAKE_PREFIX_PATH / Qt5_DIR，否则自动探测
if(NOT CMAKE_PREFIX_PATH AND NOT Qt5_DIR)
    foreach(_qt_dir
        "C:/Qt/5.15.2/msvc2019_64"
        "D:/Qt/5.15.2/msvc2019_64"
        "C:/Qt/5.15.2/msvc2017_64"
        "D:/Qt/Qt5.15.2/5.15.2/msvc2019_64"
    )
        if(EXISTS "${_qt_dir}/lib/cmake/Qt5/Qt5Config.cmake")
            set(CMAKE_PREFIX_PATH "${_qt_dir}")
            break()
        endif()
    endforeach()
endif()
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# ════════ 库类型（默认静态，最省事）════════
option(AN_WIDGETS_BUILD_SHARED "Build shared library (OFF=static)" OFF)
option(AN_WIDGETS_BUILD_DEMO "Build demo application" ON)

set(AN_WIDGETS_SOURCES
    anthememanager.cpp
    anthememanager.h
    antypedef.h
    anpushbutton.cpp
    anpushbutton.h
)

if(AN_WIDGETS_BUILD_SHARED)
    add_library(AnWidgets SHARED ${AN_WIDGETS_SOURCES})
    target_compile_definitions(AnWidgets PRIVATE AN_WIDGETS_LIBRARY)
else()
    add_library(AnWidgets STATIC ${AN_WIDGETS_SOURCES})
endif()

target_include_directories(AnWidgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/AnWidgets>
)

target_link_libraries(AnWidgets PUBLIC Qt5::Widgets)

if(AN_WIDGETS_BUILD_DEMO)
    add_subdirectory(examples)
endif()
