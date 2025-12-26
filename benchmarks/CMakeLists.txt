# Configure google benchmark executables

message(STATUS "Building for benchmarking executables...")

find_package(benchmark CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)

# Get all source files except main.cpp
file(GLOB BENCH_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM BENCH_SRC_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(benchmarks bench.cpp ${BENCH_SRC_FILES})

target_link_libraries(benchmarks PRIVATE 
    benchmark::benchmark 
    benchmark::benchmark_main
    raylib
)

target_include_directories(benchmarks PRIVATE ${CMAKE_SOURCE_DIR}/src)