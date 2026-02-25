#include "Inventory.h"
#include <iostream>
#include <cassert>

void testAddArticle() {
    Inventory inventory;
    Article gum(483, "Chewing Gum", 0.20);
    inventory.addArticle(gum);

    assert(inventory.containsArticle(483));
    std::cout << "[PASS] testAddArticle" << std::endl;
}

void testGetArticle() {
    Inventory inventory;
    Article gum(483, "Chewing Gum", 0.20);
    inventory.addArticle(gum);

    Article& found = inventory.getArticle(483);
    assert(found.getId() == 483);
    assert(found.getName() == "Chewing Gum");
    assert(found.getPrice() == 0.20);
    std::cout << "[PASS] testGetArticle" << std::endl;
}

void testDeleteArticle() {
    Inventory inventory;
    Article gum(483, "Chewing Gum", 0.20);
    inventory.addArticle(gum);
    inventory.deleteArticle(483);

    assert(!inventory.containsArticle(483));
    std::cout << "[PASS] testDeleteArticle" << std::endl;
}

void testUpdateArticle() {
    Inventory inventory;
    Article gum(483, "Chewing Gum", 0.20);
    inventory.addArticle(gum);
    inventory.updateArticle(483, "Bubble Gum", 0.35);

    Article& updated = inventory.getArticle(483);
    assert(updated.getName() == "Bubble Gum");
    assert(updated.getPrice() == 0.35);
    std::cout << "[PASS] testUpdateArticle" << std::endl;
}

void testContainsArticle() {
    Inventory inventory;
    Article gum(483, "Chewing Gum", 0.20);
    inventory.addArticle(gum);

    assert(inventory.containsArticle(483));
    assert(!inventory.containsArticle(999));
    std::cout << "[PASS] testContainsArticle" << std::endl;
}

int main() {
    std::cout << "Running tests...\n" << std::endl;

    testAddArticle();
    testGetArticle();
    testDeleteArticle();
    testUpdateArticle();
    testContainsArticle();

    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}