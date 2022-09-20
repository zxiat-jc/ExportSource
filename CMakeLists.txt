cmake_minimum_required (VERSION 3.8)

project(ExportSource VERSION 0.1 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("QT_DIR: ${QT_DIR}")
# 判断Debug
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    # 设置输出路径
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bind)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets Core5Compat)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED Core COMPONENTS Widgets Core5Compat)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_SOURCES

    ExportSource.ui
    FileUtils.h
    ExportSource.qrc

    ExportSource.h
    ExportSource.cpp
    main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ExportSource
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )    
endif()

target_link_libraries(ExportSource PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
	Qt6::Core
    #Core5Compat
	Qt6::Core5Compat
)

target_link_libraries(ExportSource PRIVATE ${qt_modules})

set_target_properties(ExportSource PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER hiyj.cn
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


target_link_libraries(ExportSource PRIVATE icu.lib)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ExportSource)
endif()