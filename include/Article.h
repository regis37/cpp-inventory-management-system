#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

/*
 * Represents a single product inside the inventory.
 * Stores identifier, name and price.
 */
class Article {
    private:
      int id;                 // Unique identifier
      std::string name;       // Product name
      double price;           // Product price in EUR
    public:
      Article(int id, const std::string& name, double price);

      int getId() const;
      std::string getName() const;
      double getPrice() const;
      std::string toString() const;

    void setName(const std::string& name);
    void setPrice(double price);
};

#endif