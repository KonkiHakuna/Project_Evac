cmake_minimum_required(VERSION 3.29)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

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
add_executable(Project_Evac src/main.cpp 
    src/core/Game.cpp src/core/Game.h
    src/entities/Enemy.cpp src/entities/Enemy.h
    src/entities/Player.cpp src/entities/Player.h
    src/gameplay/Inventory.cpp src/gameplay/Inventory.h
    src/gameplay/Item.cpp src/gameplay/Item.h
    src/ui/HUD.cpp src/ui/HUD.h
    src/ui/InventoryItemOptions.cpp src/ui/InventoryItemOptions.h
    src/ui/InventoryUI.cpp src/ui/InventoryUI.h
    src/ui/PauseMenu.cpp src/ui/PauseMenu.h
    src/ui/Shop.cpp src/ui/Shop.h
    src/ui/StartScreen.cpp src/ui/StartScreen.h
    src/world/Lobby.cpp src/world/Lobby.h
    src/world/Cave.cpp src/world/Cave.h
    src/save/Save.cpp src/save/Save.h
)

target_include_directories(Project_Evac PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Project_Evac SFML::Graphics SFML::Window SFML::System fmt)

add_custom_command(TARGET Project_Evac POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Project_Evac>/assets
)

