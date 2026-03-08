#include "Inventory.h"
#include <iostream>
#include <cassert>
#include "Exceptions.h"

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

void testSearchByName() {
    Inventory inventory;
    Article gum(483, "Chewing Gum", 0.20);
    Article candy(484, "Candy Bar", 1.50);
    Article soda(485, "Soda Can", 0.99);
    inventory.addArticle(gum);
    inventory.addArticle(candy);
    inventory.addArticle(soda);

    // Partial match
    auto results = inventory.searchByName("Gum");
    assert(results.size() == 1);
    assert(results[0]->getId() == 483);

    // Case insensitive
    results = inventory.searchByName("CANDY");
    assert(results.size() == 1);
    assert(results[0]->getId() == 484);

    // No match
    results = inventory.searchByName("Pizza");
    assert(results.empty());

    // Multiple matches
    Article gumball(486, "Gumball", 0.10);
    inventory.addArticle(gumball);
    results = inventory.searchByName("gum");
    assert(results.size() == 2);

    std::cout << "[PASS] testSearchByName" << std::endl;
}

void testInvalidPrice() {
    bool exceptionThrown = false;
    try {
        Article article(1, "Test", -5.0);
    } catch (const InvalidPriceException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    std::cout << "[PASS] testInvalidPrice" << std::endl;
}

void testInvalidName() {
    bool exceptionThrown = false;
    try {
        Article article(1, "", 1.0);
    } catch (const InvalidNameException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    std::cout << "[PASS] testInvalidName" << std::endl;
}

void testInvalidId() {
    bool exceptionThrown = false;
    try {
        Article article(-1, "Test", 5.0);
    } catch (const InvalidIdException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    std::cout << "[PASS] testInvalidId" << std::endl;
}

void testArticleNotFound() {
    Inventory inventory;

    bool exceptionThrown = false;
    try {
        inventory.updateArticle(999, "Test", 1.0);
    } catch (const ArticleNotFoundException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try {
        inventory.deleteArticle(999);
    } catch (const ArticleNotFoundException& e) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "[PASS] testArticleNotFound" << std::endl;
}

int main() {
    std::cout << "Running tests...\n" << std::endl;

    testAddArticle();
    testGetArticle();
    testDeleteArticle();
    testUpdateArticle();
    testContainsArticle();
    testSearchByName();
    testInvalidPrice();
    testInvalidName();
    testInvalidId();
    testArticleNotFound();

    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}