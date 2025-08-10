
cmake_minimum_required(VERSION 3.10)
project(2d-physics-engine)

# Diagnostic: print toolchain file
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_CXX_STANDARD 17)

# Add source files
set(SOURCES
    main.cpp
    Entity.cpp
)

# Find raylib package (vcpkg style)
find_package(raylib CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} raylib)

# Windows: Copy DLLs if needed
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:raylib>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()