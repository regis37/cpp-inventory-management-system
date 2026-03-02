# Système de Gestion d'Inventaire en C++

```
    ██╗███╗   ██╗██╗   ██╗███████╗███╗   ██╗████████╗ ██████╗
    ██║████╗  ██║██║   ██║██╔════╝████╗  ██║╚══██╔══╝██╔═══██╗
    ██║██╔██╗ ██║██║   ██║█████╗  ██╔██╗ ██║   ██║   ██║   ██║
    ██║██║╚██╗██║╚██╗ ██╔╝██╔══╝  ██║╚██╗██║   ██║   ██║   ██║
    ██║██║ ╚████║ ╚████╔╝ ███████╗██║ ╚████║   ██║   ╚██████╔╝
    ╚═╝╚═╝  ╚═══╝  ╚═══╝  ╚══════╝╚═╝  ╚═══╝   ╚═╝    ╚═════╝
```

Un système complet de gestion d'inventaire avec une interface terminal moderne et une base de données SQLite.

---

## Fonctionnalités

### Gestion des Articles
- Ajouter, modifier, supprimer des articles
- Recherche par nom, description, code-barres
- Génération automatique de codes-barres
- Prix de vente et d'achat avec calcul de marge
- Emplacement et unités personnalisables
- Dates d'expiration avec alertes

### Gestion du Stock
- Entrées et sorties de stock
- Ajustement d'inventaire
- Historique complet des mouvements
- Alertes de stock bas et rupture
- Alertes de surstock
- Indicateurs visuels de niveau de stock

### Catégories
- Organisation des articles par catégorie
- Couleurs personnalisables
- Statistiques par catégorie

### Fournisseurs
- Carnet d'adresses des fournisseurs
- Informations de contact complètes
- Association articles-fournisseurs

### Rapports & Statistiques
- Tableau de bord en temps réel
- Valeur totale de l'inventaire
- Valeur par catégorie (graphique)
- Articles les plus mouvementés
- Articles expirant bientôt
- Export CSV

### Interface Utilisateur
- Interface terminal colorée et moderne
- Bordures et tableaux Unicode
- Icônes et indicateurs visuels
- Animations de chargement
- Navigation intuitive par menu

---

## Installation

### Prérequis

- **g++** avec support C++17
- **libsqlite3-dev** (headers SQLite3)

### Sur Ubuntu/Debian

```bash
# Installer les dépendances
sudo apt-get update
sudo apt-get install -y build-essential libsqlite3-dev

# Ou utiliser le Makefile
make install-deps
```

### Sur Arch Linux

```bash
sudo pacman -S base-devel sqlite
```

### Sur macOS

```bash
brew install sqlite3
```

---

## Compilation

### Avec Make (recommandé)

```bash
# Voir l'aide
make help

# Compiler l'application principale
make

# Compiler et lancer
make run

# Nettoyer
make clean
```

### Avec CMake

```bash
mkdir build && cd build
cmake ..
make
./inventory
```

### Compilation manuelle

```bash
g++ -std=c++17 -I include \
    src/Database.cpp \
    src/UI.cpp \
    src/InventoryManager.cpp \
    tests/main_new.cpp \
    -o inventory \
    -lsqlite3 -pthread
```

---

## Utilisation

Lancez l'application :

```bash
./build/inventory
# ou
make run
```

### Menu Principal

```
╔══════════════════════════════════════════════════════════════╗
║                      MENU PRINCIPAL                          ║
╠══════════════════════════════════════════════════════════════╣
║ [1] Gestion des Articles                                     ║
║ [2] Gestion du Stock                                         ║
║ [3] Catégories                                               ║
║ [4] Fournisseurs                                             ║
║ [5] Rapports & Statistiques                                  ║
║ [6] Alertes                                                  ║
║ [7] Paramètres                                               ║
║ [0] Quitter                                                  ║
╚══════════════════════════════════════════════════════════════╝
```

---

## Structure du Projet

```
cpp-inventory-management-system/
├── include/
│   ├── Article.h          # Classe Article (ancienne version)
│   ├── Database.h         # Gestionnaire SQLite
│   ├── Inventory.h        # Classe Inventory (ancienne version)
│   ├── InventoryManager.h # Gestionnaire d'inventaire complet
│   └── UI.h               # Interface utilisateur
├── src/
│   ├── Article.cpp
│   ├── Database.cpp
│   ├── Inventory.cpp
│   ├── InventoryManager.cpp
│   └── UI.cpp
├── tests/
│   ├── main.cpp           # Ancienne version
│   ├── main_new.cpp       # Nouvelle version avec UI
│   └── tests.cpp          # Tests unitaires
├── CMakeLists.txt
├── Makefile
└── README.md
```

---

## Base de Données

L'application utilise SQLite3 avec les tables suivantes :

- **articles** : Articles avec prix, quantités, catégorie, fournisseur
- **categories** : Catégories d'articles
- **suppliers** : Fournisseurs
- **stock_movements** : Historique des mouvements de stock
- **alerts** : Alertes (stock bas, rupture, expiration)
- **settings** : Paramètres de l'application

La base de données est créée automatiquement au premier lancement (`inventory.db`).

---

## Captures d'écran

### Tableau de bord
```
   📦 15 articles   📁 7 catégories   🚚 5 fournisseurs
   💰 Valeur: 12,450.00 EUR   ⚠ 2 stock bas
```

### Liste des articles
```
╔══════╦══════════════════════════════╦════════════╦══════════╦═══════════════╗
║  ID  ║            Nom               ║    Prix    ║  Stock   ║   Catégorie   ║
╠══════╬══════════════════════════════╬════════════╬══════════╬═══════════════╣
║  1   ║ iPhone 15 Pro                ║ 1199.00 EUR║    25    ║ Électronique  ║
║  2   ║ Samsung Galaxy S24           ║  899.00 EUR║    18    ║ Électronique  ║
║  3   ║ Café Premium 500g            ║   12.50 EUR║    150   ║ Alimentaire   ║
╚══════╩══════════════════════════════╩════════════╩══════════╩═══════════════╝
```

---

## Technologies

- **C++17** : Standard moderne du langage
- **SQLite3** : Base de données embarquée
- **ANSI Escape Codes** : Couleurs et formatage terminal
- **Unicode** : Caractères de dessin pour les bordures

---

## Concepts Démontrés

- Programmation Orientée Objet (POO)
- Design Patterns (Manager, Repository)
- Séparation des responsabilités (MVC)
- Gestion de base de données SQL
- Interface utilisateur en terminal
- Gestion des erreurs et validations

---

## Licence

Ce projet est sous licence MIT.

---

## Auteur

Développé comme projet d'apprentissage C++ avancé.
