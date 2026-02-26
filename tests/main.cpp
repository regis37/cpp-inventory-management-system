#include "Inventory.h"
#include <iostream>
#include <limits>

void displayMenu() {
    std::cout << "\n=== Inventory Management ===" << std::endl;
    std::cout << "1. Display all articles"      << std::endl;
    std::cout << "2. Add an article"            << std::endl;
    std::cout << "3. Update an article"         << std::endl;
    std::cout << "4. Delete an article"         << std::endl;
    std::cout << "0. Exit"                      << std::endl;
    std::cout << "Choice: ";
}

void displayAll(Inventory& inventory) {
    std::cout << "\n" << inventory.toString();
}

void addArticle(Inventory& inventory) {
    int id;
    std::string name;
    double price;

    std::cout << "ID: ";
    std::cin >> id;
    std::cin.ignore();

    std::cout << "Name: ";
    std::getline(std::cin, name);

    std::cout << "Price: ";
    std::cin >> price;

    if (inventory.containsArticle(id)) {
        std::cout << "Error: ID " << id << " already exists." << std::endl;
        return;
    }

    Article* article = new Article(id, name, price);
    inventory.addArticle(*article);
    std::cout << "Article added successfully." << std::endl;
}

void updateArticle(Inventory& inventory) {
    int id;
    std::string name;
    double price;

    std::cout << "ID to update: ";
    std::cin >> id;
    std::cin.ignore();

    if (!inventory.containsArticle(id)) {
        std::cout << "Error: ID " << id << " not found." << std::endl;
        return;
    }

    std::cout << "New name: ";
    std::getline(std::cin, name);

    std::cout << "New price: ";
    std::cin >> price;

    inventory.updateArticle(id, name, price);
    std::cout << "Article updated successfully." << std::endl;
}

void deleteArticle(Inventory& inventory) {
    int id;

    std::cout << "ID to delete: ";
    std::cin >> id;

    if (!inventory.containsArticle(id)) {
        std::cout << "Error: ID " << id << " not found." << std::endl;
        return;
    }

    inventory.deleteArticle(id);
    std::cout << "Article deleted successfully." << std::endl;
}

int main() {
    Inventory inventory;
    int choice;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: displayAll(inventory);      break;
            case 2: addArticle(inventory);      break;
            case 3: updateArticle(inventory);   break;
            case 4: deleteArticle(inventory);   break;
            case 0: std::cout << "Goodbye!" << std::endl; break;
            default: std::cout << "Invalid choice." << std::endl;
        }
    } while (choice != 0);

    return 0;
}