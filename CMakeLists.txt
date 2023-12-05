cmake_minimum_required(VERSION 3.15...3.20)
project("q2"
        LANGUAGES CXX
        VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor)
add_subdirectory(include)
add_subdirectory(src)
