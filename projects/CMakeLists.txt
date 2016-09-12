message(STATUS "Entering directory projects/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(images)

# now images target is adding into ALL target
add_custom_target(images)
add_custom_target(wine)
add_dependencies(wine images)

add_subdirectory(01_day)
add_subdirectory(02_day)
add_subdirectory(03_day)
