#include "InventoryManager.h"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <random>

InventoryManager::InventoryManager(const std::string& dbPath)
    : db(std::make_unique<Database>(dbPath)), currentUser("Système") {}

InventoryManager::~InventoryManager() {}

bool InventoryManager::initialize() {
    if (!db->connect()) return false;
    if (!db->initializeTables()) return false;
    generateAlerts();
    return true;
}

// ==================== ARTICLES ====================

int InventoryManager::addArticle(const ArticleData& article) {
    std::ostringstream sql;
    sql << "INSERT INTO articles (name, description, barcode, price, cost_price, "
        << "quantity, min_quantity, max_quantity, category_id, supplier_id, "
        << "location, unit, weight, expiration_date, is_active) VALUES ("
        << "'" << article.name << "', "
        << "'" << article.description << "', "
        << (article.barcode.empty() ? "NULL" : "'" + article.barcode + "'") << ", "
        << article.price << ", "
        << article.costPrice << ", "
        << article.quantity << ", "
        << article.minQuantity << ", "
        << article.maxQuantity << ", "
        << (article.categoryId > 0 ? std::to_string(article.categoryId) : "NULL") << ", "
        << (article.supplierId > 0 ? std::to_string(article.supplierId) : "NULL") << ", "
        << "'" << article.location << "', "
        << "'" << article.unit << "', "
        << article.weight << ", "
        << (article.expirationDate.empty() ? "NULL" : "'" + article.expirationDate + "'") << ", "
        << (article.isActive ? 1 : 0) << ");";

    if (db->execute(sql.str())) {
        int newId = db->getLastInsertId();
        if (article.quantity > 0) {
            recordMovement(newId, "ENTREE", article.quantity, 0, article.quantity, "Stock initial");
        }
        checkAndCreateAlerts(newId);
        return newId;
    }
    return -1;
}

bool InventoryManager::updateArticle(const ArticleData& article) {
    std::ostringstream sql;
    sql << "UPDATE articles SET "
        << "name = '" << article.name << "', "
        << "description = '" << article.description << "', "
        << "barcode = " << (article.barcode.empty() ? "NULL" : "'" + article.barcode + "'") << ", "
        << "price = " << article.price << ", "
        << "cost_price = " << article.costPrice << ", "
        << "min_quantity = " << article.minQuantity << ", "
        << "max_quantity = " << article.maxQuantity << ", "
        << "category_id = " << (article.categoryId > 0 ? std::to_string(article.categoryId) : "NULL") << ", "
        << "supplier_id = " << (article.supplierId > 0 ? std::to_string(article.supplierId) : "NULL") << ", "
        << "location = '" << article.location << "', "
        << "unit = '" << article.unit << "', "
        << "weight = " << article.weight << ", "
        << "expiration_date = " << (article.expirationDate.empty() ? "NULL" : "'" + article.expirationDate + "'") << ", "
        << "is_active = " << (article.isActive ? 1 : 0) << ", "
        << "updated_at = CURRENT_TIMESTAMP "
        << "WHERE id = " << article.id << ";";

    return db->execute(sql.str());
}

bool InventoryManager::deleteArticle(int id) {
    std::string sql = "DELETE FROM articles WHERE id = " + std::to_string(id) + ";";
    return db->execute(sql);
}

ArticleData InventoryManager::getArticle(int id) {
    ArticleData article;
    std::string sql = "SELECT a.*, c.name as category_name, s.name as supplier_name "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "LEFT JOIN suppliers s ON a.supplier_id = s.id "
                      "WHERE a.id = " + std::to_string(id) + ";";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        ArticleData* art = static_cast<ArticleData*>(data);
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") art->id = std::stoi(val);
            else if (col == "name") art->name = val;
            else if (col == "description") art->description = val;
            else if (col == "barcode") art->barcode = val;
            else if (col == "price") art->price = std::stod(val);
            else if (col == "cost_price") art->costPrice = std::stod(val);
            else if (col == "quantity") art->quantity = std::stoi(val);
            else if (col == "min_quantity") art->minQuantity = std::stoi(val);
            else if (col == "max_quantity") art->maxQuantity = std::stoi(val);
            else if (col == "category_id" && !val.empty()) art->categoryId = std::stoi(val);
            else if (col == "category_name") art->categoryName = val;
            else if (col == "supplier_id" && !val.empty()) art->supplierId = std::stoi(val);
            else if (col == "supplier_name") art->supplierName = val;
            else if (col == "location") art->location = val;
            else if (col == "unit") art->unit = val;
            else if (col == "weight" && !val.empty()) art->weight = std::stod(val);
            else if (col == "expiration_date") art->expirationDate = val;
            else if (col == "is_active") art->isActive = (val == "1");
            else if (col == "created_at") art->createdAt = val;
            else if (col == "updated_at") art->updatedAt = val;
        }
        return 0;
    };

    db->executeWithCallback(sql, callback, &article);
    return article;
}

