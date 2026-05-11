find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GoogleTest not found. Tests will not be built.")
    message(STATUS "Install GoogleTest or set GTest_DIR to enable tests.")
    return()
endif()

# Test helper function
function(add_knoten_test test_name)
    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name}
        PRIVATE
            miniset
            GTest::GTest
            GTest::Main
    )
    gtest_discover_tests(${test_name})
endfunction()

# All tests in flat structure
add_knoten_test(test_math test_math.cpp)
add_knoten_test(test_ellipsoid test_ellipsoid.cpp)
add_knoten_test(test_dem test_dem.cpp)
add_knoten_test(test_csm_interface test_csm_interface.cpp)
add_knoten_test(test_angles test_angles.cpp)
add_knoten_test(test_coordinate_transforms test_coordinate_transforms.cpp)
