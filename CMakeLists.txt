cmake_minimum_required(VERSION 3.16)
project(MyProject VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# Build Options
# ------------------------------------------------------------------------------
option(ENABLE_TESTS "Build unit tests using Unity" OFF)

# ------------------------------------------------------------------------------
# Main Application Target
# ------------------------------------------------------------------------------
add_executable(my_app
    src/main.c
)

# ------------------------------------------------------------------------------
# Unit Testing Setup
# ------------------------------------------------------------------------------
if(ENABLE_TESTS)
    enable_testing()

    # Add Unity library from submodule
    add_subdirectory(external/unity)

    # Example Unit Test Executable
    add_executable(unit_tests
        tests/test_main.c
        # Add additional source files needed by your tests here
    )

    # Link Unity to your test executable
    target_link_libraries(unit_tests PRIVATE unity)

    # Register with CTest so `ctest` or `just utest-run` can execute it
    add_test(NAME AllTests COMMAND unit_tests)
endif()
