#include "Inventory.h"
#include <sstream>

/*
 * Registers an article inside the inventory.
 * Stores pointer to preserve object identity.
 */
int Inventory::addArticle(Article& article) {
    articles[article.getId()] = &article;
    return article.getId();
}

/*
 * Retrieves article by ID.
 * Uses .at() to throw exception if ID does not exist.
 */
Article& Inventory::getArticle(int id) {
    return *articles.at(id);
}

/*
 * Returns formatted overview of entire inventory.
 * std::map ensures sorted order by key.
 */
std::string Inventory::toString() const {
    std::ostringstream stream;

    stream << "Inventory ID : Article\n";
    for (const auto& pair : articles) {
        stream << pair.first << " : " << pair.second->toString() << "\n";
    }

    return stream.str();
}