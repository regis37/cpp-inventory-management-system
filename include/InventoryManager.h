#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "Database.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

// Structure pour un article complet
struct ArticleData {
    int id = 0;
    std::string name;
    std::string description;
    std::string barcode;
    double price = 0.0;
    double costPrice = 0.0;
    int quantity = 0;
    int minQuantity = 5;
    int maxQuantity = 1000;
    int categoryId = 0;
    std::string categoryName;
    int supplierId = 0;
    std::string supplierName;
    std::string location;
    std::string unit = "pièce";
    double weight = 0.0;
    std::string expirationDate;
    bool isActive = true;
    std::string createdAt;
    std::string updatedAt;
};

// Structure pour une catégorie
struct CategoryData {
    int id = 0;
    std::string name;
    std::string description;
    std::string color;
    int articleCount = 0;
};

// Structure pour un fournisseur
struct SupplierData {
    int id = 0;
    std::string name;
    std::string contactName;
    std::string email;
    std::string phone;
    std::string address;
    std::string city;
    std::string country;
    std::string notes;
    int articleCount = 0;
};

// Structure pour un mouvement de stock
struct StockMovementData {
    int id = 0;
    int articleId = 0;
    std::string articleName;
    std::string movementType; // "ENTREE", "SORTIE", "AJUSTEMENT", "INVENTAIRE"
    int quantity = 0;
    int previousQuantity = 0;
    int newQuantity = 0;
    std::string reason;
    std::string reference;
    std::string userName;
    std::string createdAt;
};

// Structure pour une alerte
struct AlertData {
    int id = 0;
    int articleId = 0;
    std::string articleName;
    std::string alertType; // "STOCK_BAS", "RUPTURE", "EXPIRATION", "SURSTOCK"
    std::string message;
    bool isRead = false;
    std::string createdAt;
};

// Structure pour les statistiques
struct InventoryStats {
    int totalArticles = 0;
    int totalCategories = 0;
    int totalSuppliers = 0;
    int lowStockCount = 0;
    int outOfStockCount = 0;
    int expiringCount = 0;
    double totalValue = 0.0;
    double totalCostValue = 0.0;
    int totalQuantity = 0;
    int unreadAlerts = 0;
};

class InventoryManager {
private:
    std::unique_ptr<Database> db;
    std::string currentUser;

    // Helpers
    void recordMovement(int articleId, const std::string& type, int quantity,
                       int prevQty, int newQty, const std::string& reason = "",
                       const std::string& reference = "");
    void checkAndCreateAlerts(int articleId);

public:
    InventoryManager(const std::string& dbPath = "inventory.db");
    ~InventoryManager();

    bool initialize();
    void setCurrentUser(const std::string& user) { currentUser = user; }

    // === GESTION DES ARTICLES ===
    int addArticle(const ArticleData& article);
    bool updateArticle(const ArticleData& article);
    bool deleteArticle(int id);
    ArticleData getArticle(int id);
    std::vector<ArticleData> getAllArticles(bool activeOnly = true);
    std::vector<ArticleData> searchArticles(const std::string& query);
    std::vector<ArticleData> getArticlesByCategory(int categoryId);
    std::vector<ArticleData> getArticlesBySupplier(int supplierId);
    std::vector<ArticleData> getLowStockArticles();
    std::vector<ArticleData> getExpiringArticles(int daysAhead = 30);

    // === GESTION DU STOCK ===
    bool addStock(int articleId, int quantity, const std::string& reason = "", const std::string& reference = "");
    bool removeStock(int articleId, int quantity, const std::string& reason = "", const std::string& reference = "");
    bool adjustStock(int articleId, int newQuantity, const std::string& reason = "");
    bool transferStock(int fromArticleId, int toArticleId, int quantity);

    // === GESTION DES CATÉGORIES ===
    int addCategory(const CategoryData& category);
    bool updateCategory(const CategoryData& category);
    bool deleteCategory(int id);
    CategoryData getCategory(int id);
    std::vector<CategoryData> getAllCategories();

    // === GESTION DES FOURNISSEURS ===
    int addSupplier(const SupplierData& supplier);
    bool updateSupplier(const SupplierData& supplier);
    bool deleteSupplier(int id);
    SupplierData getSupplier(int id);
    std::vector<SupplierData> getAllSuppliers();
    std::vector<SupplierData> searchSuppliers(const std::string& query);

    // === MOUVEMENTS DE STOCK ===
    std::vector<StockMovementData> getMovements(int limit = 100);
    std::vector<StockMovementData> getMovementsByArticle(int articleId);
    std::vector<StockMovementData> getMovementsByDate(const std::string& startDate, const std::string& endDate);

    // === ALERTES ===
    std::vector<AlertData> getAlerts(bool unreadOnly = false);
    bool markAlertAsRead(int alertId);
    bool markAllAlertsAsRead();
    void generateAlerts();

    // === STATISTIQUES ===
    InventoryStats getStatistics();
    std::map<std::string, double> getValueByCategory();
    std::map<std::string, int> getQuantityByCategory();
    std::vector<std::pair<std::string, int>> getTopSellingArticles(int limit = 10);
    std::vector<std::pair<std::string, int>> getMostMovedArticles(int limit = 10);

    // === PARAMÈTRES ===
    std::string getSetting(const std::string& key);
    bool setSetting(const std::string& key, const std::string& value);

    // === EXPORT / IMPORT ===
    bool exportToCSV(const std::string& filename);
    bool importFromCSV(const std::string& filename);

    // === UTILITAIRES ===
    bool articleExists(int id);
    bool categoryExists(int id);
    bool supplierExists(int id);
    bool barcodeExists(const std::string& barcode, int excludeId = 0);
    std::string generateBarcode();
};

#endif
