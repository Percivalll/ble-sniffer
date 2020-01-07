cmake_minimum_required(VERSION 3.0.0)
project(BleRffSniffer)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src srcList)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(main ${srcList})
target_link_libraries(main -lbladeRF -lpthread)
#https://www.cnblogs.com/ZY-Dream/p/11232779.html