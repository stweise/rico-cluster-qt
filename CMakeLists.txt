# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.5)

project(rico-cluster-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
IF(WIN32)
SET(GUI_TYPE WIN32)
ENDIF(WIN32)
# Set some Apple MacOS Specific settings
IF (APPLE)
SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

add_executable(rico-cluster-qt ${GUI_TYPE}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    graphicsscene.h
    graphicsscene.cpp
    node.h
    node.cpp
    edge.h
    edge.cpp
    )

target_link_libraries(rico-cluster-qt PRIVATE Qt5::Widgets)
