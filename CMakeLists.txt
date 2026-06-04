cmake_minimum_required(VERSION 4.2)
project(college_microservices)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(student-service)
add_subdirectory(class-service)
add_subdirectory(enrollment-service)