
#pragma once

#include "../common/Symbol.hpp"

class SymbolTablePair {
 private:
  Entry entry;
  std::string name;

 public:
  SymbolTablePair(Entry entry, std::string name) {
    this->entry = entry;
    this->name = name;
  }

  const Entry& getEntry() const { return entry; }

  const std::string& getName() const { return name; }

  friend bool operator<(const SymbolTablePair &lhs,
                        const SymbolTablePair &rhs) {
    if (lhs.entry < rhs.entry) {
      return true;
    } else {
      if (lhs.entry == rhs.entry) {
        if (lhs.name < rhs.name) {
          return true;
        }
      }
    }
    return false;
  }
};
