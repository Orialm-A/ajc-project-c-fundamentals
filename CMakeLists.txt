cmake_minimum_required(VERSION 3.16)

project(MyProject LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Build unit tests" OFF)

add_executable(my_app
    src/main.c
    src/statistics.c
    src/alertes.c
    src/releves.c
    src/affichage.c
)

target_include_directories(my_app PRIVATE
    include
)

if(ENABLE_TESTS)
    enable_testing()

    add_executable(unit_tests
        tests/test_main.c
        tests/test_statistics.c
        src/statistics.c
        external/unity/src/unity.c
    )

    target_include_directories(unit_tests PRIVATE
        include
        tests
        external/unity/src
    )

    add_test(
        NAME AllTests
        COMMAND unit_tests
    )
endif()
