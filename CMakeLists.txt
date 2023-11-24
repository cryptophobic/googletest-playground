cmake_minimum_required(VERSION 3.26)
project(gtestPractice)

set(CMAKE_CXX_STANDARD 23)

#get googletest
include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1)
FetchContent_GetProperties(googletest)
message(${googletest_POPULATED})
#googletest_POPULATED
#googletest_SOURCE_DIR
#googletest_BUILD_DIR
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
endif ()

add_executable(gtestPractice main.cpp)
add_executable(gtestPracticeV2 back.cpp)
add_executable(stack stack.cpp)
target_link_libraries(gtestPractice gtest gtest_main gmock pthread)
target_link_libraries(gtestPracticeV2 gtest gtest_main gmock pthread)
target_link_libraries(stack gtest gtest_main gmock pthread)