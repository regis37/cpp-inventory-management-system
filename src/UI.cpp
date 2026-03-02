#include "UI.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <thread>
#include <chrono>
#include <algorithm>

namespace UI {

    void clearScreen() {
        std::cout << "\033[2J\033[H";
    }

    void setCursorPosition(int x, int y) {
        std::cout << "\033[" << y << ";" << x << "H";
    }

    void hideCursor() {
        std::cout << "\033[?25l";
    }

    void showCursor() {
        std::cout << "\033[?25h";
    }

    void printHeader(const std::string& title, int width) {
        std::cout << Color::CYAN << Border::TOP_LEFT;
        for (int i = 0; i < width - 2; i++) std::cout << Border::HORIZONTAL;
        std::cout << Border::TOP_RIGHT << Color::RESET << "\n";

        std::cout << Color::CYAN << Border::VERTICAL << Color::RESET;
        std::cout << Color::BOLD << Color::BRIGHT_WHITE << centerText(title, width - 2) << Color::RESET;
        std::cout << Color::CYAN << Border::VERTICAL << Color::RESET << "\n";

        std::cout << Color::CYAN << Border::T_RIGHT;
        for (int i = 0; i < width - 2; i++) std::cout << Border::HORIZONTAL;
        std::cout << Border::T_LEFT << Color::RESET << "\n";
    }

    void printFooter(int width) {
        std::cout << Color::CYAN << Border::BOTTOM_LEFT;
        for (int i = 0; i < width - 2; i++) std::cout << Border::HORIZONTAL;
        std::cout << Border::BOTTOM_RIGHT << Color::RESET << "\n";
    }

    void printBox(const std::string& content, int width, const std::string& color) {
        std::cout << color << Border::TOP_LEFT;
        for (int i = 0; i < width - 2; i++) std::cout << Border::HORIZONTAL;
        std::cout << Border::TOP_RIGHT << Color::RESET << "\n";

        std::cout << color << Border::VERTICAL << Color::RESET;
        std::cout << centerText(content, width - 2);
        std::cout << color << Border::VERTICAL << Color::RESET << "\n";

        std::cout << color << Border::BOTTOM_LEFT;
        for (int i = 0; i < width - 2; i++) std::cout << Border::HORIZONTAL;
        std::cout << Border::BOTTOM_RIGHT << Color::RESET << "\n";
    }

    void printLine(int width, char c) {
        std::cout << Color::DIM;
        for (int i = 0; i < width; i++) std::cout << c;
        std::cout << Color::RESET << "\n";
    }

    void printSeparator(int width) {
        std::cout << Color::CYAN << Border::T_RIGHT;
        for (int i = 0; i < width - 2; i++) std::cout << Border::HORIZONTAL;
        std::cout << Border::T_LEFT << Color::RESET << "\n";
    }

    void printSuccess(const std::string& message) {
        std::cout << Color::GREEN << Color::BOLD << " " << Icon::CHECK << " " << message << Color::RESET << "\n";
    }

    void printError(const std::string& message) {
        std::cout << Color::RED << Color::BOLD << " " << Icon::CROSS << " " << message << Color::RESET << "\n";
    }

    void printWarning(const std::string& message) {
        std::cout << Color::YELLOW << Color::BOLD << " " << Icon::WARNING << " " << message << Color::RESET << "\n";
    }

    void printInfo(const std::string& message) {
        std::cout << Color::BLUE << " " << Icon::INFO << " " << message << Color::RESET << "\n";
    }

    void printTableHeader(const std::vector<std::string>& headers, const std::vector<int>& widths) {
        // Top border
        std::cout << Color::CYAN << Border::TOP_LEFT;
        for (size_t i = 0; i < widths.size(); i++) {
            for (int j = 0; j < widths[i]; j++) std::cout << Border::HORIZONTAL;
            if (i < widths.size() - 1) std::cout << Border::T_DOWN;
        }
        std::cout << Border::TOP_RIGHT << Color::RESET << "\n";

        // Headers
        std::cout << Color::CYAN << Border::VERTICAL << Color::RESET;
        for (size_t i = 0; i < headers.size(); i++) {
            std::cout << Color::BOLD << Color::BRIGHT_WHITE << centerText(headers[i], widths[i]) << Color::RESET;
            std::cout << Color::CYAN << Border::VERTICAL << Color::RESET;
        }
        std::cout << "\n";

        // Separator
        std::cout << Color::CYAN << Border::T_RIGHT;
        for (size_t i = 0; i < widths.size(); i++) {
            for (int j = 0; j < widths[i]; j++) std::cout << Border::HORIZONTAL;
            if (i < widths.size() - 1) std::cout << Border::CROSS;
        }
        std::cout << Border::T_LEFT << Color::RESET << "\n";
    }

