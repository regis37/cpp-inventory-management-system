#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include "UI.h"
#include "InventoryManager.h"

// Prototypes des fonctions
void mainMenu(InventoryManager& manager);
void articlesMenu(InventoryManager& manager);
void stockMenu(InventoryManager& manager);
void categoriesMenu(InventoryManager& manager);
void suppliersMenu(InventoryManager& manager);
void reportsMenu(InventoryManager& manager);
void alertsMenu(InventoryManager& manager);
void settingsMenu(InventoryManager& manager);

// Fonctions d'affichage
void displayDashboard(InventoryManager& manager);
void displayAllArticles(InventoryManager& manager);
void displayArticleDetails(InventoryManager& manager, int id);
void displayLowStockArticles(InventoryManager& manager);
void displayAllCategories(InventoryManager& manager);
void displayAllSuppliers(InventoryManager& manager);
void displayMovements(InventoryManager& manager, int limit = 50);
void displayAlerts(InventoryManager& manager);
void displayStatistics(InventoryManager& manager);
void displayValueByCategory(InventoryManager& manager);

// Fonctions CRUD Articles
void addNewArticle(InventoryManager& manager);
void updateExistingArticle(InventoryManager& manager);
void deleteExistingArticle(InventoryManager& manager);
void searchArticles(InventoryManager& manager);

// Fonctions Gestion du stock
void addStockEntry(InventoryManager& manager);
void removeStockEntry(InventoryManager& manager);
void adjustStockEntry(InventoryManager& manager);

// Fonctions CRUD Catégories
void addNewCategory(InventoryManager& manager);
void updateExistingCategory(InventoryManager& manager);
void deleteExistingCategory(InventoryManager& manager);

// Fonctions CRUD Fournisseurs
void addNewSupplier(InventoryManager& manager);
void updateExistingSupplier(InventoryManager& manager);
void deleteExistingSupplier(InventoryManager& manager);

// Fonctions utilitaires
void waitForKey();
int selectCategory(InventoryManager& manager);
int selectSupplier(InventoryManager& manager);

// ==================== MAIN ====================

int main() {
    InventoryManager manager;

    UI::printWelcome();
    UI::printLoadingAnimation("Initialisation de la base de données...", 1500);

    if (!manager.initialize()) {
        UI::printError("Échec de l'initialisation de la base de données!");
        return 1;
    }

    UI::printSuccess("Base de données initialisée avec succès!");
    std::cout << "\n";
    waitForKey();

    mainMenu(manager);

    UI::printGoodbye();
    return 0;
}

// ==================== MENUS ====================

void mainMenu(InventoryManager& manager) {
    while (true) {
        UI::clearScreen();
        UI::printLogo();

        // Afficher le tableau de bord rapide
        displayDashboard(manager);

        std::cout << "\n";
        UI::printHeader("MENU PRINCIPAL", 60);

        UI::printMenuItem(1, "Gestion des Articles", UI::Icon::PACKAGE);
        UI::printMenuItem(2, "Gestion du Stock", UI::Icon::CART);
        UI::printMenuItem(3, "Catégories", UI::Icon::FOLDER);
        UI::printMenuItem(4, "Fournisseurs", UI::Icon::TRUCK);
        UI::printMenuItem(5, "Rapports & Statistiques", UI::Icon::CHART);
        UI::printMenuItem(6, "Alertes", UI::Icon::ALERT);
        UI::printMenuItem(7, "Paramètres", UI::Icon::SETTINGS);
        UI::printMenuItem(0, "Quitter", "");

        UI::printFooter(60);

        int choice = UI::readMenuChoice(0, 7);

        switch (choice) {
            case 1: articlesMenu(manager); break;
            case 2: stockMenu(manager); break;
            case 3: categoriesMenu(manager); break;
            case 4: suppliersMenu(manager); break;
            case 5: reportsMenu(manager); break;
            case 6: alertsMenu(manager); break;
            case 7: settingsMenu(manager); break;
            case 0: return;
        }
    }
}

void articlesMenu(InventoryManager& manager) {
    while (true) {
        UI::clearScreen();
        UI::printHeader("GESTION DES ARTICLES", 60);

        UI::printMenuItem(1, "Voir tous les articles", UI::Icon::SEARCH);
        UI::printMenuItem(2, "Rechercher un article", UI::Icon::SEARCH);
        UI::printMenuItem(3, "Ajouter un article", UI::Icon::ARROW_RIGHT);
        UI::printMenuItem(4, "Modifier un article", UI::Icon::ARROW_RIGHT);
        UI::printMenuItem(5, "Supprimer un article", UI::Icon::CROSS);
        UI::printMenuItem(6, "Articles par catégorie", UI::Icon::FOLDER);
        UI::printMenuItem(7, "Articles en stock bas", UI::Icon::WARNING);
        UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

        UI::printFooter(60);

        int choice = UI::readMenuChoice(0, 7);

        switch (choice) {
            case 1: displayAllArticles(manager); break;
            case 2: searchArticles(manager); break;
            case 3: addNewArticle(manager); break;
            case 4: updateExistingArticle(manager); break;
            case 5: deleteExistingArticle(manager); break;
            case 6: {
                int catId = selectCategory(manager);
                if (catId > 0) {
                    UI::clearScreen();
                    auto articles = manager.getArticlesByCategory(catId);
                    CategoryData cat = manager.getCategory(catId);
                    UI::printHeader("Articles - " + cat.name, 80);

                    if (articles.empty()) {
                        UI::printInfo("Aucun article dans cette catégorie.");
                    } else {
                        std::vector<int> widths = {6, 25, 12, 10, 15};
                        UI::printTableHeader({"ID", "Nom", "Prix", "Quantité", "Fournisseur"}, widths);
                        for (const auto& art : articles) {
                            UI::printTableRow({
                                std::to_string(art.id),
                                art.name,
                                UI::formatPrice(art.price),
                                std::to_string(art.quantity),
                                art.supplierName
                            }, widths);
                        }
                        UI::printTableFooter(widths);
                    }
                    waitForKey();
                }
                break;
            }
            case 7: displayLowStockArticles(manager); break;
            case 0: return;
        }
    }
}

