cmake_minimum_required(VERSION 3.16)

project(MyProject LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(ENABLE_TESTS "Build unit tests using Unity" OFF)

# ==============================================================================
# Main application
# ==============================================================================

add_executable(my_app
    src/main.c
    src/statistics.c
    src/alertes.c
    src/saisie.c
    src/affichage.c
    src/capteur.c
    src/menu_actions.c
)

target_compile_options(my_app PRIVATE
    -Wall
    -Wextra
)

target_include_directories(my_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Required for sinf()
target_link_libraries(my_app PRIVATE
    m
)

# ==============================================================================
# Unit tests
# ==============================================================================

if(ENABLE_TESTS)
    enable_testing()

    add_executable(unit_tests
        tests/test_main.c
        tests/test_statistics.c
        tests/test_random_sensor.c
        tests/test_csv_sensor.c

        src/statistics.c
        src/capteur.c

        external/unity/src/unity.c
    )

    target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
        ${CMAKE_CURRENT_SOURCE_DIR}/external/unity/src
    )

    # Required for sinf()
    target_link_libraries(unit_tests PRIVATE
        m
    )

    add_test(
        NAME AllTests
        COMMAND unit_tests
    )
endif()
