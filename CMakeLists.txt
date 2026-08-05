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
    src/statistics.c
)

# Tell my_app where to look for headers
target_include_directories(my_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ------------------------------------------------------------------------------
# Unit Testing Setup
# ------------------------------------------------------------------------------
if(ENABLE_TESTS)
    enable_testing()

    # Add Unity library from submodule
    add_subdirectory(external/unity)

    # Unit Test Executable
    add_executable(unit_tests
        tests/test_main.c
        tests/test_statistics.c
        src/statistics.c
    )

    # Tell unit_tests where to look for headers
    target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )

    # Link Unity to your test executable
    target_link_libraries(unit_tests PRIVATE unity)

    # Register with CTest
    add_test(NAME AllTests COMMAND unit_tests)
endif()
