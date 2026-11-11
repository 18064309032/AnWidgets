cmake_minimum_required(VERSION 3.16)
project(AnWidgets VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# ════════ 动态库（默认）════════
option(AN_WIDGETS_BUILD_SHARED "Build shared library" ON)
option(AN_WIDGETS_BUILD_DEMO "Build demo application" ON)

# 源文件
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

# Demo
if(AN_WIDGETS_BUILD_DEMO)
    add_subdirectory(demo)
endif()
