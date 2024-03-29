cmake_minimum_required(VERSION 3.5)

project(punchin VERSION 0.1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.h mainwindow.cpp mainwindow.ui
        addstudentwindow.h addstudentwindow.cpp addstudentwindow.ui
        addcoursewindow.h addcoursewindow.cpp addcoursewindow.ui
        database/studentDB.cpp
        database/courseDB.cpp
        email_curl/email.cpp
        PN532/PN532_lib/pn532.cpp
        PN532/PN532_lib/pn532_rpi.cpp
        PN532/rfid.cpp
        doorlock/doorlock.cpp
        AS608/fingerprint.cpp
        AS608/AS608_lib/utils.c
        AS608/AS608_lib/as608.c
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)
find_package(SQLite3 REQUIRED)  


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(punchin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET punchin APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(punchin SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(punchin
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(punchin PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql sqlite3 wiringPi)

set_target_properties(punchin PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS punchin
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(punchin)
endif()