void stockMenu(InventoryManager& manager) {
    while (true) {
        UI::clearScreen();
        UI::printHeader("GESTION DU STOCK", 60);

        UI::printMenuItem(1, "Entrée de stock", UI::Icon::ARROW_DOWN);
        UI::printMenuItem(2, "Sortie de stock", UI::Icon::ARROW_UP);
        UI::printMenuItem(3, "Ajustement de stock", UI::Icon::SETTINGS);
        UI::printMenuItem(4, "Historique des mouvements", UI::Icon::CHART);
        UI::printMenuItem(5, "Articles en rupture", UI::Icon::WARNING);
        UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

        UI::printFooter(60);

        int choice = UI::readMenuChoice(0, 5);

        switch (choice) {
            case 1: addStockEntry(manager); break;
            case 2: removeStockEntry(manager); break;
            case 3: adjustStockEntry(manager); break;
            case 4: displayMovements(manager); break;
            case 5: displayLowStockArticles(manager); break;
            case 0: return;
        }
    }
}

void categoriesMenu(InventoryManager& manager) {
    while (true) {
        UI::clearScreen();
        UI::printHeader("GESTION DES CATÉGORIES", 60);

        UI::printMenuItem(1, "Voir toutes les catégories", UI::Icon::FOLDER);
        UI::printMenuItem(2, "Ajouter une catégorie", UI::Icon::ARROW_RIGHT);
        UI::printMenuItem(3, "Modifier une catégorie", UI::Icon::ARROW_RIGHT);
        UI::printMenuItem(4, "Supprimer une catégorie", UI::Icon::CROSS);
        UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

        UI::printFooter(60);

        int choice = UI::readMenuChoice(0, 4);

        switch (choice) {
            case 1: displayAllCategories(manager); break;
            case 2: addNewCategory(manager); break;
            case 3: updateExistingCategory(manager); break;
            case 4: deleteExistingCategory(manager); break;
            case 0: return;
        }
    }
}

void suppliersMenu(InventoryManager& manager) {
    while (true) {
        UI::clearScreen();
        UI::printHeader("GESTION DES FOURNISSEURS", 60);

        UI::printMenuItem(1, "Voir tous les fournisseurs", UI::Icon::TRUCK);
        UI::printMenuItem(2, "Ajouter un fournisseur", UI::Icon::ARROW_RIGHT);
        UI::printMenuItem(3, "Modifier un fournisseur", UI::Icon::ARROW_RIGHT);
        UI::printMenuItem(4, "Supprimer un fournisseur", UI::Icon::CROSS);
        UI::printMenuItem(5, "Rechercher un fournisseur", UI::Icon::SEARCH);
        UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

        UI::printFooter(60);

        int choice = UI::readMenuChoice(0, 5);

        switch (choice) {
            case 1: displayAllSuppliers(manager); break;
            case 2: addNewSupplier(manager); break;
            case 3: updateExistingSupplier(manager); break;
            case 4: deleteExistingSupplier(manager); break;
            case 5: {
                std::string query = UI::readString("Rechercher");
                auto suppliers = manager.searchSuppliers(query);
                UI::clearScreen();
                UI::printHeader("Résultats de recherche", 70);
                if (suppliers.empty()) {
                    UI::printInfo("Aucun fournisseur trouvé.");
                } else {
                    std::vector<int> widths = {6, 25, 20, 15};
                    UI::printTableHeader({"ID", "Nom", "Contact", "Ville"}, widths);
                    for (const auto& sup : suppliers) {
                        UI::printTableRow({
                            std::to_string(sup.id),
                            sup.name,
                            sup.contactName,
                            sup.city
                        }, widths);
                    }
                    UI::printTableFooter(widths);
                }
                waitForKey();
                break;
            }
            case 0: return;
        }
    }
}

void reportsMenu(InventoryManager& manager) {
    while (true) {
        UI::clearScreen();
        UI::printHeader("RAPPORTS & STATISTIQUES", 60);

        UI::printMenuItem(1, "Statistiques générales", UI::Icon::CHART);
        UI::printMenuItem(2, "Valeur par catégorie", UI::Icon::MONEY);
        UI::printMenuItem(3, "Articles les plus mouvementés", UI::Icon::CHART);
        UI::printMenuItem(4, "Exporter en CSV", UI::Icon::FOLDER);
        UI::printMenuItem(5, "Articles expirant bientôt", UI::Icon::WARNING);
        UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

        UI::printFooter(60);

        int choice = UI::readMenuChoice(0, 5);

        switch (choice) {
            case 1: displayStatistics(manager); break;
            case 2: displayValueByCategory(manager); break;
            case 3: {
                UI::clearScreen();
                UI::printHeader("Articles les plus mouvementés", 60);
                auto moved = manager.getMostMovedArticles(10);
                if (moved.empty()) {
                    UI::printInfo("Aucun mouvement enregistré.");
                } else {
                    std::vector<int> widths = {35, 20};
                    UI::printTableHeader({"Article", "Mouvements"}, widths);
                    for (const auto& item : moved) {
                        UI::printTableRow({item.first, std::to_string(item.second)}, widths);
                    }
                    UI::printTableFooter(widths);
                }
                waitForKey();
                break;
            }
            case 4: {
                std::string filename = UI::readString("Nom du fichier (ex: export.csv)");
                if (manager.exportToCSV(filename)) {
                    UI::printSuccess("Export réussi vers " + filename);
                } else {
                    UI::printError("Échec de l'export");
                }
                waitForKey();
                break;
            }
            case 5: {
                UI::clearScreen();
                UI::printHeader("Articles expirant dans 30 jours", 70);
                auto expiring = manager.getExpiringArticles(30);
                if (expiring.empty()) {
                    UI::printInfo("Aucun article n'expire prochainement.");
                } else {
                    std::vector<int> widths = {6, 30, 15, 15};
                    UI::printTableHeader({"ID", "Nom", "Date exp.", "Quantité"}, widths);
                    for (const auto& art : expiring) {
                        UI::printTableRow({
                            std::to_string(art.id),
                            art.name,
                            UI::formatDate(art.expirationDate),
                            std::to_string(art.quantity)
                        }, widths);
                    }
                    UI::printTableFooter(widths);
                }
                waitForKey();
                break;
            }
            case 0: return;
        }
    }
}

