add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(verify)

add_library(Manager Manager.cpp UniqueTable.cpp Node.cpp GraphRenderer.cpp)
target_link_libraries(Manager cgraph gvc)
