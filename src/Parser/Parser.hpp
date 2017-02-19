/**
 * Parser
 *
 * @author: gangliao
 */
#pragma once

#include <cstring>
#include <unordered_map>
#include "../Scanner/Scanner.hpp"
#include "../SemanticAnalyzer/SymbolTable.hpp"
#include "../SemanticAnalyzer/SymbolTerminalPair.hpp"

/**
 * Disable copy macro.
 */
#define DISABLE_COPY(class_name)                \
  class_name(class_name &&) = delete;           \
  class_name(const class_name &other) = delete; \
  class_name &operator=(const class_name &other) = delete

class Parser final {
 public:
  /// disable implicit copy
  DISABLE_COPY(Parser);
  /// enable explicit constructor
  explicit Parser(std::string fileName);
  /// parse scanner's tokens
  void parse();
  /// parse error info
  void error(std::string message);
  /// insert items into parse table
  void addToParseTable(const int nonterm, const std::vector<int> &terminals,
                       const std::vector<int> &expand_rule);
  /// output file
  std::ofstream outFile;
  /// print debug
  bool printDebug;

 private:
  /// initialize terminal map data structure: terminalMapped
  void initializeTerminalMapped();
  /// create parse table for Tiger
  void initParseTable();
  Scanner scanner;
  int numErrors;
  std::string globalFileName;
  std::stack<int> parseStack;
  std::unordered_map<int, std::string> terminalMapped;
  std::map<SymbolTerminalPair, std::vector<int> > parseTable;
};