void alertsMenu(InventoryManager& manager) {
    UI::clearScreen();
    displayAlerts(manager);

    std::cout << "\n";
    UI::printMenuItem(1, "Marquer toutes comme lues", UI::Icon::CHECK);
    UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

    int choice = UI::readMenuChoice(0, 1);
    if (choice == 1) {
        manager.markAllAlertsAsRead();
        UI::printSuccess("Toutes les alertes ont été marquées comme lues.");
        waitForKey();
    }
}

void settingsMenu(InventoryManager& manager) {
    while (true) {
        UI::clearScreen();
        UI::printHeader("PARAMÈTRES", 60);

        std::cout << "\n";
        UI::printInfo("Nom de l'entreprise: " + manager.getSetting("company_name"));
        UI::printInfo("Devise: " + manager.getSetting("currency"));
        UI::printInfo("Seuil stock bas: " + manager.getSetting("low_stock_threshold"));
        std::cout << "\n";

        UI::printMenuItem(1, "Modifier le nom de l'entreprise", UI::Icon::SETTINGS);
        UI::printMenuItem(2, "Modifier la devise", UI::Icon::MONEY);
        UI::printMenuItem(3, "Modifier le seuil de stock bas", UI::Icon::WARNING);
        UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

        UI::printFooter(60);

        int choice = UI::readMenuChoice(0, 3);

        switch (choice) {
            case 1: {
                std::string name = UI::readString("Nouveau nom");
                manager.setSetting("company_name", name);
                UI::printSuccess("Nom modifié!");
                waitForKey();
                break;
            }
            case 2: {
                std::string currency = UI::readString("Nouvelle devise (ex: EUR, USD)");
                manager.setSetting("currency", currency);
                UI::printSuccess("Devise modifiée!");
                waitForKey();
                break;
            }
            case 3: {
                int threshold = UI::readInt("Nouveau seuil");
                manager.setSetting("low_stock_threshold", std::to_string(threshold));
                UI::printSuccess("Seuil modifié!");
                waitForKey();
                break;
            }
            case 0: return;
        }
    }
}

// ==================== FONCTIONS D'AFFICHAGE ====================

void displayDashboard(InventoryManager& manager) {
    InventoryStats stats = manager.getStatistics();

    std::cout << "\n";
    std::cout << UI::Color::DIM << "  ─────────────────────────────────────────────────────────" << UI::Color::RESET << "\n";

    // Ligne 1: Statistiques principales
    std::cout << "   " << UI::Icon::PACKAGE << " " << UI::Color::BRIGHT_WHITE << stats.totalArticles << UI::Color::RESET << " articles";
    std::cout << "   " << UI::Icon::FOLDER << " " << UI::Color::BRIGHT_WHITE << stats.totalCategories << UI::Color::RESET << " catégories";
    std::cout << "   " << UI::Icon::TRUCK << " " << UI::Color::BRIGHT_WHITE << stats.totalSuppliers << UI::Color::RESET << " fournisseurs";
    std::cout << "\n";

    // Ligne 2: Valeur et alertes
    std::cout << "   " << UI::Icon::MONEY << " Valeur: " << UI::Color::GREEN << UI::formatPrice(stats.totalValue) << UI::Color::RESET;

    if (stats.unreadAlerts > 0) {
        std::cout << "   " << UI::Color::RED << UI::Icon::ALERT << " " << stats.unreadAlerts << " alertes non lues" << UI::Color::RESET;
    }

    if (stats.lowStockCount > 0) {
        std::cout << "   " << UI::Color::YELLOW << UI::Icon::WARNING << " " << stats.lowStockCount << " stock bas" << UI::Color::RESET;
    }

    std::cout << "\n";
    std::cout << UI::Color::DIM << "  ─────────────────────────────────────────────────────────" << UI::Color::RESET << "\n";
}

void displayAllArticles(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("LISTE DES ARTICLES", 100);

    auto articles = manager.getAllArticles();

    if (articles.empty()) {
        UI::printInfo("Aucun article dans l'inventaire.");
    } else {
        std::vector<int> widths = {6, 28, 12, 10, 15, 20};
        UI::printTableHeader({"ID", "Nom", "Prix", "Stock", "Catégorie", "Fournisseur"}, widths);

        for (const auto& art : articles) {
            std::string stockStr = std::to_string(art.quantity);
            std::string stockColor = "";

            if (art.quantity == 0) {
                stockColor = UI::Color::RED;
            } else if (art.quantity <= art.minQuantity) {
                stockColor = UI::Color::YELLOW;
            } else {
                stockColor = UI::Color::GREEN;
            }

            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(std::to_string(art.id), 4) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::truncate(art.name, 26), 26) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::formatPrice(art.price), 10) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << stockColor << UI::leftAlign(stockStr, 8) << UI::Color::RESET << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::truncate(art.categoryName, 13), 13) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::truncate(art.supplierName, 18), 18) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET << "\n";
        }

        UI::printTableFooter(widths);
        std::cout << "\n" << UI::Color::DIM << "  Total: " << articles.size() << " article(s)" << UI::Color::RESET << "\n";
    }

    std::cout << "\n";
    UI::printMenuItem(1, "Voir détails d'un article", UI::Icon::SEARCH);
    UI::printMenuItem(0, "Retour", UI::Icon::ARROW_LEFT);

    int choice = UI::readMenuChoice(0, 1);
    if (choice == 1) {
        int id = UI::readInt("ID de l'article");
        displayArticleDetails(manager, id);
    }
}

