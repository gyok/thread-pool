cmake_minimum_required(VERSION 3.10)

project(threadPool VERSION 1.0.0 LANGUAGES CXX)

find_package (Threads)

set(THREAD_POOL_LIB_H "threadPool.h")
set(THREAD_POOL_LIB_CPP "threadPool.cpp")
add_compile_options(-pthread)

add_library(threadPoolLib STATIC ${THREAD_POOL_LIB_H} ${THREAD_POOL_LIB_CPP})
set_target_properties(threadPoolLib PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} threadPoolLib ${CMAKE_THREAD_LIBS_INIT})
