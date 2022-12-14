cmake_minimum_required(VERSION 3.10)



set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(COMPILE_FLAGS "-stdlib=libc++ -Wall -pedantic-errors -Werror -g -fstandalone-debug")



project(a.out.app)

file(GLOB APP_SRC_FILES ${CMAKE_SOURCE_DIR}/app/*.cpp)

set(APP_SRC_FILES_EXCEPT_MAIN ${APP_SRC_FILES})
list(REMOVE_ITEM APP_SRC_FILES_EXCEPT_MAIN ${CMAKE_SOURCE_DIR}/app/main.cpp)

add_executable(${PROJECT_NAME} ${APP_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/app)
target_link_libraries(${PROJECT_NAME} pthread c++)

project(a.out.gtest)

file(GLOB GTEST_SRC_FILES ${CMAKE_SOURCE_DIR}/gtest/*.cpp)

add_executable(${PROJECT_NAME} ${GTEST_SRC_FILES} ${APP_SRC_FILES_EXCEPT_MAIN})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/app)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/gtest)
target_link_libraries(${PROJECT_NAME} pthread c++ gtest gtest_main)

