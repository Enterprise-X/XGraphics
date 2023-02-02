cmake_minimum_required(VERSION 3.5)

project(XGraphicsApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Xml)

set(PROJECT_SOURCES
        main.cpp
        Res/AppRes.qrc

        Frm/AppMainWindow.cpp Frm/AppMainWindow.h Frm/AppMainWindow.ui
        XGraphics/AppXGraphicsFactory.cpp  XGraphics/AppXGraphicsFactory.h
        Utils/U_XmlHelper.h
)

include_directories(
    ../XGraphics/include
    ../XGraphics/XItem
    ${CMAKE_CURRENT_SOURCE_DIR}/XGraphics
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(XGraphicsApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET XGraphicsApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(XGraphicsApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(XGraphicsApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(XGraphicsApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    XGraphics)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    CXX_EXTENSIONS OFF
    EXPORT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


install(TARGETS XGraphicsApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(XGraphicsApp)
endif()
