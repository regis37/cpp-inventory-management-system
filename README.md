# C++ Inventory Management System

## Overview
This project implements a simple inventory management system in C++.

Articles are stored inside an Inventory class using `std::map<int, Article*>`,
associating unique identifiers with their corresponding objects.

The focus of the project is on object-oriented design, encapsulation,
reference semantics, and modular project structure.

---

## Key Concepts Demonstrated
- Object-Oriented Programming (OOP)
- Encapsulation
- Header / implementation separation
- Usage of std::map
- Pointer-based storage
- Reference identity preservation
- Formatted output using string streams

---

## Architecture

### Article
Represents a product with:
- unique identifier
- name
- price
- formatted string representation

### Inventory
Manages registered articles:
- addArticle() → registers article
- getArticle() → retrieves article by ID
- toString() → formatted inventory overview

---

## Technical Highlights
- Identity preservation (object references remain unchanged)
- Deterministic ordering via std::map
- Clean separation between interface and implementation

---

## Build
Standard CMake project.

---

## Academic Context
Originally developed as part of an object-oriented programming exercise.
Refactored and improved for portfolio presentation.
