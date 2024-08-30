cmake_minimum_required(VERSION 3.17)
project(Butcher_Circus_Randomizer VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Check if the source and binary directories are the same
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR
        "In-source builds are not allowed. "
        "Please create a separate build directory and run CMake from there.\n"
    )
endif()


# In case of non-standart Qt installation, create env.cmake file and set CMAKE_PREFIX to qt installation
# example: set(CMAKE_PREFIX_PATH "~/Qt/6.7.2/gcc_64") 
include(${PROJECT_SOURCE_DIR}/env.cmake OPTIONAL)
find_package(Qt6 COMPONENTS Core Multimedia Widgets REQUIRED)

find_package(GTest CONFIG)
if(GTest_FOUND)
	enable_testing()
	add_subdirectory(test)
else()
	message(STATUS "Gtest not found. Tests will not be build.")
endif()

# Ensure automatic processing of Qt files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
	${PROJECT_SOURCE_DIR}/include/
)

# Collect all source files
file(GLOB_RECURSE PROJECT_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/assets/*.qrc"
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
	qt_add_executable(Butcher_Circus_Randomizer
		MANUAL_FINALIZATION
		${PROJECT_SOURCES}
	)
else()
	if(ANDROID)
		add_library(Butcher_Circus_Randomizer SHARED
			${PROJECT_SOURCES}
		)
	else()
		add_executable(Butcher_Circus_Randomizer
			${PROJECT_SOURCES}
		)
	endif()
endif()

target_link_libraries(Butcher_Circus_Randomizer PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia
)

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