std::vector<ArticleData> InventoryManager::getAllArticles(bool activeOnly) {
    std::vector<ArticleData> articles;
    std::string sql = "SELECT a.*, c.name as category_name, s.name as supplier_name "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "LEFT JOIN suppliers s ON a.supplier_id = s.id ";
    if (activeOnly) sql += "WHERE a.is_active = 1 ";
    sql += "ORDER BY a.name;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<ArticleData>* list = static_cast<std::vector<ArticleData>*>(data);
        ArticleData art;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") art.id = std::stoi(val);
            else if (col == "name") art.name = val;
            else if (col == "description") art.description = val;
            else if (col == "barcode") art.barcode = val;
            else if (col == "price" && !val.empty()) art.price = std::stod(val);
            else if (col == "cost_price" && !val.empty()) art.costPrice = std::stod(val);
            else if (col == "quantity" && !val.empty()) art.quantity = std::stoi(val);
            else if (col == "min_quantity" && !val.empty()) art.minQuantity = std::stoi(val);
            else if (col == "max_quantity" && !val.empty()) art.maxQuantity = std::stoi(val);
            else if (col == "category_id" && !val.empty()) art.categoryId = std::stoi(val);
            else if (col == "category_name") art.categoryName = val;
            else if (col == "supplier_id" && !val.empty()) art.supplierId = std::stoi(val);
            else if (col == "supplier_name") art.supplierName = val;
            else if (col == "location") art.location = val;
            else if (col == "unit") art.unit = val;
            else if (col == "weight" && !val.empty()) art.weight = std::stod(val);
            else if (col == "expiration_date") art.expirationDate = val;
            else if (col == "is_active") art.isActive = (val == "1");
            else if (col == "created_at") art.createdAt = val;
            else if (col == "updated_at") art.updatedAt = val;
        }
        list->push_back(art);
        return 0;
    };

    db->executeWithCallback(sql, callback, &articles);
    return articles;
}

std::vector<ArticleData> InventoryManager::searchArticles(const std::string& query) {
    std::vector<ArticleData> articles;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    std::string sql = "SELECT a.*, c.name as category_name, s.name as supplier_name "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "LEFT JOIN suppliers s ON a.supplier_id = s.id "
                      "WHERE LOWER(a.name) LIKE '%" + lowerQuery + "%' "
                      "OR LOWER(a.description) LIKE '%" + lowerQuery + "%' "
                      "OR a.barcode LIKE '%" + query + "%' "
                      "OR LOWER(a.location) LIKE '%" + lowerQuery + "%' "
                      "ORDER BY a.name;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<ArticleData>* list = static_cast<std::vector<ArticleData>*>(data);
        ArticleData art;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") art.id = std::stoi(val);
            else if (col == "name") art.name = val;
            else if (col == "description") art.description = val;
            else if (col == "barcode") art.barcode = val;
            else if (col == "price" && !val.empty()) art.price = std::stod(val);
            else if (col == "cost_price" && !val.empty()) art.costPrice = std::stod(val);
            else if (col == "quantity" && !val.empty()) art.quantity = std::stoi(val);
            else if (col == "min_quantity" && !val.empty()) art.minQuantity = std::stoi(val);
            else if (col == "max_quantity" && !val.empty()) art.maxQuantity = std::stoi(val);
            else if (col == "category_id" && !val.empty()) art.categoryId = std::stoi(val);
            else if (col == "category_name") art.categoryName = val;
            else if (col == "supplier_id" && !val.empty()) art.supplierId = std::stoi(val);
            else if (col == "supplier_name") art.supplierName = val;
            else if (col == "location") art.location = val;
            else if (col == "unit") art.unit = val;
            else if (col == "is_active") art.isActive = (val == "1");
        }
        list->push_back(art);
        return 0;
    };

    db->executeWithCallback(sql, callback, &articles);
    return articles;
}

std::vector<ArticleData> InventoryManager::getArticlesByCategory(int categoryId) {
    std::vector<ArticleData> articles;
    std::string sql = "SELECT a.*, c.name as category_name, s.name as supplier_name "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "LEFT JOIN suppliers s ON a.supplier_id = s.id "
                      "WHERE a.category_id = " + std::to_string(categoryId) + " "
                      "AND a.is_active = 1 ORDER BY a.name;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<ArticleData>* list = static_cast<std::vector<ArticleData>*>(data);
        ArticleData art;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") art.id = std::stoi(val);
            else if (col == "name") art.name = val;
            else if (col == "price" && !val.empty()) art.price = std::stod(val);
            else if (col == "quantity" && !val.empty()) art.quantity = std::stoi(val);
            else if (col == "category_name") art.categoryName = val;
            else if (col == "supplier_name") art.supplierName = val;
            else if (col == "unit") art.unit = val;
        }
        list->push_back(art);
        return 0;
    };

    db->executeWithCallback(sql, callback, &articles);
    return articles;
}

std::vector<ArticleData> InventoryManager::getArticlesBySupplier(int supplierId) {
    std::vector<ArticleData> articles;
    std::string sql = "SELECT a.*, c.name as category_name, s.name as supplier_name "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "LEFT JOIN suppliers s ON a.supplier_id = s.id "
                      "WHERE a.supplier_id = " + std::to_string(supplierId) + " "
                      "AND a.is_active = 1 ORDER BY a.name;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<ArticleData>* list = static_cast<std::vector<ArticleData>*>(data);
        ArticleData art;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") art.id = std::stoi(val);
            else if (col == "name") art.name = val;
            else if (col == "price" && !val.empty()) art.price = std::stod(val);
            else if (col == "quantity" && !val.empty()) art.quantity = std::stoi(val);
            else if (col == "category_name") art.categoryName = val;
            else if (col == "supplier_name") art.supplierName = val;
        }
        list->push_back(art);
        return 0;
    };

    db->executeWithCallback(sql, callback, &articles);
    return articles;
}

