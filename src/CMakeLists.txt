add_subdirectory(test)

add_library(Manager Manager.cpp UniqueTable.cpp Node.cpp GraphRenderer.cpp)
target_link_libraries(Manager cgraph gvc)
