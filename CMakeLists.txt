cmake_minimum_required(VERSION 3.5)

project(weboasis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(weboasis
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        image.h image.cpp
        bingimage.h bingimage.cpp
        images/app-development.png
        bingtext.h bingtext.cpp
        wikipedia.h wikipedia.cpp

    )
# Define target properties for Android with Qt 6 as:
    set_property(TARGET weboasis APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(weboasis SHARED
            ${PROJECT_SOURCES}
        )
    endif()
# Define properties for Android with Qt 5 after find_package() calls as:
#if(WIN32)
#    set(WIN_ICON_PATH "C:/programming/project/weboasis/images/app-development.ico")  # Replace with the actual path to your icon.ico file
#    set_target_properties(weboasis PROPERTIES WIN32_EXECUTABLE TRUE)
#    set_target_properties(weboasis PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#    set_target_properties(weboasis PROPERTIES VS_GLOBAL_ApplicationIcon "${WIN_ICON_PATH}")
#endif()

##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(weboasis
#            ${PROJECT_SOURCES}
#        )
endif()


target_link_libraries(weboasis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(weboasis PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network) # Link Network module

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.weboasis)
endif()
set_target_properties(weboasis PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS weboasis
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(weboasis)
endif()
