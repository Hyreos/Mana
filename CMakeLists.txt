cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME Mana)

project(${PROJECT_NAME})

option(MANA_IS_ASAN "Enable ASAN build." OFF)
option(MANA_IS_VERBOSE "Enable verbose mode." OFF)
option(MANA_BUILD_CLI "Enable command-line tool build." ON)
option(MANA_IS_SHARED "Build Mana as a shared library." OFF)
option(MANA_USE_PIC "Use -fPIC when building MANA." OFF)

add_compile_options(-g)
add_link_options(-g)

if (MANA_USE_PIC)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (NOT WIN32)
            add_compile_options(-fPIC)
        endif()
    endif()
endif()

if (MANA_IS_SHARED)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if (MANA_IS_SHARED)
    add_library(${PROJECT_NAME} SHARED)
else()
    add_library(${PROJECT_NAME} STATIC)
endif()

if (MANA_IS_SHARED)
    if (WIN32)
        set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
endif()

if (MANA_IS_ASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)

    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-asynchronous-unwind-tables -fno-exceptions)

    if (MANA_IS_ASAN OR CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -O0)
    endif()
endif()

set(ABSL_FIND_GOOGLETEST OFF)
set(ABSL_PROPAGATE_CXX_STD ON)

if (WIN32)
    add_compile_definitions(_DISABLE_STRING_ANNOTATION=1 _DISABLE_VECTOR_ANNOTATION=1)
endif()

add_subdirectory(ThirdParty/abseil)

target_link_libraries(${PROJECT_NAME} PRIVATE absl::strings)

if (MANA_IS_ASAN)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        message(FATAL_ERROR "Clang ASAN builds are not supported in debug mode.")
    endif()

    if (MANA_IS_SHARED)
        if (WIN32)
            if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                message(FATAL_ERROR "Windows dynamic ASAN builds are not supported on clang.")
            endif()
        endif()
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}
                                                    ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/abseil)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

if (MANA_IS_VERBOSE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE MANA_IS_VERBOSE)
endif()

target_sources(${PROJECT_NAME} PRIVATE  Parser/Parser.cc
                                        Parser/Lexer.cc
                                        Parser/AST/CCAttribute.cc
                                        Parser/AST/SerializeAttribute.cc
                                        Parser/AST/BinaryExpression.cc
                                        Parser/AST/BoolLiteralExpression.cc
                                        Parser/AST/ComponentDeclaration.cc
                                        Parser/AST/FloatLiteralExpression.cc
                                        Parser/AST/GroupExpression.cc
                                        Parser/AST/IdentifierExpression.cc
                                        Parser/AST/ImportDeclaration.cc
                                        Parser/AST/IntegerLiteralExpression.cc
                                        Parser/AST/MemberDeclaration.cc
                                        Parser/AST/Qualifier.cc
                                        Parser/AST/StringLiteralExpression.cc
                                        Parser/AST/TreeNode.cc
                                        Parser/AST/TypeDeclaration.cc
                                        Parser/AST/UnaryExpression.cc
                                        Utils/RTTI.cc)

if (MANA_BUILD_CLI)
    add_executable(manac)

    set_target_properties(manac PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    if (MANA_IS_ASAN)
        target_compile_options(manac PRIVATE -fsanitize=address)

        target_link_options(manac PRIVATE -fsanitize=address)
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -fno-asynchronous-unwind-tables -fno-exceptions)

        target_compile_options(manac PRIVATE -O0)
    endif()

    target_link_libraries(manac PRIVATE ${PROJECT_NAME})

    target_include_directories(manac PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/abseil)

    target_sources(manac PRIVATE CLI.cc)
endif()
