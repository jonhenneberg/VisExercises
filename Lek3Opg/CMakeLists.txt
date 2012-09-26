CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

project(VisLec3)
find_package(OpenCV REQUIRED)
add_executable(VisLec3 VisLec3.cpp)
target_link_libraries(VisLec3 ${OpenCV_LIBS})
