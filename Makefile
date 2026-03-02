# Makefile pour le Système de Gestion d'Inventaire
# ================================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
LDFLAGS = -lsqlite3 -pthread

# Répertoires
SRC_DIR = src
TEST_DIR = tests
INCLUDE_DIR = include
BUILD_DIR = build

# Fichiers sources pour la nouvelle version
NEW_SOURCES = $(SRC_DIR)/Database.cpp \
              $(SRC_DIR)/UI.cpp \
              $(SRC_DIR)/InventoryManager.cpp \
              $(TEST_DIR)/main_new.cpp

# Fichiers sources pour l'ancienne version
OLD_SOURCES = $(SRC_DIR)/Article.cpp \
              $(SRC_DIR)/Inventory.cpp \
              $(TEST_DIR)/main.cpp

# Fichiers sources pour les tests
TEST_SOURCES = $(SRC_DIR)/Article.cpp \
               $(SRC_DIR)/Inventory.cpp \
               $(TEST_DIR)/tests.cpp

# Cibles
.PHONY: all clean install-deps run help

all: inventory

# Nouvelle version avec interface et base de données
inventory: $(NEW_SOURCES)
	@echo "🔨 Compilation de l'application principale..."
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/inventory $(NEW_SOURCES) $(LDFLAGS)
	@echo "✅ Compilation terminée: $(BUILD_DIR)/inventory"

# Ancienne version
inventory_old: $(OLD_SOURCES)
	@echo "🔨 Compilation de l'ancienne version..."
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/inventory_old $(OLD_SOURCES)
	@echo "✅ Compilation terminée: $(BUILD_DIR)/inventory_old"

# Tests
tests: $(TEST_SOURCES)
	@echo "🔨 Compilation des tests..."
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/tests $(TEST_SOURCES)
	@echo "✅ Compilation terminée: $(BUILD_DIR)/tests"

# Installer les dépendances (Ubuntu/Debian)
install-deps:
	@echo "📦 Installation des dépendances..."
	sudo apt-get update
	sudo apt-get install -y libsqlite3-dev build-essential
	@echo "✅ Dépendances installées!"

# Lancer l'application
run: inventory
	@echo "🚀 Lancement de l'application..."
	@./$(BUILD_DIR)/inventory

# Lancer les tests
run-tests: tests
	@echo "🧪 Exécution des tests..."
	@./$(BUILD_DIR)/tests

# Nettoyer
clean:
	@echo "🧹 Nettoyage..."
	@rm -rf $(BUILD_DIR)
	@rm -f inventory.db
	@echo "✅ Nettoyage terminé!"

# Aide
help:
	@echo ""
	@echo "╔══════════════════════════════════════════════════════════════╗"
	@echo "║       SYSTÈME DE GESTION D'INVENTAIRE - AIDE                 ║"
	@echo "╚══════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "  Commandes disponibles:"
	@echo ""
	@echo "    make install-deps  - Installer les dépendances (SQLite3)"
	@echo "    make               - Compiler l'application principale"
	@echo "    make inventory_old - Compiler l'ancienne version"
	@echo "    make tests         - Compiler les tests"
	@echo "    make run           - Compiler et lancer l'application"
	@echo "    make run-tests     - Compiler et lancer les tests"
	@echo "    make clean         - Nettoyer les fichiers compilés"
	@echo ""
	@echo "  Prérequis:"
	@echo "    - g++ (compilateur C++17)"
	@echo "    - libsqlite3-dev (headers SQLite3)"
	@echo ""
	@echo "  Pour installer les prérequis sur Ubuntu/Debian:"
	@echo "    make install-deps"
	@echo ""
