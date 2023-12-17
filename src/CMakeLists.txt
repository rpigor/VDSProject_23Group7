add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(verify)

add_library(Manager Manager.cpp UniqueTable.cpp Node.cpp GraphRenderer.cpp)

target_link_libraries(Manager PUBLIC cgraph gvc)

target_compile_options(Manager PRIVATE -Ofast -Wall -Wextra)