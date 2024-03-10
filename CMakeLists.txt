cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME Mona)

project(${PROJECT_NAME})

add_library(${PROJECT_NAME})

option(MONA_IS_ASAN "Enable ASAN build" OFF)
option(MONA_BUILD_CLI "Enable command-line tool build" ON)

if (MONA_IS_ASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)

    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-asynchronous-unwind-tables -fno-exceptions)

    if (MONA_IS_ASAN)
        target_compile_options(${PROJECT_NAME} PRIVATE -O0)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)
    endif()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

target_sources(${PROJECT_NAME} PRIVATE  Parser/Parser.cc
                                        Parser/Lexer.cc
                                        Parser/AST/Attribute.cc
                                        Parser/AST/CField.cc
                                        Parser/AST/Component.cc
                                        Parser/AST/Number.cc
                                        Parser/AST/TreeNode.cc
                                        Parser/AST/TSymbol.cc)

if (MONA_BUILD_CLI)
    add_executable(monac)

    set_target_properties(monac PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    if (MONA_IS_ASAN)
        target_compile_options(monac PRIVATE -fsanitize=address)

        target_link_options(monac PRIVATE -fsanitize=address)
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -fno-asynchronous-unwind-tables -fno-exceptions)

        if (MONA_IS_ASAN)
            target_compile_options(monac PRIVATE -O0)
        else()
            target_compile_options(monac PRIVATE -Ofast)
        endif()
    endif()

    set_target_properties(monac PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")

    target_link_libraries(monac PRIVATE ${PROJECT_NAME})

    target_include_directories(monac PRIVATE ${CMAKE_CURRENT_LIST_DIR})

    target_sources(monac PRIVATE CLI.cc)
endif()
