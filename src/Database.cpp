#include "Database.h"
#include <iostream>

Database::Database(const std::string& path) : db(nullptr), dbPath(path) {}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "Erreur d'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void Database::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::isConnected() const {
    return db != nullptr;
}

bool Database::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK) {
        std::cerr << "Erreur SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::execute(const std::string& sql) {
    return executeSQL(sql);
}

bool Database::executeWithCallback(const std::string& sql,
                                   int (*callback)(void*, int, char**, char**),
                                   void* data) {
    char* errMsg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), callback, data, &errMsg);
    if (result != SQLITE_OK) {
        std::cerr << "Erreur SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int Database::getLastInsertId() const {
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

std::string Database::getLastError() const {
    return sqlite3_errmsg(db);
}

bool Database::initializeTables() {
    // Table des catégories
    std::string createCategories = R"(
        CREATE TABLE IF NOT EXISTS categories (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            description TEXT,
            color TEXT DEFAULT '#FFFFFF',
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    // Table des fournisseurs
    std::string createSuppliers = R"(
        CREATE TABLE IF NOT EXISTS suppliers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            contact_name TEXT,
            email TEXT,
            phone TEXT,
            address TEXT,
            city TEXT,
            country TEXT,
            notes TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    // Table des articles étendue
    std::string createArticles = R"(
        CREATE TABLE IF NOT EXISTS articles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT,
            barcode TEXT UNIQUE,
            price REAL NOT NULL DEFAULT 0.0,
            cost_price REAL DEFAULT 0.0,
            quantity INTEGER DEFAULT 0,
            min_quantity INTEGER DEFAULT 5,
            max_quantity INTEGER DEFAULT 1000,
            category_id INTEGER,
            supplier_id INTEGER,
            location TEXT,
            unit TEXT DEFAULT 'pièce',
            weight REAL,
            expiration_date DATE,
            is_active INTEGER DEFAULT 1,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (category_id) REFERENCES categories(id),
            FOREIGN KEY (supplier_id) REFERENCES suppliers(id)
        );
    )";

    // Table des mouvements de stock
    std::string createMovements = R"(
        CREATE TABLE IF NOT EXISTS stock_movements (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            article_id INTEGER NOT NULL,
            movement_type TEXT NOT NULL,
            quantity INTEGER NOT NULL,
            previous_quantity INTEGER,
            new_quantity INTEGER,
            reason TEXT,
            reference TEXT,
            user_name TEXT DEFAULT 'Système',
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (article_id) REFERENCES articles(id)
        );
    )";

    // Table des paramètres
    std::string createSettings = R"(
        CREATE TABLE IF NOT EXISTS settings (
            key TEXT PRIMARY KEY,
            value TEXT,
            description TEXT
        );
    )";

    // Table des alertes
    std::string createAlerts = R"(
        CREATE TABLE IF NOT EXISTS alerts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            article_id INTEGER,
            alert_type TEXT NOT NULL,
            message TEXT NOT NULL,
            is_read INTEGER DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (article_id) REFERENCES articles(id)
        );
    )";

    // Exécution des créations
    if (!executeSQL(createCategories)) return false;
    if (!executeSQL(createSuppliers)) return false;
    if (!executeSQL(createArticles)) return false;
    if (!executeSQL(createMovements)) return false;
    if (!executeSQL(createSettings)) return false;
    if (!executeSQL(createAlerts)) return false;

    // Insertion des catégories par défaut
    std::string insertDefaultCategories = R"(
        INSERT OR IGNORE INTO categories (name, description, color) VALUES
        ('Électronique', 'Appareils et composants électroniques', '#3498db'),
        ('Alimentaire', 'Produits alimentaires et boissons', '#27ae60'),
        ('Vêtements', 'Habits et accessoires vestimentaires', '#9b59b6'),
        ('Bureautique', 'Fournitures de bureau', '#f39c12'),
        ('Maison', 'Articles pour la maison', '#e74c3c'),
        ('Sport', 'Équipements sportifs', '#1abc9c'),
        ('Autre', 'Articles divers', '#95a5a6');
    )";
    executeSQL(insertDefaultCategories);

    // Paramètres par défaut
    std::string insertDefaultSettings = R"(
        INSERT OR IGNORE INTO settings (key, value, description) VALUES
        ('company_name', 'Mon Inventaire', 'Nom de l''entreprise'),
        ('currency', 'EUR', 'Devise utilisée'),
        ('low_stock_threshold', '10', 'Seuil d''alerte stock bas'),
        ('date_format', 'DD/MM/YYYY', 'Format de date'),
        ('language', 'fr', 'Langue de l''interface');
    )";
    executeSQL(insertDefaultSettings);

    return true;
}
