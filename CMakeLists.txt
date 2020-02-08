cmake_minimum_required(VERSION 3.9)
project(JsonX VERSION 0.2.0 DESCRIPTION "JsonX library")

include(CTest)
enable_testing()

add_library(JsonX STATIC
    jsonx.cpp
    io.cpp)

set(APP_EXE JsonX_Test)
add_executable(${APP_EXE} test.cpp)
target_link_libraries(${APP_EXE} JsonX)

add_test(Test ${APP_EXE})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