std::vector<ArticleData> InventoryManager::getLowStockArticles() {
    std::vector<ArticleData> articles;
    std::string sql = "SELECT a.*, c.name as category_name, s.name as supplier_name "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "LEFT JOIN suppliers s ON a.supplier_id = s.id "
                      "WHERE a.quantity <= a.min_quantity AND a.is_active = 1 "
                      "ORDER BY a.quantity ASC;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<ArticleData>* list = static_cast<std::vector<ArticleData>*>(data);
        ArticleData art;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") art.id = std::stoi(val);
            else if (col == "name") art.name = val;
            else if (col == "quantity" && !val.empty()) art.quantity = std::stoi(val);
            else if (col == "min_quantity" && !val.empty()) art.minQuantity = std::stoi(val);
            else if (col == "category_name") art.categoryName = val;
            else if (col == "supplier_name") art.supplierName = val;
        }
        list->push_back(art);
        return 0;
    };

    db->executeWithCallback(sql, callback, &articles);
    return articles;
}

std::vector<ArticleData> InventoryManager::getExpiringArticles(int daysAhead) {
    std::vector<ArticleData> articles;
    std::string sql = "SELECT a.*, c.name as category_name "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "WHERE a.expiration_date IS NOT NULL "
                      "AND a.expiration_date <= date('now', '+" + std::to_string(daysAhead) + " days') "
                      "AND a.is_active = 1 "
                      "ORDER BY a.expiration_date ASC;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<ArticleData>* list = static_cast<std::vector<ArticleData>*>(data);
        ArticleData art;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") art.id = std::stoi(val);
            else if (col == "name") art.name = val;
            else if (col == "expiration_date") art.expirationDate = val;
            else if (col == "quantity" && !val.empty()) art.quantity = std::stoi(val);
            else if (col == "category_name") art.categoryName = val;
        }
        list->push_back(art);
        return 0;
    };

    db->executeWithCallback(sql, callback, &articles);
    return articles;
}

// ==================== GESTION DU STOCK ====================

bool InventoryManager::addStock(int articleId, int quantity, const std::string& reason, const std::string& reference) {
    ArticleData article = getArticle(articleId);
    if (article.id == 0) return false;

    int newQty = article.quantity + quantity;
    std::string sql = "UPDATE articles SET quantity = " + std::to_string(newQty) +
                      ", updated_at = CURRENT_TIMESTAMP WHERE id = " + std::to_string(articleId) + ";";

    if (db->execute(sql)) {
        recordMovement(articleId, "ENTREE", quantity, article.quantity, newQty, reason, reference);
        checkAndCreateAlerts(articleId);
        return true;
    }
    return false;
}

bool InventoryManager::removeStock(int articleId, int quantity, const std::string& reason, const std::string& reference) {
    ArticleData article = getArticle(articleId);
    if (article.id == 0 || article.quantity < quantity) return false;

    int newQty = article.quantity - quantity;
    std::string sql = "UPDATE articles SET quantity = " + std::to_string(newQty) +
                      ", updated_at = CURRENT_TIMESTAMP WHERE id = " + std::to_string(articleId) + ";";

    if (db->execute(sql)) {
        recordMovement(articleId, "SORTIE", quantity, article.quantity, newQty, reason, reference);
        checkAndCreateAlerts(articleId);
        return true;
    }
    return false;
}

bool InventoryManager::adjustStock(int articleId, int newQuantity, const std::string& reason) {
    ArticleData article = getArticle(articleId);
    if (article.id == 0) return false;

    std::string sql = "UPDATE articles SET quantity = " + std::to_string(newQuantity) +
                      ", updated_at = CURRENT_TIMESTAMP WHERE id = " + std::to_string(articleId) + ";";

    if (db->execute(sql)) {
        int diff = newQuantity - article.quantity;
        recordMovement(articleId, "AJUSTEMENT", std::abs(diff), article.quantity, newQuantity, reason);
        checkAndCreateAlerts(articleId);
        return true;
    }
    return false;
}

bool InventoryManager::transferStock(int fromArticleId, int toArticleId, int quantity) {
    if (!removeStock(fromArticleId, quantity, "Transfert vers article #" + std::to_string(toArticleId))) {
        return false;
    }
    if (!addStock(toArticleId, quantity, "Transfert depuis article #" + std::to_string(fromArticleId))) {
        // Rollback
        addStock(fromArticleId, quantity, "Annulation transfert");
        return false;
    }
    return true;
}

void InventoryManager::recordMovement(int articleId, const std::string& type, int quantity,
                                      int prevQty, int newQty, const std::string& reason,
                                      const std::string& reference) {
    std::ostringstream sql;
    sql << "INSERT INTO stock_movements (article_id, movement_type, quantity, "
        << "previous_quantity, new_quantity, reason, reference, user_name) VALUES ("
        << articleId << ", '" << type << "', " << quantity << ", "
        << prevQty << ", " << newQty << ", '" << reason << "', '"
        << reference << "', '" << currentUser << "');";
    db->execute(sql.str());
}

// ==================== CATÉGORIES ====================

