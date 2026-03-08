#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class InvalidIdException : public std::invalid_argument {
public:
  InvalidIdException(const std::string& message)
      : std::invalid_argument(message) {}
};

class InvalidPriceException : public std::invalid_argument {
public:
  InvalidPriceException(const std::string& message)
      : std::invalid_argument(message) {}
};

class InvalidNameException : public std::invalid_argument {
public:
  InvalidNameException(const std::string& message)
      : std::invalid_argument(message) {}
};

class ArticleNotFoundException : public std::invalid_argument {
public:
  ArticleNotFoundException(const std::string& message)
      : std::invalid_argument(message) {}
};

class DuplicateArticleException : public std::invalid_argument {
public:
  DuplicateArticleException(const std::string& message)
      : std::invalid_argument(message) {}
};

#endif