
#pragma once

#include <unordered_map>

#include "Level.hpp"
#include "Record.hpp"
#include "SymbolTablePair.hpp"

/// Implement the scoping and symbol table details in this file
class SymbolTable {
 private:
  Level topLevel;
  Level currLevel;

  int scopeLevel;
  std::map<SymbolTablePair, Record> table_;

  void insertTypes(SymbolTablePair& idx, std::string& arg1, std::string& arg2,
                   std::string& arg3) {
    Record record(scopeLevel);

    if (arg1 == "array") {
      /*************************************
      * type id = array [num] of type_id
      *            |      |        |
      *           arg1   arg2     arg3
      *************************************/
      record.dimension = std::stoi(arg2);
      record.type = arg3;
    } else {
      /*************************************
      * type id = <id> | int | float
      *************************************/
      record.dimension = lookup(Entry::Types, arg1).getDimension();
      record.type = lookup(Entry::Types, arg1).getType();
    }
    // insert into table
    table_[idx] = record;
  }

  void insertFunctions(SymbolTablePair& idx, std::string& retType,
                       std::vector<std::string>& paramTypes) {
    Record record(scopeLevel);
    record.parameterTypes.reserve(paramTypes.size());
    record.parameterDimensions.reserve(paramTypes.size());

    for (auto& paramType : paramTypes) {
      // assign parameters type
      record.parameterTypes.push_back(
          lookup(Entry::Types, paramType).getType());
      // assign parameters dim
      record.parameterDimensions.push_back(
          lookup(Entry::Types, paramType).getDimension());
    }

    // assign return type
    record.returnType = retType;
    // insert into table
    table_[idx] = record;
  }

  void insertVariables(SymbolTablePair& idx, std::string& arg1,
                       std::string& arg2, std::string& arg3) {
    Record record(scopeLevel);

    if (arg1 == "array") {
      record.dimension = std::stoi(arg2);
      record.type = arg3;
    } else {
      record.dimension = lookup(Entry::Types, arg1).getDimension();
      record.type = lookup(Entry::Types, arg1).getType();
    }
    table_[idx] = record;
  }

  void insertConstants() {
    Record record1(scopeLevel);
    record1.dimension = 1;
    record1.type = "int";
    SymbolTablePair idx1(Entry::Types, "int");
    table_[idx1] = record1;

    Record record2(scopeLevel);
    record2.dimension = 1;
    record2.type = "float";
    SymbolTablePair idx2(Entry::Types, "float");
    table_[idx2] = record2;
  }

  void insertTemporaries(SymbolTablePair& idx) {
    std::cerr << "Not Implemented! \n";
    std::exit(EXIT_FAILURE);
  }

 public:
  SymbolTable(int level) {
    scopeLevel = level;
    insertConstants();
  }

  /// insert symbols into table
  template <typename... Args>
  void insert(SymbolTablePair& idx, Args... args) {
#define INSERT_SYMBOL_TABLE(__type__) \
  case Entry::__type__: {             \
    insert##__type__(idx, args...);   \
    break;                            \
  }

    switch (idx.getEntry()) {
      INSERT_SYMBOL_TABLE(Types)
      INSERT_SYMBOL_TABLE(Constants)
      INSERT_SYMBOL_TABLE(Functions)
      INSERT_SYMBOL_TABLE(Variables)
      INSERT_SYMBOL_TABLE(Temporaries)
      default: {
        std::cerr << "Entry enum value: " << idx.getEntry()
                  << " not defined! \n";
        std::exit(EXIT_FAILURE);
      }
    }
  }

  const Record& lookup(Entry entry, std::string name) {
    SymbolTablePair idx(entry, name);
    if (table_.find(idx) == table_.end()) {
      std::cerr << name << " is not defined before! \n";
      std::exit(EXIT_FAILURE);
    }
    return table_[idx];
  }
};
