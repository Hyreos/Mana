cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME Test)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME})

option(MONA_IS_ASAN "Enable ASAN build" OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-asynchronous-unwind-tables -fno-exceptions)

    if (MONA_IS_ASAN)
        target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)

        target_compile_options(${PROJECT_NAME} PRIVATE -O0 -fsanitize=address)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

target_sources(${PROJECT_NAME} PRIVATE  Start.cc 
                                        Parser/Parser.cc
                                        Parser/Lexer.cc
                                        Parser/AST/Attribute.cc
                                        Parser/AST/CField.cc
                                        Parser/AST/Component.cc
                                        Parser/AST/Number.cc
                                        Parser/AST/TreeNode.cc
                                        Parser/AST/TSymbol.cc)

