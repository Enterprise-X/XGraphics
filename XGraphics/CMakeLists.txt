cmake_minimum_required(VERSION 3.14)

project(XGraphics LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

add_library(XGraphics SHARED
  include/XGraphicsGlobal.h
  # XView/XScene/XItem/XLink
  include/XGraphicsView.h           src/XGraphicsView.cpp
  include/XGraphicsScene.h          src/XGraphicsScene.cpp
  include/XGraphicsItem.h           src/XGraphicsItem.cpp
  include/XGraphicsConnectLink.h    src/XGraphicsConnectLink.cpp
  # TextEdiT
  include/XGraphicsTextEdit.h       src/XGraphicsTextEdit.cpp

  # Static Utils
  include/XGraphicsUtils.h

  # Minidata/Factory
  include/XGraphicsDelegateFactory.h
  include/XGraphicsMimeData.h

  # XItem
  XItem/CommonUtils.h
  XItem/XGraphicsRectItem.h XItem/XGraphicsRectItem.cpp
  XItem/XGraphicsEllipseItem.h XItem/XGraphicsEllipseItem.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(XGraphics PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(XGraphics PRIVATE XGRAPHICS_LIBRARY)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    CXX_EXTENSIONS OFF
    EXPORT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
