add_subdirectory(test)

add_library(Manager Manager.cpp Table.cpp)
target_link_libraries(Manager cgraph gvc)
