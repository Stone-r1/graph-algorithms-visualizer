cmake_minimum_required(VERSION 3.12)
project(graph CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)
set(SOURCES
    src/test.cpp
    src/board.cpp
    src/node.cpp
    src/sidebar.cpp

    src/algorithms/BFS/bfs.cpp
    src/algorithms/DFS/dfs.cpp
    src/algorithms/Dijkstra/dijkstra.cpp
    src/algorithms/Bellman-Ford/bellmanFord.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    src/algorithms
)
target_link_libraries(${PROJECT_NAME} raylib)
