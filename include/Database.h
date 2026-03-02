#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <functional>
#include <sqlite3.h>

class Database {
private:
    sqlite3* db;
    std::string dbPath;

    bool executeSQL(const std::string& sql);

public:
    Database(const std::string& path = "inventory.db");
    ~Database();

    bool connect();
    void disconnect();
    bool isConnected() const;

    // Initialisation des tables
    bool initializeTables();

    // Exécution de requêtes
    bool execute(const std::string& sql);
    bool executeWithCallback(const std::string& sql,
                            int (*callback)(void*, int, char**, char**),
                            void* data);

    // Getters
    sqlite3* getHandle() const { return db; }
    int getLastInsertId() const;
    std::string getLastError() const;
};

#endif
