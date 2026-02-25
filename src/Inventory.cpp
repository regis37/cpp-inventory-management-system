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

/*
 * Deletes article corresponding to the id from inventory
 */
void Inventory::deleteArticle(int id) {
  articles.erase(id);
}

/*
 * Update article corresponding to the id from inventory
 * with new name and price
 */
void Inventory::updateArticle(int id, const std::string& name, double price) {
    if (articles.count(id)) {
        articles[id]->setName(name);
        articles[id]->setPrice(price);
    }
}

/*
 * Check if an article is present in the inventory
 */
bool Inventory::containsArticle(int id) const {
  return articles.count(id);
}