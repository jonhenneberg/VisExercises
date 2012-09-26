CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

project(VISLek2)
find_package(OpenCV REQUIRED)
add_executable(VISLek2 VISLek2.cpp)
target_link_libraries(VISLek2 ${OpenCV_LIBS})