int InventoryManager::addCategory(const CategoryData& category) {
    std::ostringstream sql;
    sql << "INSERT INTO categories (name, description, color) VALUES ("
        << "'" << category.name << "', "
        << "'" << category.description << "', "
        << "'" << category.color << "');";

    if (db->execute(sql.str())) {
        return db->getLastInsertId();
    }
    return -1;
}

bool InventoryManager::updateCategory(const CategoryData& category) {
    std::ostringstream sql;
    sql << "UPDATE categories SET "
        << "name = '" << category.name << "', "
        << "description = '" << category.description << "', "
        << "color = '" << category.color << "' "
        << "WHERE id = " << category.id << ";";
    return db->execute(sql.str());
}

bool InventoryManager::deleteCategory(int id) {
    // D'abord, mettre les articles de cette catégorie à NULL
    std::string updateArticles = "UPDATE articles SET category_id = NULL WHERE category_id = " + std::to_string(id) + ";";
    db->execute(updateArticles);

    std::string sql = "DELETE FROM categories WHERE id = " + std::to_string(id) + ";";
    return db->execute(sql);
}

CategoryData InventoryManager::getCategory(int id) {
    CategoryData category;
    std::string sql = "SELECT c.*, COUNT(a.id) as article_count "
                      "FROM categories c "
                      "LEFT JOIN articles a ON c.id = a.category_id AND a.is_active = 1 "
                      "WHERE c.id = " + std::to_string(id) + " GROUP BY c.id;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        CategoryData* cat = static_cast<CategoryData*>(data);
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") cat->id = std::stoi(val);
            else if (col == "name") cat->name = val;
            else if (col == "description") cat->description = val;
            else if (col == "color") cat->color = val;
            else if (col == "article_count" && !val.empty()) cat->articleCount = std::stoi(val);
        }
        return 0;
    };

    db->executeWithCallback(sql, callback, &category);
    return category;
}

std::vector<CategoryData> InventoryManager::getAllCategories() {
    std::vector<CategoryData> categories;
    std::string sql = "SELECT c.*, COUNT(a.id) as article_count "
                      "FROM categories c "
                      "LEFT JOIN articles a ON c.id = a.category_id AND a.is_active = 1 "
                      "GROUP BY c.id ORDER BY c.name;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<CategoryData>* list = static_cast<std::vector<CategoryData>*>(data);
        CategoryData cat;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") cat.id = std::stoi(val);
            else if (col == "name") cat.name = val;
            else if (col == "description") cat.description = val;
            else if (col == "color") cat.color = val;
            else if (col == "article_count" && !val.empty()) cat.articleCount = std::stoi(val);
        }
        list->push_back(cat);
        return 0;
    };

    db->executeWithCallback(sql, callback, &categories);
    return categories;
}

// ==================== FOURNISSEURS ====================

int InventoryManager::addSupplier(const SupplierData& supplier) {
    std::ostringstream sql;
    sql << "INSERT INTO suppliers (name, contact_name, email, phone, address, city, country, notes) VALUES ("
        << "'" << supplier.name << "', "
        << "'" << supplier.contactName << "', "
        << "'" << supplier.email << "', "
        << "'" << supplier.phone << "', "
        << "'" << supplier.address << "', "
        << "'" << supplier.city << "', "
        << "'" << supplier.country << "', "
        << "'" << supplier.notes << "');";

    if (db->execute(sql.str())) {
        return db->getLastInsertId();
    }
    return -1;
}

bool InventoryManager::updateSupplier(const SupplierData& supplier) {
    std::ostringstream sql;
    sql << "UPDATE suppliers SET "
        << "name = '" << supplier.name << "', "
        << "contact_name = '" << supplier.contactName << "', "
        << "email = '" << supplier.email << "', "
        << "phone = '" << supplier.phone << "', "
        << "address = '" << supplier.address << "', "
        << "city = '" << supplier.city << "', "
        << "country = '" << supplier.country << "', "
        << "notes = '" << supplier.notes << "' "
        << "WHERE id = " << supplier.id << ";";
    return db->execute(sql.str());
}

bool InventoryManager::deleteSupplier(int id) {
    std::string updateArticles = "UPDATE articles SET supplier_id = NULL WHERE supplier_id = " + std::to_string(id) + ";";
    db->execute(updateArticles);

    std::string sql = "DELETE FROM suppliers WHERE id = " + std::to_string(id) + ";";
    return db->execute(sql);
}

SupplierData InventoryManager::getSupplier(int id) {
    SupplierData supplier;
    std::string sql = "SELECT s.*, COUNT(a.id) as article_count "
                      "FROM suppliers s "
                      "LEFT JOIN articles a ON s.id = a.supplier_id AND a.is_active = 1 "
                      "WHERE s.id = " + std::to_string(id) + " GROUP BY s.id;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        SupplierData* sup = static_cast<SupplierData*>(data);
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") sup->id = std::stoi(val);
            else if (col == "name") sup->name = val;
            else if (col == "contact_name") sup->contactName = val;
            else if (col == "email") sup->email = val;
            else if (col == "phone") sup->phone = val;
            else if (col == "address") sup->address = val;
            else if (col == "city") sup->city = val;
            else if (col == "country") sup->country = val;
            else if (col == "notes") sup->notes = val;
            else if (col == "article_count" && !val.empty()) sup->articleCount = std::stoi(val);
        }
        return 0;
    };

    db->executeWithCallback(sql, callback, &supplier);
    return supplier;
}

