cmake_minimum_required(VERSION 3.29)
project(Project_Evac)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.1.4
)
FetchContent_MakeAvailable(fmt)

add_executable(Project_Evac src/main.cpp src/Game.cpp src/Game.h src/UI.cpp src/UI.h)
target_link_libraries(Project_Evac SFML::Graphics SFML::Window SFML::System fmt)

add_custom_command(TARGET Project_Evac POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Project_Evac>/assets
)