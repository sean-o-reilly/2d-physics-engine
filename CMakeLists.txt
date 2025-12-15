cmake_minimum_required(VERSION 3.15)
project(2d-physics-engine)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(/W4 /permissive- /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

find_package(raylib CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(benchmarks)