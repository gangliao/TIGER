#include "Level.hpp"
#include "Record.hpp"
// Implement the scoping and symbol table details in this file
class SymbolTable {
 private:
  Level topLevel;
  Level currLevel;

  int scopeLevel;

 public:
  SymbolTable() {
    Record intRecord(scopeLevel);
    Record floatRecord(scopeLevel);
    intRecord.type = "int";
    floatRecord.type = "float";
    //.... Fill in the other built-in constructs
    // also add any  data structure required
  }
};
