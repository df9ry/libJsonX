cmake_minimum_required(VERSION 3.9)
project(JsonX VERSION 0.2.0 DESCRIPTION "JsonX library")

include(CTest)
enable_testing()

set (SOURCES
    jsonx.cpp
    io.cpp)
set (HEADERS
    jsonx.hpp
    io.hpp
    scanner.hpp)

add_library(JsonX STATIC ${SOURCES} ${HEADERS})
target_compile_features(JsonX PUBLIC cxx_std_17)
set_target_properties(JsonX PROPERTIES
    PUBLIC_HEADER jsonx.hpp)

install(TARGETS JsonX
    LIBRARY       DESTINATION "lib"
    ARCHIVE       DESTINATION "lib"
    PUBLIC_HEADER DESTINATION "include")

set(APP_EXE JsonX_Test)
add_executable(${APP_EXE} test.cpp)
target_compile_features(${APP_EXE} PUBLIC cxx_std_17)
if (WIN32)
    target_link_libraries(${APP_EXE} JsonX)
else()
    target_link_libraries(${APP_EXE} JsonX stdc++ m)
endif()

add_test(Test ${APP_EXE})
