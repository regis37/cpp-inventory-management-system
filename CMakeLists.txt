cmake_minimum_required(VERSION 3.20)
project(cpp_inventory_management_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(inventory
        src/Article.cpp
        src/Inventory.cpp
        tests/main.cpp
)