void displayArticleDetails(InventoryManager& manager, int id) {
    ArticleData art = manager.getArticle(id);

    if (art.id == 0) {
        UI::printError("Article non trouvé!");
        waitForKey();
        return;
    }

    UI::clearScreen();
    UI::printHeader("DÉTAILS DE L'ARTICLE #" + std::to_string(art.id), 60);

    std::cout << "\n";
    std::cout << "  " << UI::Color::BOLD << "Informations générales" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Color::DIM << "─────────────────────────────────────" << UI::Color::RESET << "\n";
    std::cout << "  Nom:           " << UI::Color::BRIGHT_WHITE << art.name << UI::Color::RESET << "\n";
    std::cout << "  Description:   " << art.description << "\n";
    std::cout << "  Code-barres:   " << (art.barcode.empty() ? "-" : art.barcode) << "\n";
    std::cout << "  Emplacement:   " << (art.location.empty() ? "-" : art.location) << "\n";
    std::cout << "  Unité:         " << art.unit << "\n";

    std::cout << "\n";
    std::cout << "  " << UI::Color::BOLD << "Prix et stock" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Color::DIM << "─────────────────────────────────────" << UI::Color::RESET << "\n";
    std::cout << "  Prix de vente: " << UI::Color::GREEN << UI::formatPrice(art.price) << UI::Color::RESET << "\n";
    std::cout << "  Prix d'achat:  " << UI::formatPrice(art.costPrice) << "\n";
    std::cout << "  Marge:         " << UI::formatPrice(art.price - art.costPrice)
              << " (" << std::fixed << std::setprecision(1)
              << ((art.costPrice > 0) ? ((art.price - art.costPrice) / art.costPrice * 100) : 0)
              << "%)\n";

    std::string stockColor = UI::Color::GREEN;
    if (art.quantity == 0) stockColor = UI::Color::RED;
    else if (art.quantity <= art.minQuantity) stockColor = UI::Color::YELLOW;

    std::cout << "\n  Stock actuel:  " << stockColor << art.quantity << " " << art.unit << UI::Color::RESET << "  ";
    UI::printStockBar(art.quantity, art.minQuantity, art.maxQuantity, 20);
    std::cout << "\n";
    std::cout << "  Stock minimum: " << art.minQuantity << "\n";
    std::cout << "  Stock maximum: " << art.maxQuantity << "\n";

    std::cout << "\n";
    std::cout << "  " << UI::Color::BOLD << "Classification" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Color::DIM << "─────────────────────────────────────" << UI::Color::RESET << "\n";
    std::cout << "  Catégorie:     " << (art.categoryName.empty() ? "-" : art.categoryName) << "\n";
    std::cout << "  Fournisseur:   " << (art.supplierName.empty() ? "-" : art.supplierName) << "\n";

    if (!art.expirationDate.empty()) {
        std::cout << "  Date exp.:     " << UI::Color::YELLOW << UI::formatDate(art.expirationDate) << UI::Color::RESET << "\n";
    }

    std::cout << "\n";
    std::cout << "  " << UI::Color::DIM << "Créé le: " << UI::formatDate(art.createdAt)
              << " | Modifié le: " << UI::formatDate(art.updatedAt) << UI::Color::RESET << "\n";

    UI::printFooter(60);

    // Afficher les derniers mouvements
    std::cout << "\n";
    UI::printInfo("Derniers mouvements de stock:");
    auto movements = manager.getMovementsByArticle(id);
    if (movements.empty()) {
        std::cout << "  " << UI::Color::DIM << "Aucun mouvement enregistré" << UI::Color::RESET << "\n";
    } else {
        int count = 0;
        for (const auto& mov : movements) {
            if (count >= 5) break;
            std::string typeIcon = (mov.movementType == "ENTREE") ? UI::Color::GREEN + "+" : UI::Color::RED + "-";
            std::cout << "  " << typeIcon << mov.quantity << UI::Color::RESET
                      << " " << UI::Color::DIM << "(" << mov.movementType << ")"
                      << " - " << mov.reason
                      << " - " << UI::formatDate(mov.createdAt) << UI::Color::RESET << "\n";
            count++;
        }
    }

    waitForKey();
}

void displayLowStockArticles(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("ARTICLES EN STOCK BAS / RUPTURE", 80);

    auto articles = manager.getLowStockArticles();

    if (articles.empty()) {
        UI::printSuccess("Aucun article en stock bas!");
    } else {
        std::vector<int> widths = {6, 30, 12, 12, 15};
        UI::printTableHeader({"ID", "Nom", "Stock", "Minimum", "Fournisseur"}, widths);

        for (const auto& art : articles) {
            std::string status;
            std::string color;
            if (art.quantity == 0) {
                status = "RUPTURE";
                color = UI::Color::RED;
            } else {
                status = std::to_string(art.quantity);
                color = UI::Color::YELLOW;
            }

            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(std::to_string(art.id), 4) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::truncate(art.name, 28), 28) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << color << UI::leftAlign(status, 10) << UI::Color::RESET << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(std::to_string(art.minQuantity), 10) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::truncate(art.supplierName, 13), 13) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET << "\n";
        }

        UI::printTableFooter(widths);
        UI::printWarning(std::to_string(articles.size()) + " article(s) nécessitent un réapprovisionnement");
    }

    waitForKey();
}

