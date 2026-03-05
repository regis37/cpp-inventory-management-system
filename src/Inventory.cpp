#include "Inventory.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
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

/*
 * Search articles by name (case-insensitive, partial match)
 */
std::vector<Article*> Inventory::searchByName(const std::string& search) const {
    std::vector<Article*> results;

    std::string searchLower = search;
    std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);

    for (const auto& pair : articles) {
        std::string nameLower = pair.second->getName();
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        if (nameLower.find(searchLower) != std::string::npos) {
            results.push_back(pair.second);
        }
    }

    return results;
}

void Inventory::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    file << "id,name,price\n";

    for (const auto& pair : articles) {
        file << pair.second->getId()   << ","
             << pair.second->getName() << ","
             << pair.second->getPrice()<< "\n";
    }

    file.close();
    std::cout << "Inventory saved to " << filename << std::endl;
}

void Inventory::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No save file found, starting fresh." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header line

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, name, priceStr;

        std::getline(ss, idStr,   ',');
        std::getline(ss, name,    ',');
        std::getline(ss, priceStr,',');

        int id       = std::stoi(idStr);
        double price = std::stod(priceStr);

        Article* article = new Article(id, name, price);
        articles[id] = article;
    }

    file.close();
    std::cout << "Inventory loaded from " << filename << std::endl;
}


const std::map<int, Article*>& Inventory::getArticles() const{
  return articles;
  }

std::map<int, Article*>& Inventory::getArticles() {
    return articles;
}