std::vector<SupplierData> InventoryManager::getAllSuppliers() {
    std::vector<SupplierData> suppliers;
    std::string sql = "SELECT s.*, COUNT(a.id) as article_count "
                      "FROM suppliers s "
                      "LEFT JOIN articles a ON s.id = a.supplier_id AND a.is_active = 1 "
                      "GROUP BY s.id ORDER BY s.name;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<SupplierData>* list = static_cast<std::vector<SupplierData>*>(data);
        SupplierData sup;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") sup.id = std::stoi(val);
            else if (col == "name") sup.name = val;
            else if (col == "contact_name") sup.contactName = val;
            else if (col == "email") sup.email = val;
            else if (col == "phone") sup.phone = val;
            else if (col == "city") sup.city = val;
            else if (col == "article_count" && !val.empty()) sup.articleCount = std::stoi(val);
        }
        list->push_back(sup);
        return 0;
    };

    db->executeWithCallback(sql, callback, &suppliers);
    return suppliers;
}

std::vector<SupplierData> InventoryManager::searchSuppliers(const std::string& query) {
    std::vector<SupplierData> suppliers;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    std::string sql = "SELECT s.*, COUNT(a.id) as article_count "
                      "FROM suppliers s "
                      "LEFT JOIN articles a ON s.id = a.supplier_id "
                      "WHERE LOWER(s.name) LIKE '%" + lowerQuery + "%' "
                      "OR LOWER(s.city) LIKE '%" + lowerQuery + "%' "
                      "OR LOWER(s.contact_name) LIKE '%" + lowerQuery + "%' "
                      "GROUP BY s.id ORDER BY s.name;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<SupplierData>* list = static_cast<std::vector<SupplierData>*>(data);
        SupplierData sup;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") sup.id = std::stoi(val);
            else if (col == "name") sup.name = val;
            else if (col == "contact_name") sup.contactName = val;
            else if (col == "email") sup.email = val;
            else if (col == "phone") sup.phone = val;
            else if (col == "city") sup.city = val;
            else if (col == "article_count" && !val.empty()) sup.articleCount = std::stoi(val);
        }
        list->push_back(sup);
        return 0;
    };

    db->executeWithCallback(sql, callback, &suppliers);
    return suppliers;
}

// ==================== MOUVEMENTS ====================

std::vector<StockMovementData> InventoryManager::getMovements(int limit) {
    std::vector<StockMovementData> movements;
    std::string sql = "SELECT m.*, a.name as article_name "
                      "FROM stock_movements m "
                      "JOIN articles a ON m.article_id = a.id "
                      "ORDER BY m.created_at DESC LIMIT " + std::to_string(limit) + ";";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<StockMovementData>* list = static_cast<std::vector<StockMovementData>*>(data);
        StockMovementData mov;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") mov.id = std::stoi(val);
            else if (col == "article_id") mov.articleId = std::stoi(val);
            else if (col == "article_name") mov.articleName = val;
            else if (col == "movement_type") mov.movementType = val;
            else if (col == "quantity" && !val.empty()) mov.quantity = std::stoi(val);
            else if (col == "previous_quantity" && !val.empty()) mov.previousQuantity = std::stoi(val);
            else if (col == "new_quantity" && !val.empty()) mov.newQuantity = std::stoi(val);
            else if (col == "reason") mov.reason = val;
            else if (col == "reference") mov.reference = val;
            else if (col == "user_name") mov.userName = val;
            else if (col == "created_at") mov.createdAt = val;
        }
        list->push_back(mov);
        return 0;
    };

    db->executeWithCallback(sql, callback, &movements);
    return movements;
}

std::vector<StockMovementData> InventoryManager::getMovementsByArticle(int articleId) {
    std::vector<StockMovementData> movements;
    std::string sql = "SELECT m.*, a.name as article_name "
                      "FROM stock_movements m "
                      "JOIN articles a ON m.article_id = a.id "
                      "WHERE m.article_id = " + std::to_string(articleId) + " "
                      "ORDER BY m.created_at DESC;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<StockMovementData>* list = static_cast<std::vector<StockMovementData>*>(data);
        StockMovementData mov;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") mov.id = std::stoi(val);
            else if (col == "article_id") mov.articleId = std::stoi(val);
            else if (col == "article_name") mov.articleName = val;
            else if (col == "movement_type") mov.movementType = val;
            else if (col == "quantity" && !val.empty()) mov.quantity = std::stoi(val);
            else if (col == "previous_quantity" && !val.empty()) mov.previousQuantity = std::stoi(val);
            else if (col == "new_quantity" && !val.empty()) mov.newQuantity = std::stoi(val);
            else if (col == "reason") mov.reason = val;
            else if (col == "created_at") mov.createdAt = val;
        }
        list->push_back(mov);
        return 0;
    };

    db->executeWithCallback(sql, callback, &movements);
    return movements;
}

