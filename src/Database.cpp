#include "Database.h"
#include <iostream>

Database::Database(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::createTable() {
    const char* query =
        "CREATE TABLE IF NOT EXISTS articles ("
        "id    INTEGER PRIMARY KEY,"
        "name  TEXT    NOT NULL,"
        "price REAL    NOT NULL"
        ");";

    sqlite3_exec(db, query, nullptr, nullptr, nullptr);
}

void Database::save(const std::map<int, Article*>& articles) {
    // Clear the table before inserting
    sqlite3_exec(db, "DELETE FROM articles", nullptr, nullptr, nullptr);

    // Prepare the query — ? are placeholders
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO articles (id, name, price) VALUES (?, ?, ?)", -1, &stmt, nullptr);

    // Loop through all articles and insert them
    for (const auto& pair : articles) {
        sqlite3_bind_int(stmt, 1, pair.second->getId());
        sqlite3_bind_text(stmt, 2, pair.second->getName().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, pair.second->getPrice());

        sqlite3_step(stmt);
        sqlite3_reset(stmt);  // Reset for next insertion
    }

    sqlite3_finalize(stmt);
}

void Database::load(std::map<int, Article*>& articles) {
    // Prepare the SELECT query
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT id, name, price FROM articles", -1, &stmt, nullptr);

    // Loop through all rows
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id          = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double price    = sqlite3_column_double(stmt, 2);

        Article* article = new Article(id, name, price);
        articles[id] = article;
    }

    sqlite3_finalize(stmt);
}