void displayAllCategories(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("CATÉGORIES", 70);

    auto categories = manager.getAllCategories();

    if (categories.empty()) {
        UI::printInfo("Aucune catégorie définie.");
    } else {
        std::vector<int> widths = {6, 20, 30, 12};
        UI::printTableHeader({"ID", "Nom", "Description", "Articles"}, widths);

        for (const auto& cat : categories) {
            UI::printTableRow({
                std::to_string(cat.id),
                cat.name,
                cat.description,
                std::to_string(cat.articleCount)
            }, widths);
        }

        UI::printTableFooter(widths);
    }

    waitForKey();
}

void displayAllSuppliers(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("FOURNISSEURS", 90);

    auto suppliers = manager.getAllSuppliers();

    if (suppliers.empty()) {
        UI::printInfo("Aucun fournisseur enregistré.");
    } else {
        std::vector<int> widths = {6, 25, 20, 15, 15, 8};
        UI::printTableHeader({"ID", "Nom", "Contact", "Email", "Ville", "Articles"}, widths);

        for (const auto& sup : suppliers) {
            UI::printTableRow({
                std::to_string(sup.id),
                sup.name,
                sup.contactName,
                UI::truncate(sup.email, 13),
                sup.city,
                std::to_string(sup.articleCount)
            }, widths);
        }

        UI::printTableFooter(widths);
    }

    waitForKey();
}

void displayMovements(InventoryManager& manager, int limit) {
    UI::clearScreen();
    UI::printHeader("HISTORIQUE DES MOUVEMENTS", 100);

    auto movements = manager.getMovements(limit);

    if (movements.empty()) {
        UI::printInfo("Aucun mouvement enregistré.");
    } else {
        std::vector<int> widths = {8, 25, 12, 10, 25, 18};
        UI::printTableHeader({"ID", "Article", "Type", "Qté", "Raison", "Date"}, widths);

        for (const auto& mov : movements) {
            std::string typeColor;
            std::string typeStr = mov.movementType;
            if (mov.movementType == "ENTREE") {
                typeColor = UI::Color::GREEN;
            } else if (mov.movementType == "SORTIE") {
                typeColor = UI::Color::RED;
            } else {
                typeColor = UI::Color::YELLOW;
            }

            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(std::to_string(mov.id), 6) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::truncate(mov.articleName, 23), 23) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << typeColor << UI::leftAlign(typeStr, 10) << UI::Color::RESET << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(std::to_string(mov.quantity), 8) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::truncate(mov.reason, 23), 23) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET;
            std::cout << " " << UI::leftAlign(UI::formatDate(mov.createdAt), 16) << " ";
            std::cout << UI::Color::CYAN << UI::Border::VERTICAL << UI::Color::RESET << "\n";
        }

        UI::printTableFooter(widths);
    }

    waitForKey();
}

void displayAlerts(InventoryManager& manager) {
    UI::printHeader("ALERTES", 80);

    auto alerts = manager.getAlerts();

    if (alerts.empty()) {
        UI::printSuccess("Aucune alerte!");
    } else {
        for (const auto& alert : alerts) {
            std::string icon;
            std::string color;

            if (alert.alertType == "RUPTURE") {
                icon = UI::Icon::CROSS;
                color = UI::Color::RED;
            } else if (alert.alertType == "STOCK_BAS") {
                icon = UI::Icon::WARNING;
                color = UI::Color::YELLOW;
            } else if (alert.alertType == "EXPIRATION") {
                icon = UI::Icon::WARNING;
                color = UI::Color::YELLOW;
            } else {
                icon = UI::Icon::INFO;
                color = UI::Color::BLUE;
            }

            std::string readStatus = alert.isRead ? UI::Color::DIM : UI::Color::BOLD;

            std::cout << "  " << color << icon << " " << readStatus << alert.message << UI::Color::RESET;
            std::cout << UI::Color::DIM << " - " << UI::formatDate(alert.createdAt) << UI::Color::RESET << "\n";
        }
    }
}

void displayStatistics(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("STATISTIQUES GÉNÉRALES", 60);

    InventoryStats stats = manager.getStatistics();

    std::cout << "\n";
    std::cout << "  " << UI::Color::BOLD << "Vue d'ensemble" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Color::DIM << "─────────────────────────────────────" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Icon::PACKAGE << " Total articles:      " << UI::Color::BRIGHT_WHITE << stats.totalArticles << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Icon::FOLDER << " Total catégories:    " << UI::Color::BRIGHT_WHITE << stats.totalCategories << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Icon::TRUCK << " Total fournisseurs:  " << UI::Color::BRIGHT_WHITE << stats.totalSuppliers << UI::Color::RESET << "\n";
    std::cout << "  " << "   Quantité totale:    " << UI::Color::BRIGHT_WHITE << stats.totalQuantity << " unités" << UI::Color::RESET << "\n";

    std::cout << "\n";
    std::cout << "  " << UI::Color::BOLD << "Valeur de l'inventaire" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Color::DIM << "─────────────────────────────────────" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Icon::MONEY << " Valeur de vente:     " << UI::Color::GREEN << UI::formatPrice(stats.totalValue) << UI::Color::RESET << "\n";
    std::cout << "  " << "   Valeur d'achat:     " << UI::formatPrice(stats.totalCostValue) << "\n";
    std::cout << "  " << "   Profit potentiel:   " << UI::Color::GREEN << UI::formatPrice(stats.totalValue - stats.totalCostValue) << UI::Color::RESET << "\n";

    std::cout << "\n";
    std::cout << "  " << UI::Color::BOLD << "État du stock" << UI::Color::RESET << "\n";
    std::cout << "  " << UI::Color::DIM << "─────────────────────────────────────" << UI::Color::RESET << "\n";

    if (stats.outOfStockCount > 0) {
        std::cout << "  " << UI::Color::RED << UI::Icon::CROSS << " En rupture:          " << stats.outOfStockCount << " article(s)" << UI::Color::RESET << "\n";
    }
    if (stats.lowStockCount > 0) {
        std::cout << "  " << UI::Color::YELLOW << UI::Icon::WARNING << " Stock bas:           " << stats.lowStockCount << " article(s)" << UI::Color::RESET << "\n";
    }
    if (stats.expiringCount > 0) {
        std::cout << "  " << UI::Color::YELLOW << UI::Icon::WARNING << " Expirant bientôt:    " << stats.expiringCount << " article(s)" << UI::Color::RESET << "\n";
    }
    if (stats.unreadAlerts > 0) {
        std::cout << "  " << UI::Color::RED << UI::Icon::ALERT << " Alertes non lues:    " << stats.unreadAlerts << UI::Color::RESET << "\n";
    }

    if (stats.outOfStockCount == 0 && stats.lowStockCount == 0 && stats.expiringCount == 0) {
        std::cout << "  " << UI::Color::GREEN << UI::Icon::CHECK << " Tout est en ordre!" << UI::Color::RESET << "\n";
    }

    UI::printFooter(60);
    waitForKey();
}