std::vector<StockMovementData> InventoryManager::getMovementsByDate(const std::string& startDate, const std::string& endDate) {
    std::vector<StockMovementData> movements;
    std::string sql = "SELECT m.*, a.name as article_name "
                      "FROM stock_movements m "
                      "JOIN articles a ON m.article_id = a.id "
                      "WHERE date(m.created_at) BETWEEN '" + startDate + "' AND '" + endDate + "' "
                      "ORDER BY m.created_at DESC;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<StockMovementData>* list = static_cast<std::vector<StockMovementData>*>(data);
        StockMovementData mov;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") mov.id = std::stoi(val);
            else if (col == "article_name") mov.articleName = val;
            else if (col == "movement_type") mov.movementType = val;
            else if (col == "quantity" && !val.empty()) mov.quantity = std::stoi(val);
            else if (col == "previous_quantity" && !val.empty()) mov.previousQuantity = std::stoi(val);
            else if (col == "new_quantity" && !val.empty()) mov.newQuantity = std::stoi(val);
            else if (col == "created_at") mov.createdAt = val;
        }
        list->push_back(mov);
        return 0;
    };

    db->executeWithCallback(sql, callback, &movements);
    return movements;
}

// ==================== ALERTES ====================

void InventoryManager::checkAndCreateAlerts(int articleId) {
    ArticleData article = getArticle(articleId);
    if (article.id == 0) return;

    // Alerte stock bas
    if (article.quantity <= article.minQuantity && article.quantity > 0) {
        std::string sql = "INSERT INTO alerts (article_id, alert_type, message) "
                          "SELECT " + std::to_string(articleId) + ", 'STOCK_BAS', "
                          "'Stock bas pour " + article.name + ": " + std::to_string(article.quantity) + " restants' "
                          "WHERE NOT EXISTS (SELECT 1 FROM alerts WHERE article_id = " + std::to_string(articleId) +
                          " AND alert_type = 'STOCK_BAS' AND is_read = 0);";
        db->execute(sql);
    }

    // Alerte rupture de stock
    if (article.quantity == 0) {
        std::string sql = "INSERT INTO alerts (article_id, alert_type, message) "
                          "SELECT " + std::to_string(articleId) + ", 'RUPTURE', "
                          "'Rupture de stock pour " + article.name + "!' "
                          "WHERE NOT EXISTS (SELECT 1 FROM alerts WHERE article_id = " + std::to_string(articleId) +
                          " AND alert_type = 'RUPTURE' AND is_read = 0);";
        db->execute(sql);
    }

    // Alerte surstock
    if (article.quantity > article.maxQuantity) {
        std::string sql = "INSERT INTO alerts (article_id, alert_type, message) "
                          "SELECT " + std::to_string(articleId) + ", 'SURSTOCK', "
                          "'Surstock pour " + article.name + ": " + std::to_string(article.quantity) + " (max: " +
                          std::to_string(article.maxQuantity) + ")' "
                          "WHERE NOT EXISTS (SELECT 1 FROM alerts WHERE article_id = " + std::to_string(articleId) +
                          " AND alert_type = 'SURSTOCK' AND is_read = 0);";
        db->execute(sql);
    }
}

std::vector<AlertData> InventoryManager::getAlerts(bool unreadOnly) {
    std::vector<AlertData> alerts;
    std::string sql = "SELECT al.*, a.name as article_name "
                      "FROM alerts al "
                      "LEFT JOIN articles a ON al.article_id = a.id ";
    if (unreadOnly) sql += "WHERE al.is_read = 0 ";
    sql += "ORDER BY al.created_at DESC;";

    auto callback = [](void* data, int argc, char** argv, char** colNames) -> int {
        std::vector<AlertData>* list = static_cast<std::vector<AlertData>*>(data);
        AlertData alert;
        for (int i = 0; i < argc; i++) {
            std::string col = colNames[i];
            std::string val = argv[i] ? argv[i] : "";
            if (col == "id") alert.id = std::stoi(val);
            else if (col == "article_id" && !val.empty()) alert.articleId = std::stoi(val);
            else if (col == "article_name") alert.articleName = val;
            else if (col == "alert_type") alert.alertType = val;
            else if (col == "message") alert.message = val;
            else if (col == "is_read") alert.isRead = (val == "1");
            else if (col == "created_at") alert.createdAt = val;
        }
        list->push_back(alert);
        return 0;
    };

    db->executeWithCallback(sql, callback, &alerts);
    return alerts;
}

bool InventoryManager::markAlertAsRead(int alertId) {
    std::string sql = "UPDATE alerts SET is_read = 1 WHERE id = " + std::to_string(alertId) + ";";
    return db->execute(sql);
}

bool InventoryManager::markAllAlertsAsRead() {
    std::string sql = "UPDATE alerts SET is_read = 1 WHERE is_read = 0;";
    return db->execute(sql);
}

void InventoryManager::generateAlerts() {
    // Vérifier tous les articles pour générer des alertes
    std::vector<ArticleData> articles = getAllArticles();
    for (const auto& article : articles) {
        checkAndCreateAlerts(article.id);
    }

    // Alertes d'expiration
    std::string sql = "INSERT INTO alerts (article_id, alert_type, message) "
                      "SELECT id, 'EXPIRATION', "
                      "'Article \"' || name || '\" expire le ' || expiration_date "
                      "FROM articles "
                      "WHERE expiration_date IS NOT NULL "
                      "AND expiration_date <= date('now', '+30 days') "
                      "AND is_active = 1 "
                      "AND id NOT IN (SELECT article_id FROM alerts WHERE alert_type = 'EXPIRATION' AND is_read = 0);";
    db->execute(sql);
}

