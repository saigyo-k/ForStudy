cmake_minimum_required(VERSION 2.8.4)
project(LOF)

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp LOF.cpp)
add_executable(LOF ${SOURCE_FILES})
target_link_libraries(LOF ${OpenCV_LIBS})