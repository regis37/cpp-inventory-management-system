#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

namespace UI {
    // Codes de couleurs ANSI
    namespace Color {
        const std::string RESET = "\033[0m";
        const std::string BOLD = "\033[1m";
        const std::string DIM = "\033[2m";
        const std::string UNDERLINE = "\033[4m";
        const std::string BLINK = "\033[5m";
        const std::string REVERSE = "\033[7m";

        // Couleurs de texte
        const std::string BLACK = "\033[30m";
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string BLUE = "\033[34m";
        const std::string MAGENTA = "\033[35m";
        const std::string CYAN = "\033[36m";
        const std::string WHITE = "\033[37m";

        // Couleurs vives
        const std::string BRIGHT_BLACK = "\033[90m";
        const std::string BRIGHT_RED = "\033[91m";
        const std::string BRIGHT_GREEN = "\033[92m";
        const std::string BRIGHT_YELLOW = "\033[93m";
        const std::string BRIGHT_BLUE = "\033[94m";
        const std::string BRIGHT_MAGENTA = "\033[95m";
        const std::string BRIGHT_CYAN = "\033[96m";
        const std::string BRIGHT_WHITE = "\033[97m";

        // Couleurs de fond
        const std::string BG_BLACK = "\033[40m";
        const std::string BG_RED = "\033[41m";
        const std::string BG_GREEN = "\033[42m";
        const std::string BG_YELLOW = "\033[43m";
        const std::string BG_BLUE = "\033[44m";
        const std::string BG_MAGENTA = "\033[45m";
        const std::string BG_CYAN = "\033[46m";
        const std::string BG_WHITE = "\033[47m";
    }

    // Caractères de bordure (Unicode box-drawing)
    namespace Border {
        const std::string TOP_LEFT = "╔";
        const std::string TOP_RIGHT = "╗";
        const std::string BOTTOM_LEFT = "╚";
        const std::string BOTTOM_RIGHT = "╝";
        const std::string HORIZONTAL = "═";
        const std::string VERTICAL = "║";
        const std::string T_DOWN = "╦";
        const std::string T_UP = "╩";
        const std::string T_RIGHT = "╠";
        const std::string T_LEFT = "╣";
        const std::string CROSS = "╬";

        // Bordure simple
        const std::string S_TOP_LEFT = "┌";
        const std::string S_TOP_RIGHT = "┐";
        const std::string S_BOTTOM_LEFT = "└";
        const std::string S_BOTTOM_RIGHT = "┘";
        const std::string S_HORIZONTAL = "─";
        const std::string S_VERTICAL = "│";
        const std::string S_T_DOWN = "┬";
        const std::string S_T_UP = "┴";
        const std::string S_T_RIGHT = "├";
        const std::string S_T_LEFT = "┤";
        const std::string S_CROSS = "┼";
    }

    // Icônes
    namespace Icon {
        const std::string CHECK = "✓";
        const std::string CROSS = "✗";
        const std::string WARNING = "⚠";
        const std::string INFO = "ℹ";
        const std::string STAR = "★";
        const std::string ARROW_RIGHT = "→";
        const std::string ARROW_LEFT = "←";
        const std::string ARROW_UP = "↑";
        const std::string ARROW_DOWN = "↓";
        const std::string BULLET = "•";
        const std::string PACKAGE = "📦";
        const std::string CART = "🛒";
        const std::string MONEY = "💰";
        const std::string CHART = "📊";
        const std::string SEARCH = "🔍";
        const std::string SETTINGS = "⚙";
        const std::string USER = "👤";
        const std::string FOLDER = "📁";
        const std::string ALERT = "🔔";
        const std::string TRUCK = "🚚";
    }

    // Fonctions utilitaires
    void clearScreen();
    void setCursorPosition(int x, int y);
    void hideCursor();
    void showCursor();

    // Affichage
    void printHeader(const std::string& title, int width = 60);
    void printFooter(int width = 60);
    void printBox(const std::string& content, int width = 60, const std::string& color = Color::CYAN);
    void printLine(int width = 60, char c = '=');
    void printSeparator(int width = 60);

    // Messages
    void printSuccess(const std::string& message);
    void printError(const std::string& message);
    void printWarning(const std::string& message);
    void printInfo(const std::string& message);

    // Tableaux
    void printTableHeader(const std::vector<std::string>& headers, const std::vector<int>& widths);
    void printTableRow(const std::vector<std::string>& cells, const std::vector<int>& widths);
    void printTableSeparator(const std::vector<int>& widths);
    void printTableFooter(const std::vector<int>& widths);

    // Menus
    void printMenuItem(int number, const std::string& text, const std::string& icon = "");
    void printMenuTitle(const std::string& title);

    // Barres de progression
    void printProgressBar(int current, int total, int width = 40);
    void printStockBar(int current, int min, int max, int width = 20);

    // Input
    std::string readString(const std::string& prompt);
    int readInt(const std::string& prompt);
    double readDouble(const std::string& prompt);
    bool readConfirm(const std::string& prompt);
    int readMenuChoice(int min, int max);

    // Utilitaires de formatage
    std::string centerText(const std::string& text, int width);
    std::string leftAlign(const std::string& text, int width);
    std::string rightAlign(const std::string& text, int width);
    std::string truncate(const std::string& text, int maxLength);
    std::string formatPrice(double price, const std::string& currency = "EUR");
    std::string formatDate(const std::string& date);
    std::string formatQuantity(int quantity, const std::string& unit = "");

    // Animation
    void printLoadingAnimation(const std::string& message, int duration = 1000);
    void printTypingEffect(const std::string& text, int delayMs = 30);

    // Logo et bannières
    void printLogo();
    void printWelcome();
    void printGoodbye();
}

#endif