    void printTableRow(const std::vector<std::string>& cells, const std::vector<int>& widths) {
        std::cout << Color::CYAN << Border::VERTICAL << Color::RESET;
        for (size_t i = 0; i < cells.size(); i++) {
            std::string cell = truncate(cells[i], widths[i] - 2);
            std::cout << " " << leftAlign(cell, widths[i] - 2) << " ";
            std::cout << Color::CYAN << Border::VERTICAL << Color::RESET;
        }
        std::cout << "\n";
    }

    void printTableSeparator(const std::vector<int>& widths) {
        std::cout << Color::CYAN << Border::T_RIGHT;
        for (size_t i = 0; i < widths.size(); i++) {
            for (int j = 0; j < widths[i]; j++) std::cout << Border::HORIZONTAL;
            if (i < widths.size() - 1) std::cout << Border::CROSS;
        }
        std::cout << Border::T_LEFT << Color::RESET << "\n";
    }

    void printTableFooter(const std::vector<int>& widths) {
        std::cout << Color::CYAN << Border::BOTTOM_LEFT;
        for (size_t i = 0; i < widths.size(); i++) {
            for (int j = 0; j < widths[i]; j++) std::cout << Border::HORIZONTAL;
            if (i < widths.size() - 1) std::cout << Border::T_UP;
        }
        std::cout << Border::BOTTOM_RIGHT << Color::RESET << "\n";
    }

    void printMenuItem(int number, const std::string& text, const std::string& icon) {
        std::cout << Color::CYAN << Border::VERTICAL << Color::RESET << " ";
        if (!icon.empty()) {
            std::cout << icon << " ";
        }
        std::cout << Color::BRIGHT_YELLOW << "[" << number << "]" << Color::RESET;
        std::cout << " " << text << "\n";
    }

    void printMenuTitle(const std::string& title) {
        std::cout << "\n" << Color::CYAN << Border::VERTICAL << Color::RESET;
        std::cout << Color::BOLD << Color::BRIGHT_MAGENTA << " " << Icon::ARROW_RIGHT << " " << title << Color::RESET << "\n";
    }

    void printProgressBar(int current, int total, int width) {
        float progress = (total > 0) ? static_cast<float>(current) / total : 0;
        int filled = static_cast<int>(progress * width);

        std::cout << Color::DIM << "[" << Color::RESET;
        for (int i = 0; i < width; i++) {
            if (i < filled) {
                std::cout << Color::GREEN << "█" << Color::RESET;
            } else {
                std::cout << Color::DIM << "░" << Color::RESET;
            }
        }
        std::cout << Color::DIM << "]" << Color::RESET;
        std::cout << " " << static_cast<int>(progress * 100) << "%";
    }

    void printStockBar(int current, int min, int max, int width) {
        float ratio = (max > 0) ? static_cast<float>(current) / max : 0;
        int filled = static_cast<int>(ratio * width);

        std::string barColor;
        if (current <= min) {
            barColor = Color::RED;
        } else if (current <= min * 2) {
            barColor = Color::YELLOW;
        } else {
            barColor = Color::GREEN;
        }

        std::cout << "[";
        for (int i = 0; i < width; i++) {
            if (i < filled) {
                std::cout << barColor << "█" << Color::RESET;
            } else {
                std::cout << Color::DIM << "░" << Color::RESET;
            }
        }
        std::cout << "] " << current << "/" << max;
    }

