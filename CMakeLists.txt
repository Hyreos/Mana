cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME Mana)

project(${PROJECT_NAME})

add_library(${PROJECT_NAME})

option(MANA_IS_ASAN "Enable ASAN build" OFF)
option(MANA_BUILD_CLI "Enable command-line tool build" ON)

if (MANA_IS_ASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)

    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-asynchronous-unwind-tables -fno-exceptions)

    if (MANA_IS_ASAN)
        target_compile_options(${PROJECT_NAME} PRIVATE -O0)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
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
                                        Parser/AST/TSymbol.cc
                                        
                                        Parser/AST/SubOp.cc
                                        Parser/AST/SumOp.cc
                                        Parser/AST/MulOp.cc
                                        Parser/AST/DivOp.cc)

if (MANA_BUILD_CLI)
    add_executable(manac)

    set_target_properties(manac PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    if (MANA_IS_ASAN)
        target_compile_options(manac PRIVATE -fsanitize=address)

        target_link_options(manac PRIVATE -fsanitize=address)
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -fno-asynchronous-unwind-tables -fno-exceptions)

        if (MANA_IS_ASAN)
            target_compile_options(manac PRIVATE -O0)
        else()
            target_compile_options(manac PRIVATE -Ofast)
        endif()
    endif()

    set_target_properties(manac PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")

    target_link_libraries(manac PRIVATE ${PROJECT_NAME})

    target_include_directories(manac PRIVATE ${CMAKE_CURRENT_LIST_DIR})

    target_sources(manac PRIVATE CLI.cc)
endif()
