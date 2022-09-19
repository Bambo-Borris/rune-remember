cmake_minimum_required(VERSION 3.16)
project(rune-remember CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /permissive-)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(rune-remember WIN32)
    target_link_libraries(rune-remember PRIVATE SFML::Main)
else()
    add_executable(rune-remember)
endif()

# if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
# target_sources(rune-remember PRIVATE src/appicon.rc)
# endif()
target_compile_features(rune-remember PUBLIC cxx_std_17)

target_sources(rune-remember PRIVATE
    src/Main.cpp
    src/RuneRemember.cpp
)
target_link_libraries(rune-remember PRIVATE SFML::Graphics spdlog)
