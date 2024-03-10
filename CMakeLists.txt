cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME Test)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -O0 -fno-asynchronous-unwind-tables -fno-exceptions -fsanitize=address)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)

    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

target_sources(${PROJECT_NAME} PRIVATE Lexer.cc Start.cc)

