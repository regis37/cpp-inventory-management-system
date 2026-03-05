#ifndef INVENTORY_H
#define INVENTORY_H

#include "Article.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>

/*
 * Inventory class managing articles.
 * Uses std::map to associate article IDs with object references.
 */
class Inventory {

private:
    // Key = article ID
    // Value = pointer to existing Article object
    std::map<int, Article*> articles;

public:
    Article& getArticle(int id);
    const std::map<int, Article*>& getArticles() const;
    // Non-const overload — allows modification of the articles map
    std::map<int, Article*>& getArticles();

    int addArticle(Article& article);
    std::string toString() const;
    void deleteArticle(int id);
    void updateArticle(int id, const std::string& name, double price);
    //Search with id
    bool containsArticle(int id) const;
    std::vector<Article*> searchByName(const std::string& search) const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif