set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.5)

project(Butcher_Circus_Randomizer VERSION 3.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/raik199x/Qt/6.4.2/gcc_64")
find_package(Qt6 COMPONENTS Core Multimedia REQUIRED Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
	src/filemanip.cpp
	src/heroselection.cpp 
	src/main.cpp 
	src/mainwindow.cpp

	include/random.h 
	include/filemanip.h
	include/heroselection.h
	include/mainwindow.h
	include/config.h
	include/centralwidget.h

	forms/heroselection.ui
	forms/mainwindow.ui

	assets/resources.qrc
	assets/Butcher_Circus_Randomizer_ru_RU.ts
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
	qt_add_executable(Butcher_Circus_Randomizer
		MANUAL_FINALIZATION
		${PROJECT_SOURCES}
	)
# Define target properties for Android with Qt 6 as:
#	set_property(TARGET Butcher_Circus_Randomizer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#				 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
	if(ANDROID)
		add_library(Butcher_Circus_Randomizer SHARED
			${PROJECT_SOURCES}
		)
# Define properties for Android with Qt 5 after find_package() calls as:
#	set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
	else()
		add_executable(Butcher_Circus_Randomizer
			${PROJECT_SOURCES}
		)
	endif()
endif()

target_link_libraries(Butcher_Circus_Randomizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)
set_target_properties(Butcher_Circus_Randomizer PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS Butcher_Circus_Randomizer
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(Butcher_Circus_Randomizer)
endif()
