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

# Set some Windows Specific settings
IF(WIN32)
	SET(GUI_TYPE WIN32)
ENDIF(WIN32)

# Set some Apple MacOS Specific settings
IF (APPLE)
	SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

# Default to Release builds
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

add_executable(rico-cluster-qt ${GUI_TYPE}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    inputdialog.h
    inputdialog.cpp
    graphicsscene.h
    graphicsscene.cpp
    graphicsview.h
    graphicsview.cpp
    node.h
    node.cpp
    edge.h
    edge.cpp
    )

target_link_libraries(rico-cluster-qt PRIVATE Qt5::Widgets)
