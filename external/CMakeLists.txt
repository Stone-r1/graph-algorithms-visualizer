cmake_minimum_required(VERSION 3.12)
project(ExternalLibraries)

include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE      # only latest commit, speeds up a lot
)

FetchContent_MakeAvailable(raylib)
