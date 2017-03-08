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

/**
 * Template used for reverse iteration in C++11 range-based for loops.
 *
 *  std::vector<int> v = {1, 2, 3, 4, 5};
 *  for (auto x : reverse_iterate(v))
 *    std::cout << x << " ";
 */
template <typename T>
class reverse_range final {
 private:
  T &x;

 public:
  explicit reverse_range(T &x) : x(x) {}
  auto begin() const -> decltype(this->x.rbegin()) { return x.rbegin(); }
  auto end() const -> decltype(this->x.rend()) { return x.rend(); }
};

template <typename T>
reverse_range<T> reverse_iterate(T &x) {
  return reverse_range<T>(x);
}

class Parser final {
 public:
  /// disable implicit copy
  DISABLE_COPY(Parser);

  /// enable explicit constructor
  explicit Parser(std::string fileName);

  /// parse scanner's tokens
  void parse();

  /// parse error info
  void error(int expr, TokenPair *word);

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

  /// parse action like TYPES, VARIABLES, FUNCTIONS declaration
  void parseAction(int expr, std::vector<TokenPair> &tempBuffer);

  /**
   * @brief parse expression from infix to postfix expression.
   *
   * @note  posifix expression is convenient way to do semantic
   *        check and generate IR.
   */
  std::string cvt2PostExpr(std::vector<TokenPair> &tempBuffer, size_t index);

  /// generate IR and symbol table elements from postfix expression
  void genExprIR(std::string expr);

  /// initialize Scop
  inline void initScoping() {
    ++currentLevel;
    SymbolTablePtr st = std::make_shared<SymbolTable>(currentLevel);
    g_SymbolTable[currentLevel] = st;
  }

  /// finalize Scope
  inline void finalizeScoping() {
    g_SymbolTable[currentLevel]->dump();
    g_SymbolTable.erase(currentLevel);
    --currentLevel;
  }

  /// detect action
  bool detectAction(int symbol, bool &enable_buffer,
                    std::vector<TokenPair> &tempBuffer);

  Scanner scanner;             /// code scanner
  int numErrors = 0;           /// how many errors detected
  int currentLevel = -1;       /// current paser code's scope level
  int numTemps = 0;            /// generate temp variable for IR
  std::string globalFileName;  /// global file name
  std::stack<int> parseStack;  /// parse stack
  std::stack<int> expOpStack;  /// expression operator stack

  /// terminal symbol's string output for error info
  std::unordered_map<int, std::string> terminalMapped;

  /// parse table for parsing
  std::map<SymbolTerminalPair, std::vector<int> > parseTable;
};
