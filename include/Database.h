#ifndef DATABASE_H
#define DATABASE_H

#include "Article.h"
#include <sqlite3.h>
#include <map>
#include <string>

/*
 * Manages SQLite database connection.
 * Handles persistence of articles.
 */
class Database {
public:
    Database(const std::string& filename);
    ~Database();

    void createTable();
    void save(const std::map<int, Article*>& articles);
    void load(std::map<int, Article*>& articles);

private:
    sqlite3* db;
};

#endif