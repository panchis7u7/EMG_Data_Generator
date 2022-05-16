cmake_minimum_required(VERSION 3.18.4)

project(EMG_Data_Parser VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_subdirectory(src)
#add_subdirectory(libs)
#add_subdirectory(tests)

