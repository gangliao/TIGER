
#pragma once

#include <unordered_map>

#include "Level.hpp"
#include "Record.hpp"
#include "SymbolTablePair.hpp"

/// Implement the scoping and symbol table details in this file
class SymbolTable;

using SymbolTablePtr = std::shared_ptr<SymbolTable>;

class SymbolTable {
 private:
  Level topLevel;
  Level currLevel;

  int scopeLevel;
  std::map<SymbolTablePair, RecordPtr> table_;

 public:
  SymbolTable(int level) {
    scopeLevel = level;
    insertConstants();
  }

  void insertTypes(SymbolTablePair& idx,  // NOLINT
                   std::string arg1,      // NOLINT
                   std::string arg2,      // NOLINT
                   std::string arg3) {    // NOLINT
    RecordPtr record = std::make_shared<Record>(scopeLevel);
    if (arg1 == "array") {
      /*************************************
      * type id = array [num] of type_id
      *            |      |        |
      *           arg1   arg2     arg3
      *************************************/
      record->dimension = std::stoi(arg2);
      record->type = arg3;
    } else {
      /*************************************
      * type id = <id> | int | float
      *************************************/
      record->dimension = lookup(Entry::Types, arg1)->getDimension();
      record->type = lookup(Entry::Types, arg1)->getType();
    }
    // insert into table
    table_[idx] = record;
  }

  void insertFunctions(SymbolTablePair& idx, // NOLINT
                       std::string retType,  // NOLINT
                       std::vector<std::string>& paramTypes, // NOLINT
                       std::vector<std::string>& params) {   // NOLINT
    RecordPtr record = std::make_shared<Record>(scopeLevel);
    record->parameterTypes.reserve(paramTypes.size());
    record->parameterDimensions.reserve(paramTypes.size());

    for (auto& paramType : paramTypes) {
      // assign parameters type
      record->parameterTypes.push_back(
          lookup(Entry::Types, paramType)->getType());
      // assign parameters dim
      record->parameterDimensions.push_back(
          lookup(Entry::Types, paramType)->getDimension());
    }

    // assign return type
    record->returnType = retType;
    // insert into table
    table_[idx] = record;
  }

  void insertVariables(SymbolTablePair& idx, // NOLINT
                       std::string arg1,     // NOLINT
                       std::string arg2,     // NOLINT
                       std::string arg3) {   // NOLINT
    RecordPtr record = std::make_shared<Record>(scopeLevel);

    if (arg1 == "array") {
      record->dimension = std::stoi(arg2);
      record->type = arg3;
    } else {
      record->dimension = lookup(Entry::Types, arg1)->getDimension();
      record->type = lookup(Entry::Types, arg1)->getType();
    }
    table_[idx] = record;
  }

  void insertConstants() {
    RecordPtr record1 = std::make_shared<Record>(scopeLevel);
    record1->dimension = 1;
    record1->type = "int";
    SymbolTablePair idx1(Entry::Types, "int");
    table_[idx1] = record1;

    RecordPtr record2 = std::make_shared<Record>(scopeLevel);
    record2->dimension = 1;
    record2->type = "float";
    SymbolTablePair idx2(Entry::Types, "float");
    table_[idx2] = record2;
  }

  void insertTemporaries() {
    std::cerr << "Not Implemented! \n";
    std::exit(EXIT_FAILURE);
  }

  const RecordPtr lookup(Entry entry, std::string name) {
    SymbolTablePair idx(entry, name);
    if (table_.find(idx) == table_.end()) {
      std::cerr << name << " is not defined before! \n";
      std::exit(EXIT_FAILURE);
    }
    return table_[idx];
  }

  void dump() {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Symbol table level: " << scopeLevel << std::endl;
    std::cout << "---------------------------------" << std::endl;
    for (auto& item : table_) {
      if (item.first.getEntry() == Entry::Types) {
        std::cout << "TYPE: \t" << item.first.getName() << std::endl;
        std::cout << "type: \t" << item.second->getType() << std::endl;
        std::cout << "dim: \t" << item.second->getDimension() << std::endl;
      } else if (item.first.getEntry() == Entry::Variables) {
        std::cout << "VARIABLE: \t" << item.first.getName() << std::endl;
        std::cout << "type: \t" << item.second->getType() << std::endl;
        std::cout << "dim: \t" << item.second->getDimension() << std::endl;
      } else if (item.first.getEntry() == Entry::Functions) {
        std::cout << "FUNCTION: \t" << item.first.getName() << std::endl;
        auto& paramType = item.second->getParameterTypes();
        auto& paramDims = item.second->getParameterDimensions();
        std::cout << "Parameters: \t" << std::endl;
        for (int i = 0; i < paramType.size(); ++i) {
          std::cout << "param " << i << " :" << paramType[i]
                    << " type: " << paramDims[i] << std::endl;
        }
        std::cout << "Return Type: \t" << item.second->getReturnType()
                  << std::endl;
      }
    }
    std::cout << "---------------------------------" << std::endl;
  }
};
