# C++ Inventory Management System

## Overview
This project implements a complete inventory management system in C++.

Articles are stored inside an Inventory class using `std::map<int, Article*>`,
associating unique identifiers with their corresponding objects.
Data is persisted using a SQLite database, and the system includes
a terminal user interface, input validation via custom exceptions,
and a full suite of unit tests.

The focus of the project is on object-oriented design, encapsulation,
reference semantics, and modular project structure.

---

## Key Concepts Demonstrated
- Object-Oriented Programming (OOP)
- Encapsulation with getters and setters
- Header / implementation separation
- Usage of std::map
- SQLite database persistence
- Custom exception handling
- Inheritance from C++ standard exception classes
- Formatted output using string streams
- Unit testing with assertions

---

## Architecture

### Article
Represents a product with:
- unique identifier
- name
- price
- formatted string representation
- input validation via constructor

### Inventory
Manages registered articles:
- addArticle() → registers article
- getArticle() → retrieves article by ID
- deleteArticle() → deletes article by ID
- updateArticle() → updates article name and price by ID
- containsArticle() → checks if an article exists by ID
- searchByName() → case-insensitive partial search by name
- toString() → formatted inventory overview

### Database
Manages SQLite persistence:
- Database() → opens or creates the .db file
- createTable() → initializes the articles table if it does not exist
- save() → persists all articles to the database
- load() → loads all articles from the database

### Exceptions
Custom exception classes for input validation:
- InvalidIdException → thrown when ID is negative or zero
- InvalidNameException → thrown when name is empty
- InvalidPriceException → thrown when price is negative
- ArticleNotFoundException → thrown when article ID does not exist

---

## Technical Highlights
- Identity preservation (object references remain unchanged)
- Deterministic ordering via std::map
- Clean separation between interface and implementation
- SQLite embedded database for persistent storage
- Custom exception hierarchy inheriting from std::invalid_argument
- Case-insensitive partial search using std::transform

---

## Build

### Prerequisites
- g++ with C++17 support
- CMake 3.20+
- SQLite3 (headers and static library)

### Compilation
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DSQLite3_INCLUDE_DIR=C:\sqlite3 -DSQLite3_LIBRARY=C:\sqlite3\libsqlite3.a
cmake --build .
```

### Run
```bash
.\inventory.exe   # Terminal menu
.\tests.exe       # Unit tests
```

---

## Project Structure
```
cpp-inventory-management-system/
├── src/
│   ├── Article.cpp
│   ├── Inventory.cpp
│   └── Database.cpp
├── include/
│   ├── Article.h
│   ├── Inventory.h
│   ├── Database.h
│   └── Exceptions.h
├── tests/
│   ├── main.cpp
│   └── tests.cpp
├── CMakeLists.txt
└── README.md
```

---

## Academic Context
Originally developed as part of an object-oriented programming exercise.
Independently extended with SQLite persistence, custom exception handling,
a terminal user interface, and a full unit test suite for portfolio presentation.