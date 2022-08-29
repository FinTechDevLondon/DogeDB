cmake_minimum_required(VERSION 3.21.0)
project(dogedb VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

#add_executable(mytest mytest.cpp)

add_subdirectory(std_forward)

add_subdirectory(reference_wrapper_test)


#include(CTest)
#enable_testing()

add_executable(dogedb main.cpp)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)

