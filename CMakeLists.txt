cmake_minimum_required(VERSION 3.20)
project(cpp_inventory_management_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver SQLite3
find_package(SQLite3 REQUIRED)

# Trouver les threads (nécessaire pour les animations)
find_package(Threads REQUIRED)

include_directories(include)

# Ancienne version (gardée pour compatibilité)
add_executable(inventory_old
        src/Article.cpp
        src/Inventory.cpp
        tests/main.cpp
)
target_include_directories(inventory_old PRIVATE include)

# Nouvelle version avec interface et base de données
add_executable(inventory
        src/Database.cpp
        src/UI.cpp
        src/InventoryManager.cpp
        tests/main_new.cpp
)
target_include_directories(inventory PRIVATE include)
target_link_libraries(inventory SQLite::SQLite3 Threads::Threads)

# Tests de l'ancienne version
add_executable(tests
        src/Article.cpp
        src/Inventory.cpp
        tests/tests.cpp
)
target_include_directories(tests PRIVATE include)
