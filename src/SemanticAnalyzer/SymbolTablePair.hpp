//#include "../common/Symbol.hpp"

class SymbolTablePair {
private:
  Entry entry;
  std::string name;

public:
  SymbolTablePair(Entry entry, std::string name) {
    this->entry = entry;
    this->name = name;
  }

  Entry getEntry() { return entry; }

  std::string getName() { return name; }
};
