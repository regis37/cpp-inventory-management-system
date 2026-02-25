#ifndef INVENTORY_H
#define INVENTORY_H

#include "Article.h"
#include <map>
#include <string>

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
    int addArticle(Article& article);
    Article& getArticle(int id);
    std::string toString() const;
};

#endif