void displayValueByCategory(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("VALEUR PAR CATÉGORIE", 60);

    auto values = manager.getValueByCategory();
    double total = 0;

    for (const auto& [name, value] : values) {
        total += value;
    }

    if (values.empty()) {
        UI::printInfo("Aucune donnée disponible.");
    } else {
        std::cout << "\n";
        for (const auto& [name, value] : values) {
            int barWidth = (total > 0) ? static_cast<int>((value / total) * 40) : 0;
            double percent = (total > 0) ? (value / total * 100) : 0;

            std::cout << "  " << UI::leftAlign(name, 20);
            std::cout << UI::Color::CYAN;
            for (int i = 0; i < barWidth; i++) std::cout << "█";
            for (int i = barWidth; i < 40; i++) std::cout << "░";
            std::cout << UI::Color::RESET;
            std::cout << " " << UI::formatPrice(value);
            std::cout << UI::Color::DIM << " (" << std::fixed << std::setprecision(1) << percent << "%)" << UI::Color::RESET;
            std::cout << "\n";
        }

        std::cout << "\n  " << UI::Color::DIM << "─────────────────────────────────────────────────────" << UI::Color::RESET << "\n";
        std::cout << "  " << UI::Color::BOLD << "TOTAL: " << UI::formatPrice(total) << UI::Color::RESET << "\n";
    }

    waitForKey();
}

// ==================== FONCTIONS CRUD ====================

void addNewArticle(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("AJOUTER UN ARTICLE", 60);

    ArticleData article;

    article.name = UI::readString("Nom de l'article");
    article.description = UI::readString("Description (optionnel)");
    article.barcode = UI::readString("Code-barres (optionnel, laisser vide pour auto)");

    if (article.barcode.empty()) {
        article.barcode = manager.generateBarcode();
        UI::printInfo("Code-barres généré: " + article.barcode);
    } else if (manager.barcodeExists(article.barcode)) {
        UI::printError("Ce code-barres existe déjà!");
        waitForKey();
        return;
    }

    article.price = UI::readDouble("Prix de vente");
    article.costPrice = UI::readDouble("Prix d'achat");
    article.quantity = UI::readInt("Quantité initiale");
    article.minQuantity = UI::readInt("Quantité minimum (alerte)");
    article.maxQuantity = UI::readInt("Quantité maximum");
    article.location = UI::readString("Emplacement (optionnel)");
    article.unit = UI::readString("Unité (ex: pièce, kg, litre)");
    if (article.unit.empty()) article.unit = "pièce";

    article.expirationDate = UI::readString("Date d'expiration YYYY-MM-DD (optionnel)");

    std::cout << "\n";
    article.categoryId = selectCategory(manager);
    article.supplierId = selectSupplier(manager);

    std::cout << "\n";
    if (UI::readConfirm("Confirmer l'ajout de cet article?")) {
        int id = manager.addArticle(article);
        if (id > 0) {
            UI::printSuccess("Article ajouté avec succès! ID: " + std::to_string(id));
        } else {
            UI::printError("Échec de l'ajout de l'article.");
        }
    } else {
        UI::printInfo("Ajout annulé.");
    }

    waitForKey();
}

void updateExistingArticle(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("MODIFIER UN ARTICLE", 60);

    int id = UI::readInt("ID de l'article à modifier");

    ArticleData article = manager.getArticle(id);
    if (article.id == 0) {
        UI::printError("Article non trouvé!");
        waitForKey();
        return;
    }

    UI::printInfo("Article actuel: " + article.name);
    std::cout << "\n";

    std::string input;

    input = UI::readString("Nouveau nom [" + article.name + "]");
    if (!input.empty()) article.name = input;

    input = UI::readString("Nouvelle description [" + article.description + "]");
    if (!input.empty()) article.description = input;

    input = UI::readString("Nouveau prix [" + UI::formatPrice(article.price) + "]");
    if (!input.empty()) article.price = std::stod(input);

    input = UI::readString("Nouveau prix d'achat [" + UI::formatPrice(article.costPrice) + "]");
    if (!input.empty()) article.costPrice = std::stod(input);

    input = UI::readString("Nouvelle quantité min [" + std::to_string(article.minQuantity) + "]");
    if (!input.empty()) article.minQuantity = std::stoi(input);

    input = UI::readString("Nouvelle quantité max [" + std::to_string(article.maxQuantity) + "]");
    if (!input.empty()) article.maxQuantity = std::stoi(input);

    input = UI::readString("Nouvel emplacement [" + article.location + "]");
    if (!input.empty()) article.location = input;

    if (UI::readConfirm("Modifier la catégorie?")) {
        article.categoryId = selectCategory(manager);
    }

    if (UI::readConfirm("Modifier le fournisseur?")) {
        article.supplierId = selectSupplier(manager);
    }

    std::cout << "\n";
    if (UI::readConfirm("Confirmer les modifications?")) {
        if (manager.updateArticle(article)) {
            UI::printSuccess("Article modifié avec succès!");
        } else {
            UI::printError("Échec de la modification.");
        }
    } else {
        UI::printInfo("Modification annulée.");
    }

    waitForKey();
}

