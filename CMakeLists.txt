cmake_minimum_required(VERSION 3.6)
project(Saniwa)

#cache value
set(QTPATH "/usr/local/opt/qt5/" CACHE STRING "QT5 location")
message(${QTPATH})
# configuration for Qt
set(CMAKE_PREFIX_PATH ${QTPATH})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# dependency
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5WebEngineWidgets REQUIRED)

set(SOURCE_FILES main.cpp Window/MainWindow.cpp Window/MainWindow.h)
add_executable(Saniwa ${SOURCE_FILES})
target_link_libraries(Saniwa Qt5::Widgets Qt5::WebEngineWidgets)