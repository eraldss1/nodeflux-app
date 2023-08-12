cmake_minimum_required(VERSION 3.8)

project(CrowResizeImage)

find_package(Crow CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(CrowResizeImage "main.cpp")

target_link_libraries(CrowResizeImage PUBLIC Crow::Crow ${OpenCV_LIBS})
