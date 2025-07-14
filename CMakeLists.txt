cmake_minimum_required(VERSION 3.10)
project(NetworkProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(domain STATIC 
    domain.cpp
)

add_executable(network network.cpp)
target_link_libraries(network domain)

add_executable(test_client test/test_client.cpp)

if(UNIX)
    target_link_libraries(network)
    target_link_libraries(test_client)
endif()