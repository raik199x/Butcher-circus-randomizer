include(CTest)

file(GLOB SRCS
    src/*.cpp
)


add_executable(test_butcher_circus ${SRCS})
add_dependencies(test_butcher_circus Butcher_Circus_Randomizer)

target_link_libraries(test_butcher_circus PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
	GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
    ${PROJECT_SOURCE_DIR}/build/CMakeFiles/Butcher_Circus_Randomizer.dir/src/randomize_rules.cpp.o
    ${PROJECT_SOURCE_DIR}/build/CMakeFiles/Butcher_Circus_Randomizer.dir/src/random_master.cpp.o
    ${PROJECT_SOURCE_DIR}/build/CMakeFiles/Butcher_Circus_Randomizer.dir/src/asset_manager.cpp.o
)

target_include_directories(test_butcher_circus PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

add_test(
    NAME App_Test
    COMMAND test_butcher_circus
)