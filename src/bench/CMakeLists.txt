project(VDSProject_verify CXX C)
cmake_minimum_required(VERSION 3.10)

add_library(Benchmark
        BenchParser.cpp
        BenchmarkLib.cpp
        CircuitToBDD.cpp
        bench_grammar.hpp
        skip_parser.hpp)

#Boost
#find_package(Boost)

#Executable
add_executable(VDSProject_bench main_bench.cpp)
target_link_libraries(VDSProject_bench Manager)
target_link_libraries(VDSProject_bench Benchmark)
#target_link_libraries(VDSProject_bench ${Boost_LIBRARIES})
