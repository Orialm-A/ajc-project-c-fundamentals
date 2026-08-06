# Default recipe: build the main app in debug mode
default: build

# ------------------------------------------------------------------------------
# Debug App Commands
# ------------------------------------------------------------------------------

# Configure and build the main debug binary
build:
    cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=OFF
    cmake --build build/debug -j

# Clean and rebuild the debug binary
rebuild:
    rm -rf build/debug
    just build

# Run the main debug executable
run:
    ./build/debug/my_app

# ------------------------------------------------------------------------------
# Unit Test Commands
# ------------------------------------------------------------------------------

# Configure and build unit tests
utest-build:
    cmake -B build/utest -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON
    cmake --build build/utest -j

# Clean and rebuild unit tests
utest-rebuild:
    rm -rf build/utest
    just utest-build

# Run the test executable directly (or via CTest)
utest-run:
    ctest --test-dir build/utest --output-on-failure

# ------------------------------------------------------------------------------
# Utility Commands
# ------------------------------------------------------------------------------

# Remove all build directories
clean:
    rm -rf build

# Check code quality
check:
    cppcheck --enable=all --std=c11 --language=c -I include --suppress=missingIncludeSystem --error-exitcode=1 src/*.c
