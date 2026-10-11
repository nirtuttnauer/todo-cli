# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Set the project name
project(todo)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the source directory
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Find all source and header files in the /src directory
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${SRC_DIR}/*.h)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add an executable target
add_executable(todo ${SOURCES} ${HEADERS})

# Link include directories specifically to the target
target_include_directories(todo PRIVATE ${SRC_DIR})

# Set flags for debug and release configurations
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
include(FetchContent)
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
)
FetchContent_MakeAvailable(json)
target_link_libraries(todo PRIVATE nlohmann_json::nlohmann_json)

include(CTest)
if(BUILD_TESTING)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    add_test(NAME cli_regressions
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tests/test_cli.py $<TARGET_FILE:todo>)
endif()