    std::string readString(const std::string& prompt) {
        std::cout << Color::BRIGHT_CYAN << " " << Icon::ARROW_RIGHT << " " << Color::RESET << prompt << ": ";
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    int readInt(const std::string& prompt) {
        while (true) {
            std::cout << Color::BRIGHT_CYAN << " " << Icon::ARROW_RIGHT << " " << Color::RESET << prompt << ": ";
            std::string input;
            std::getline(std::cin, input);
            try {
                return std::stoi(input);
            } catch (...) {
                printError("Veuillez entrer un nombre valide.");
            }
        }
    }

    double readDouble(const std::string& prompt) {
        while (true) {
            std::cout << Color::BRIGHT_CYAN << " " << Icon::ARROW_RIGHT << " " << Color::RESET << prompt << ": ";
            std::string input;
            std::getline(std::cin, input);
            // Remplacer les virgules par des points
            std::replace(input.begin(), input.end(), ',', '.');
            try {
                return std::stod(input);
            } catch (...) {
                printError("Veuillez entrer un nombre valide.");
            }
        }
    }

    bool readConfirm(const std::string& prompt) {
        std::cout << Color::BRIGHT_YELLOW << " " << Icon::WARNING << " " << Color::RESET << prompt << " (o/n): ";
        std::string input;
        std::getline(std::cin, input);
        return (input == "o" || input == "O" || input == "oui" || input == "Oui" || input == "y" || input == "Y" || input == "yes");
    }

    int readMenuChoice(int min, int max) {
        while (true) {
            std::cout << "\n" << Color::BRIGHT_CYAN << " " << Icon::ARROW_RIGHT << " " << Color::RESET << "Votre choix: ";
            std::string input;
            std::getline(std::cin, input);
            try {
                int choice = std::stoi(input);
                if (choice >= min && choice <= max) {
                    return choice;
                }
                printError("Choix invalide. Veuillez entrer un nombre entre " + std::to_string(min) + " et " + std::to_string(max) + ".");
            } catch (...) {
                printError("Veuillez entrer un nombre valide.");
            }
        }
    }

    std::string centerText(const std::string& text, int width) {
        int textLen = text.length();
        if (textLen >= width) return text.substr(0, width);
        int padding = (width - textLen) / 2;
        return std::string(padding, ' ') + text + std::string(width - textLen - padding, ' ');
    }

    std::string leftAlign(const std::string& text, int width) {
        if (static_cast<int>(text.length()) >= width) return text.substr(0, width);
        return text + std::string(width - text.length(), ' ');
    }

    std::string rightAlign(const std::string& text, int width) {
        if (static_cast<int>(text.length()) >= width) return text.substr(0, width);
        return std::string(width - text.length(), ' ') + text;
    }

    std::string truncate(const std::string& text, int maxLength) {
        if (static_cast<int>(text.length()) <= maxLength) return text;
        if (maxLength <= 3) return text.substr(0, maxLength);
        return text.substr(0, maxLength - 3) + "...";
    }

    std::string formatPrice(double price, const std::string& currency) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << price << " " << currency;
        return ss.str();
    }

    std::string formatDate(const std::string& date) {
        if (date.empty() || date == "NULL") return "-";
        // Format YYYY-MM-DD vers DD/MM/YYYY
        if (date.length() >= 10 && date[4] == '-') {
            return date.substr(8, 2) + "/" + date.substr(5, 2) + "/" + date.substr(0, 4);
        }
        return date;
    }

    std::string formatQuantity(int quantity, const std::string& unit) {
        std::string result = std::to_string(quantity);
        if (!unit.empty()) {
            result += " " + unit;
        }
        return result;
    }

    void printLoadingAnimation(const std::string& message, int duration) {
        const std::vector<std::string> frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
        int frameCount = frames.size();
        int iterations = duration / 100;

        hideCursor();
        for (int i = 0; i < iterations; i++) {
            std::cout << "\r" << Color::CYAN << frames[i % frameCount] << " " << Color::RESET << message << "   " << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "\r" << std::string(message.length() + 10, ' ') << "\r";
        showCursor();
    }

    void printTypingEffect(const std::string& text, int delayMs) {
        for (char c : text) {
            std::cout << c << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
    }

    void printLogo() {
        std::cout << Color::CYAN << R"(
    ╔══════════════════════════════════════════════════════════════════╗
    ║                                                                  ║
    ║   ██╗███╗   ██╗██╗   ██╗███████╗███╗   ██╗████████╗ ██████╗      ║
    ║   ██║████╗  ██║██║   ██║██╔════╝████╗  ██║╚══██╔══╝██╔═══██╗     ║
    ║   ██║██╔██╗ ██║██║   ██║█████╗  ██╔██╗ ██║   ██║   ██║   ██║     ║
    ║   ██║██║╚██╗██║╚██╗ ██╔╝██╔══╝  ██║╚██╗██║   ██║   ██║   ██║     ║
    ║   ██║██║ ╚████║ ╚████╔╝ ███████╗██║ ╚████║   ██║   ╚██████╔╝     ║
    ║   ╚═╝╚═╝  ╚═══╝  ╚═══╝  ╚══════╝╚═╝  ╚═══╝   ╚═╝    ╚═════╝      ║
    ║                                                                  ║
    ║)" << Color::BRIGHT_YELLOW << "              📦 SYSTÈME DE GESTION D'INVENTAIRE 📦" << Color::CYAN << R"(              ║
    ║                                                                  ║
    ╚══════════════════════════════════════════════════════════════════╝
)" << Color::RESET;
    }

    void printWelcome() {
        clearScreen();
        printLogo();
        std::cout << "\n";
        printInfo("Bienvenue dans votre système de gestion d'inventaire!");
        printInfo("Tapez le numéro de l'option souhaitée pour naviguer.");
        std::cout << "\n";
    }

    void printGoodbye() {
        std::cout << "\n";
        printBox("Merci d'avoir utilisé notre système! À bientôt! 👋", 60, Color::GREEN);
        std::cout << "\n";
    }

}