void deleteExistingArticle(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("SUPPRIMER UN ARTICLE", 60);

    int id = UI::readInt("ID de l'article à supprimer");

    ArticleData article = manager.getArticle(id);
    if (article.id == 0) {
        UI::printError("Article non trouvé!");
        waitForKey();
        return;
    }

    UI::printWarning("Vous allez supprimer: " + article.name);
    UI::printWarning("Cette action est irréversible!");

    if (UI::readConfirm("Confirmer la suppression?")) {
        if (manager.deleteArticle(id)) {
            UI::printSuccess("Article supprimé avec succès!");
        } else {
            UI::printError("Échec de la suppression.");
        }
    } else {
        UI::printInfo("Suppression annulée.");
    }

    waitForKey();
}

void searchArticles(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("RECHERCHER UN ARTICLE", 60);

    std::string query = UI::readString("Terme de recherche");

    auto articles = manager.searchArticles(query);

    UI::clearScreen();
    UI::printHeader("Résultats pour: " + query, 80);

    if (articles.empty()) {
        UI::printInfo("Aucun article trouvé.");
    } else {
        std::vector<int> widths = {6, 30, 12, 10, 18};
        UI::printTableHeader({"ID", "Nom", "Prix", "Stock", "Catégorie"}, widths);

        for (const auto& art : articles) {
            UI::printTableRow({
                std::to_string(art.id),
                art.name,
                UI::formatPrice(art.price),
                std::to_string(art.quantity),
                art.categoryName
            }, widths);
        }

        UI::printTableFooter(widths);
        UI::printInfo(std::to_string(articles.size()) + " article(s) trouvé(s)");
    }

    waitForKey();
}

// ==================== GESTION DU STOCK ====================

void addStockEntry(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("ENTRÉE DE STOCK", 60);

    int id = UI::readInt("ID de l'article");

    ArticleData article = manager.getArticle(id);
    if (article.id == 0) {
        UI::printError("Article non trouvé!");
        waitForKey();
        return;
    }

    UI::printInfo("Article: " + article.name + " (Stock actuel: " + std::to_string(article.quantity) + ")");

    int quantity = UI::readInt("Quantité à ajouter");
    std::string reason = UI::readString("Raison (ex: Réception commande)");
    std::string reference = UI::readString("Référence (ex: BON-2024-001)");

    if (UI::readConfirm("Confirmer l'entrée de " + std::to_string(quantity) + " unités?")) {
        if (manager.addStock(id, quantity, reason, reference)) {
            UI::printSuccess("Stock ajouté! Nouveau stock: " + std::to_string(article.quantity + quantity));
        } else {
            UI::printError("Échec de l'ajout de stock.");
        }
    }

    waitForKey();
}

void removeStockEntry(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("SORTIE DE STOCK", 60);

    int id = UI::readInt("ID de l'article");

    ArticleData article = manager.getArticle(id);
    if (article.id == 0) {
        UI::printError("Article non trouvé!");
        waitForKey();
        return;
    }

    UI::printInfo("Article: " + article.name + " (Stock actuel: " + std::to_string(article.quantity) + ")");

    int quantity = UI::readInt("Quantité à retirer");

    if (quantity > article.quantity) {
        UI::printError("Stock insuffisant!");
        waitForKey();
        return;
    }

    std::string reason = UI::readString("Raison (ex: Vente, Perte)");
    std::string reference = UI::readString("Référence (ex: VENTE-2024-001)");

    if (UI::readConfirm("Confirmer la sortie de " + std::to_string(quantity) + " unités?")) {
        if (manager.removeStock(id, quantity, reason, reference)) {
            UI::printSuccess("Stock retiré! Nouveau stock: " + std::to_string(article.quantity - quantity));
        } else {
            UI::printError("Échec du retrait de stock.");
        }
    }

    waitForKey();
}

void adjustStockEntry(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("AJUSTEMENT DE STOCK", 60);

    int id = UI::readInt("ID de l'article");

    ArticleData article = manager.getArticle(id);
    if (article.id == 0) {
        UI::printError("Article non trouvé!");
        waitForKey();
        return;
    }

    UI::printInfo("Article: " + article.name + " (Stock actuel: " + std::to_string(article.quantity) + ")");

    int newQuantity = UI::readInt("Nouvelle quantité");
    std::string reason = UI::readString("Raison de l'ajustement (ex: Inventaire)");

    if (UI::readConfirm("Confirmer l'ajustement à " + std::to_string(newQuantity) + " unités?")) {
        if (manager.adjustStock(id, newQuantity, reason)) {
            UI::printSuccess("Stock ajusté!");
        } else {
            UI::printError("Échec de l'ajustement.");
        }
    }

    waitForKey();
}

// ==================== CATÉGORIES ====================

void addNewCategory(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("AJOUTER UNE CATÉGORIE", 60);

    CategoryData category;
    category.name = UI::readString("Nom de la catégorie");
    category.description = UI::readString("Description");
    category.color = UI::readString("Couleur (ex: #FF5733)");
    if (category.color.empty()) category.color = "#FFFFFF";

    if (UI::readConfirm("Confirmer l'ajout?")) {
        int id = manager.addCategory(category);
        if (id > 0) {
            UI::printSuccess("Catégorie ajoutée! ID: " + std::to_string(id));
        } else {
            UI::printError("Échec de l'ajout.");
        }
    }

    waitForKey();
}

