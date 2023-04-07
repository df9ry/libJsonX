cmake_minimum_required(VERSION 3.9)
set (PROJECT_NAME JsonX)
project(${PROJECT_NAME} VERSION 0.3.0 DESCRIPTION "JsonX library")

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # Required for shared objects

include(CTest)
enable_testing()

set (SOURCES
    jsonx.cpp
    io.cpp)
set (HEADERS
    jsonx.hpp
    io.hpp
    scanner.hpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(JsonX PROPERTIES
    PUBLIC_HEADER jsonx.hpp)

add_custom_target(CopyConf ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/test.jsonc"
        "${CMAKE_CURRENT_BINARY_DIR}")

install(TARGETS ${PROJECT_NAME}
    LIBRARY       DESTINATION "${CMAKE_BUILD_TYPE}/lib"
    ARCHIVE       DESTINATION "${CMAKE_BUILD_TYPE}/lib"
    PUBLIC_HEADER DESTINATION "${CMAKE_BUILD_TYPE}/include")

set(APP_EXE JsonX_Test)
add_executable(${APP_EXE} test.cpp test.jsonc)
target_compile_features(${APP_EXE} PUBLIC cxx_std_20)
if (WIN32)
    target_link_libraries(${APP_EXE} ${PROJECT_NAME} stdc++)
else()
    target_link_libraries(${APP_EXE} ${PROJECT_NAME} stdc++ m)
endif()

add_test(Test ${APP_EXE})