// ==================== STATISTIQUES ====================

InventoryStats InventoryManager::getStatistics() {
    InventoryStats stats;

    // Total articles
    std::string sql = "SELECT COUNT(*) as count FROM articles WHERE is_active = 1;";
    auto countCallback = [](void* data, int, char** argv, char**) -> int {
        int* count = static_cast<int*>(data);
        *count = argv[0] ? std::stoi(argv[0]) : 0;
        return 0;
    };
    db->executeWithCallback(sql, countCallback, &stats.totalArticles);

    // Total catégories
    sql = "SELECT COUNT(*) as count FROM categories;";
    db->executeWithCallback(sql, countCallback, &stats.totalCategories);

    // Total fournisseurs
    sql = "SELECT COUNT(*) as count FROM suppliers;";
    db->executeWithCallback(sql, countCallback, &stats.totalSuppliers);

    // Articles en stock bas
    sql = "SELECT COUNT(*) as count FROM articles WHERE quantity <= min_quantity AND quantity > 0 AND is_active = 1;";
    db->executeWithCallback(sql, countCallback, &stats.lowStockCount);

    // Articles en rupture
    sql = "SELECT COUNT(*) as count FROM articles WHERE quantity = 0 AND is_active = 1;";
    db->executeWithCallback(sql, countCallback, &stats.outOfStockCount);

    // Articles expirant bientôt
    sql = "SELECT COUNT(*) as count FROM articles WHERE expiration_date IS NOT NULL "
          "AND expiration_date <= date('now', '+30 days') AND is_active = 1;";
    db->executeWithCallback(sql, countCallback, &stats.expiringCount);

    // Alertes non lues
    sql = "SELECT COUNT(*) as count FROM alerts WHERE is_read = 0;";
    db->executeWithCallback(sql, countCallback, &stats.unreadAlerts);

    // Valeur totale et quantité totale
    sql = "SELECT SUM(quantity * price) as total_value, SUM(quantity * cost_price) as total_cost, "
          "SUM(quantity) as total_qty FROM articles WHERE is_active = 1;";
    auto valueCallback = [](void* data, int, char** argv, char**) -> int {
        InventoryStats* s = static_cast<InventoryStats*>(data);
        s->totalValue = argv[0] ? std::stod(argv[0]) : 0.0;
        s->totalCostValue = argv[1] ? std::stod(argv[1]) : 0.0;
        s->totalQuantity = argv[2] ? std::stoi(argv[2]) : 0;
        return 0;
    };
    db->executeWithCallback(sql, valueCallback, &stats);

    return stats;
}

std::map<std::string, double> InventoryManager::getValueByCategory() {
    std::map<std::string, double> values;
    std::string sql = "SELECT COALESCE(c.name, 'Sans catégorie') as category, "
                      "SUM(a.quantity * a.price) as total_value "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "WHERE a.is_active = 1 "
                      "GROUP BY c.id ORDER BY total_value DESC;";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        std::map<std::string, double>* map = static_cast<std::map<std::string, double>*>(data);
        if (argc >= 2 && argv[0] && argv[1]) {
            (*map)[argv[0]] = std::stod(argv[1]);
        }
        return 0;
    };

    db->executeWithCallback(sql, callback, &values);
    return values;
}

std::map<std::string, int> InventoryManager::getQuantityByCategory() {
    std::map<std::string, int> quantities;
    std::string sql = "SELECT COALESCE(c.name, 'Sans catégorie') as category, "
                      "SUM(a.quantity) as total_qty "
                      "FROM articles a "
                      "LEFT JOIN categories c ON a.category_id = c.id "
                      "WHERE a.is_active = 1 "
                      "GROUP BY c.id ORDER BY total_qty DESC;";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        std::map<std::string, int>* map = static_cast<std::map<std::string, int>*>(data);
        if (argc >= 2 && argv[0] && argv[1]) {
            (*map)[argv[0]] = std::stoi(argv[1]);
        }
        return 0;
    };

    db->executeWithCallback(sql, callback, &quantities);
    return quantities;
}

std::vector<std::pair<std::string, int>> InventoryManager::getMostMovedArticles(int limit) {
    std::vector<std::pair<std::string, int>> results;
    std::string sql = "SELECT a.name, COUNT(m.id) as movement_count "
                      "FROM stock_movements m "
                      "JOIN articles a ON m.article_id = a.id "
                      "GROUP BY m.article_id "
                      "ORDER BY movement_count DESC LIMIT " + std::to_string(limit) + ";";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        std::vector<std::pair<std::string, int>>* list = static_cast<std::vector<std::pair<std::string, int>>*>(data);
        if (argc >= 2 && argv[0] && argv[1]) {
            list->push_back({argv[0], std::stoi(argv[1])});
        }
        return 0;
    };

    db->executeWithCallback(sql, callback, &results);
    return results;
}

