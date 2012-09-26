CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

project(VisLec4)
find_package(OpenCV REQUIRED)
add_executable(VisLec4 VisLec4.cpp)
target_link_libraries(VisLec4 ${OpenCV_LIBS})
