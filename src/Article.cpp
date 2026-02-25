#include "Article.h"
#include <sstream>
#include <iomanip>


// Constructor
Article::Article(int id, const std::string& name, double price)
    : id(id), name(name), price(price) {}

// Getters
int Article::getId() const {
    return id;
}

std::string Article::getName() const {
    return name;
}

double Article::getPrice() const {
    return price;
}
// Setters
void Article::setName(const std::string& name) { this->name = name; }

void Article::setPrice(double price) { this->price = price; }

/*
 -Creates formatted string:
 -For example: "483 Chewing Gum (0.20 EUR)"
 */

std::string Article::toString() const {
    std::ostringstream stream;

    stream << id << " " << name << " ("
           << std::fixed
           << std::setprecision(2)   // Always 2 decimal places
           << price << " EUR)";
    return stream.str();
}