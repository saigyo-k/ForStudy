cmake_minimum_required(VERSION 3.1)
project(ColorEdgeDetection)

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(ColorEdgeDetection ${SOURCE_FILES} ColorEdge.cpp ColorEdge.h)
target_link_libraries(ColorEdgeDetection ${OpenCV_LIBS})