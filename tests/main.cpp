#include "Inventory.h"
#include <iostream>
#include <vector>

int main() {
    Inventory inventory;
    std::vector<int> ids;

    Article gum(483, "Chewing Gum", 0.20);
    Article lollipop(736, "Lollipop", 0.49);
    Article candy(392, "Candy", 0.15);

    ids.push_back(inventory.addArticle(gum));
    ids.push_back(inventory.addArticle(lollipop));
    ids.push_back(inventory.addArticle(candy));

    std::cout << gum.toString() << std::endl;
    std::cout << lollipop.toString() << std::endl;
    std::cout << candy.toString() << std::endl;

    std::cout << "\n" << inventory.toString() << std::endl;

    for (int id : ids) {
        Article& article = inventory.getArticle(id);
        std::cout << id << " -> " << article.toString() << std::endl;
    }

    return 0;
}