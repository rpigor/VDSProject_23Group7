project(VDSProject_test CXX C)
cmake_minimum_required(VERSION 3.10)


add_executable(VDSProject_reachability main_test.cpp Reachability.cpp Reachability.h Tests.h ReachabilityInterface.h)
target_link_libraries(VDSProject_reachability Manager)
target_link_libraries(VDSProject_reachability gtest gtest_main pthread)


