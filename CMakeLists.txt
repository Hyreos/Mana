cmake_minimum_required(VERSION 3.28)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(PROJECT_NAME Test)

project(${PROJECT_NAME})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-O0 -fno-asynchronous-unwind-tables -fno-exceptions -fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

target_sources(${PROJECT_NAME} PRIVATE Lexer.cc Start.cc)

