cmake_minimum_required(VERSION 3.20)
project(cpp_inventory_management_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SQLite3 REQUIRED)

include_directories(include)

add_executable(inventory
        src/Article.cpp
        src/Inventory.cpp
        src/Database.cpp
        tests/main.cpp
)

add_executable(tests
        src/Article.cpp
        src/Inventory.cpp
        tests/tests.cpp
)

target_include_directories(inventory PRIVATE include)
target_include_directories(tests PRIVATE include)
target_link_libraries(inventory SQLite3::SQLite3)