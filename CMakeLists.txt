cmake_minimum_required(VERSION 3.6)
project(Saniwa)

#cache value
set(QTPATH "/usr/local/opt/qt5/" CACHE STRING "QT5 location")
set(RELEASE_BUILD "OFF" CACHE BOOL "is release build")
set(ADDITIONAL_LIBRARIES "")
message(${QTPATH})
# configuration for Qt
set(CMAKE_PREFIX_PATH ${QTPATH})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# dependency
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5WebEngineWidgets REQUIRED)
FIND_PACKAGE(Qt5WebEngineCore REQUIRED)

#if UNIX like env
if(UNIX OR LINUX OR APPLE)
    set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} pthread)
endif()

#release build
set(OS_BUNDLE "")
if(RELEASE_BUILD)
    if(APPLE)
        set(OS_BUNDLE MACOSX_BUNDLE)
    endif()
endif()

#set source files
set(SOURCE_FILES winmain.cpp main.cpp Window/MainWindow.cpp Window/MainWindow.h)
add_executable(Saniwa ${OS_BUNDLE} ${SOURCE_FILES})
target_link_libraries(Saniwa Qt5::Core Qt5::Widgets Qt5::WebEngineWidgets Qt5::WebEngineCore ${ADDITIONAL_LIBRARIES})

#if MSVC
if(MSVC)
    add_definitions(-D_UNICODE)
    set_target_properties(Saniwa PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(Saniwa PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    add_custom_command(TARGET Saniwa POST_BUILD COMMAND cd $(TargetDir) && ${QTPATH}/bin/windeployqt $(TargetPath) COMMENT "Preparing Qt runtime dependencies...")
ENDIF()