std::vector<std::pair<std::string, int>> InventoryManager::getTopSellingArticles(int limit) {
    std::vector<std::pair<std::string, int>> results;
    std::string sql = "SELECT a.name, SUM(m.quantity) as total_out "
                      "FROM stock_movements m "
                      "JOIN articles a ON m.article_id = a.id "
                      "WHERE m.movement_type = 'SORTIE' "
                      "GROUP BY m.article_id "
                      "ORDER BY total_out DESC LIMIT " + std::to_string(limit) + ";";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        std::vector<std::pair<std::string, int>>* list = static_cast<std::vector<std::pair<std::string, int>>*>(data);
        if (argc >= 2 && argv[0] && argv[1]) {
            list->push_back({argv[0], std::stoi(argv[1])});
        }
        return 0;
    };

    db->executeWithCallback(sql, callback, &results);
    return results;
}

// ==================== PARAMÈTRES ====================

std::string InventoryManager::getSetting(const std::string& key) {
    std::string value;
    std::string sql = "SELECT value FROM settings WHERE key = '" + key + "';";

    auto callback = [](void* data, int, char** argv, char**) -> int {
        std::string* val = static_cast<std::string*>(data);
        if (argv[0]) *val = argv[0];
        return 0;
    };

    db->executeWithCallback(sql, callback, &value);
    return value;
}

bool InventoryManager::setSetting(const std::string& key, const std::string& value) {
    std::string sql = "INSERT OR REPLACE INTO settings (key, value) VALUES ('" + key + "', '" + value + "');";
    return db->execute(sql);
}

// ==================== EXPORT/IMPORT ====================

bool InventoryManager::exportToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    // En-têtes
    file << "ID,Nom,Description,Code-barres,Prix,Prix d'achat,Quantité,Catégorie,Fournisseur,Emplacement,Unité\n";

    std::vector<ArticleData> articles = getAllArticles(false);
    for (const auto& art : articles) {
        file << art.id << ","
             << "\"" << art.name << "\","
             << "\"" << art.description << "\","
             << art.barcode << ","
             << art.price << ","
             << art.costPrice << ","
             << art.quantity << ","
             << "\"" << art.categoryName << "\","
             << "\"" << art.supplierName << "\","
             << "\"" << art.location << "\","
             << "\"" << art.unit << "\"\n";
    }

    file.close();
    return true;
}

bool InventoryManager::importFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        // Parsing CSV basique - à améliorer pour des cas complexes
        std::stringstream ss(line);
        ArticleData article;
        std::string cell;

        std::getline(ss, cell, ',');
        // ID ignoré pour import
        std::getline(ss, article.name, ',');
        std::getline(ss, article.description, ',');
        std::getline(ss, article.barcode, ',');
        std::getline(ss, cell, ','); article.price = std::stod(cell);
        std::getline(ss, cell, ','); article.costPrice = std::stod(cell);
        std::getline(ss, cell, ','); article.quantity = std::stoi(cell);

        // Nettoyage des guillemets
        article.name.erase(std::remove(article.name.begin(), article.name.end(), '"'), article.name.end());
        article.description.erase(std::remove(article.description.begin(), article.description.end(), '"'), article.description.end());

        addArticle(article);
    }

    file.close();
    return true;
}

// ==================== UTILITAIRES ====================

bool InventoryManager::articleExists(int id) {
    int count = 0;
    std::string sql = "SELECT COUNT(*) FROM articles WHERE id = " + std::to_string(id) + ";";
    auto callback = [](void* data, int, char** argv, char**) -> int {
        int* c = static_cast<int*>(data);
        *c = argv[0] ? std::stoi(argv[0]) : 0;
        return 0;
    };
    db->executeWithCallback(sql, callback, &count);
    return count > 0;
}

bool InventoryManager::categoryExists(int id) {
    int count = 0;
    std::string sql = "SELECT COUNT(*) FROM categories WHERE id = " + std::to_string(id) + ";";
    auto callback = [](void* data, int, char** argv, char**) -> int {
        int* c = static_cast<int*>(data);
        *c = argv[0] ? std::stoi(argv[0]) : 0;
        return 0;
    };
    db->executeWithCallback(sql, callback, &count);
    return count > 0;
}

bool InventoryManager::supplierExists(int id) {
    int count = 0;
    std::string sql = "SELECT COUNT(*) FROM suppliers WHERE id = " + std::to_string(id) + ";";
    auto callback = [](void* data, int, char** argv, char**) -> int {
        int* c = static_cast<int*>(data);
        *c = argv[0] ? std::stoi(argv[0]) : 0;
        return 0;
    };
    db->executeWithCallback(sql, callback, &count);
    return count > 0;
}

bool InventoryManager::barcodeExists(const std::string& barcode, int excludeId) {
    if (barcode.empty()) return false;
    int count = 0;
    std::string sql = "SELECT COUNT(*) FROM articles WHERE barcode = '" + barcode + "'";
    if (excludeId > 0) sql += " AND id != " + std::to_string(excludeId);
    sql += ";";
    auto callback = [](void* data, int, char** argv, char**) -> int {
        int* c = static_cast<int*>(data);
        *c = argv[0] ? std::stoi(argv[0]) : 0;
        return 0;
    };
    db->executeWithCallback(sql, callback, &count);
    return count > 0;
}

std::string InventoryManager::generateBarcode() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string barcode;
    for (int i = 0; i < 13; i++) {
        barcode += std::to_string(dis(gen));
    }

    // Vérifier l'unicité
    while (barcodeExists(barcode)) {
        barcode.clear();
        for (int i = 0; i < 13; i++) {
            barcode += std::to_string(dis(gen));
        }
    }

    return barcode;
}