void updateExistingCategory(InventoryManager& manager) {
    displayAllCategories(manager);

    int id = UI::readInt("ID de la catégorie à modifier");

    CategoryData category = manager.getCategory(id);
    if (category.id == 0) {
        UI::printError("Catégorie non trouvée!");
        waitForKey();
        return;
    }

    std::string input;
    input = UI::readString("Nouveau nom [" + category.name + "]");
    if (!input.empty()) category.name = input;

    input = UI::readString("Nouvelle description [" + category.description + "]");
    if (!input.empty()) category.description = input;

    if (UI::readConfirm("Confirmer les modifications?")) {
        if (manager.updateCategory(category)) {
            UI::printSuccess("Catégorie modifiée!");
        } else {
            UI::printError("Échec de la modification.");
        }
    }

    waitForKey();
}

void deleteExistingCategory(InventoryManager& manager) {
    displayAllCategories(manager);

    int id = UI::readInt("ID de la catégorie à supprimer");

    CategoryData category = manager.getCategory(id);
    if (category.id == 0) {
        UI::printError("Catégorie non trouvée!");
        waitForKey();
        return;
    }

    UI::printWarning("Catégorie: " + category.name + " (" + std::to_string(category.articleCount) + " articles)");
    UI::printWarning("Les articles seront mis sans catégorie.");

    if (UI::readConfirm("Confirmer la suppression?")) {
        if (manager.deleteCategory(id)) {
            UI::printSuccess("Catégorie supprimée!");
        } else {
            UI::printError("Échec de la suppression.");
        }
    }

    waitForKey();
}

// ==================== FOURNISSEURS ====================

void addNewSupplier(InventoryManager& manager) {
    UI::clearScreen();
    UI::printHeader("AJOUTER UN FOURNISSEUR", 60);

    SupplierData supplier;
    supplier.name = UI::readString("Nom du fournisseur");
    supplier.contactName = UI::readString("Nom du contact");
    supplier.email = UI::readString("Email");
    supplier.phone = UI::readString("Téléphone");
    supplier.address = UI::readString("Adresse");
    supplier.city = UI::readString("Ville");
    supplier.country = UI::readString("Pays");
    supplier.notes = UI::readString("Notes");

    if (UI::readConfirm("Confirmer l'ajout?")) {
        int id = manager.addSupplier(supplier);
        if (id > 0) {
            UI::printSuccess("Fournisseur ajouté! ID: " + std::to_string(id));
        } else {
            UI::printError("Échec de l'ajout.");
        }
    }

    waitForKey();
}

void updateExistingSupplier(InventoryManager& manager) {
    displayAllSuppliers(manager);

    int id = UI::readInt("ID du fournisseur à modifier");

    SupplierData supplier = manager.getSupplier(id);
    if (supplier.id == 0) {
        UI::printError("Fournisseur non trouvé!");
        waitForKey();
        return;
    }

    std::string input;
    input = UI::readString("Nouveau nom [" + supplier.name + "]");
    if (!input.empty()) supplier.name = input;

    input = UI::readString("Nouveau contact [" + supplier.contactName + "]");
    if (!input.empty()) supplier.contactName = input;

    input = UI::readString("Nouvel email [" + supplier.email + "]");
    if (!input.empty()) supplier.email = input;

    input = UI::readString("Nouveau téléphone [" + supplier.phone + "]");
    if (!input.empty()) supplier.phone = input;

    input = UI::readString("Nouvelle ville [" + supplier.city + "]");
    if (!input.empty()) supplier.city = input;

    if (UI::readConfirm("Confirmer les modifications?")) {
        if (manager.updateSupplier(supplier)) {
            UI::printSuccess("Fournisseur modifié!");
        } else {
            UI::printError("Échec de la modification.");
        }
    }

    waitForKey();
}

void deleteExistingSupplier(InventoryManager& manager) {
    displayAllSuppliers(manager);

    int id = UI::readInt("ID du fournisseur à supprimer");

    SupplierData supplier = manager.getSupplier(id);
    if (supplier.id == 0) {
        UI::printError("Fournisseur non trouvé!");
        waitForKey();
        return;
    }

    UI::printWarning("Fournisseur: " + supplier.name + " (" + std::to_string(supplier.articleCount) + " articles)");

    if (UI::readConfirm("Confirmer la suppression?")) {
        if (manager.deleteSupplier(id)) {
            UI::printSuccess("Fournisseur supprimé!");
        } else {
            UI::printError("Échec de la suppression.");
        }
    }

    waitForKey();
}

// ==================== UTILITAIRES ====================

void waitForKey() {
    std::cout << "\n" << UI::Color::DIM << "  Appuyez sur Entrée pour continuer..." << UI::Color::RESET;
    std::cin.get();
}

int selectCategory(InventoryManager& manager) {
    auto categories = manager.getAllCategories();

    if (categories.empty()) {
        UI::printInfo("Aucune catégorie disponible.");
        return 0;
    }

    std::cout << "\n  " << UI::Color::BOLD << "Sélectionner une catégorie:" << UI::Color::RESET << "\n";
    std::cout << "  [0] Aucune catégorie\n";

    for (const auto& cat : categories) {
        std::cout << "  [" << cat.id << "] " << cat.name << "\n";
    }

    return UI::readInt("ID de la catégorie");
}

int selectSupplier(InventoryManager& manager) {
    auto suppliers = manager.getAllSuppliers();

    if (suppliers.empty()) {
        UI::printInfo("Aucun fournisseur disponible.");
        return 0;
    }

    std::cout << "\n  " << UI::Color::BOLD << "Sélectionner un fournisseur:" << UI::Color::RESET << "\n";
    std::cout << "  [0] Aucun fournisseur\n";

    for (const auto& sup : suppliers) {
        std::cout << "  [" << sup.id << "] " << sup.name << " (" << sup.city << ")\n";
    }

    return UI::readInt("ID du fournisseur");
}
