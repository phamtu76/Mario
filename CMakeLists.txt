cmake_minimum_required(VERSION 3.16)
project(MarioGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Mặc định build Release nếu generator một cấu hình (MinGW/Ninja/Makefile).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(MarioGame
    main.cpp
    Mario.cpp
    Map.cpp
    Collision.cpp
    UI.cpp
    Game.cpp
    Config.h
    Mario.h
    Map.h
    Collision.h
    UI.h
    Game.h
)

target_link_libraries(MarioGame PRIVATE sfml-graphics sfml-window sfml-system)

if(MSVC)
    target_compile_options(MarioGame PRIVATE /W4)
else()
    target_compile_options(MarioGame PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Sao chép thư mục assets (font...) cạnh file .exe sau mỗi lần build.
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET MarioGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:MarioGame>/assets"
    )
endif()
