cmake_minimum_required(VERSION 3.12)
project(graph CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(external/raylib)

set(SOURCES
    src/main.cpp
    src/board.cpp
    src/node.cpp
    src/sidebar.cpp

    src/algorithms/BFS/bfs.cpp
    src/algorithms/DFS/dfs.cpp
    src/algorithms/Dijkstra/dijkstra.cpp
    src/algorithms/Bellman-Ford/bellmanFord.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    src/algorithms
)
