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

  /// generate IR code
  void ir_code();

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

  /// initial symbol string for IR
  void initializeIRMapped();

  /// create parse table for Tiger
  void initParseTable();

  /// parse action like TYPES, VARIABLES, FUNCTIONS declaration
  void parseAction(int expr, std::vector<TokenPair> &tempBuffer);

  /// parse for action
  void parseForAction(std::vector<TokenPair> &blockBuffer);

  /// parse for statement end
  void parseForActionEnd(std::vector<TokenPair> &blockBuffer);

  /// parse function action: function name (x:int) : return-type
  void parseFuncAction(std::vector<TokenPair> &tempBuffer);

  /// parse if statement action
  void parseIfAction(std::vector<TokenPair> &tempBuffer);

  /// parse return statement action
  void parseReturnAction(std::vector<TokenPair> &tempBuffer);

  /**
   * @brief parse expression from infix to postfix expression.
   *
   * @note  posifix expression is convenient way to do semantic
   *        check and generate IR.
   */
  std::vector<TokenPair> cvt2PostExpr(std::vector<TokenPair> &tempBuffer,
                                      size_t index);

  /// generate IR and symbol table elements from postfix expression
  TokenPair evaPostfix(std::vector<TokenPair> &expr);

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

  /// generate new temp name
  inline std::string new_temp() { return "$t" + std::to_string(numTemps++); }

  /// generate new loop label name
  inline std::string new_loop_label() {
    return "loop_label" + std::to_string(numLoops++);
  }
  /// generate new if label name
  inline std::string new_if_label() {
    return "if_label" + std::to_string(numIfs++);
  }

  /// get sub token pairs buffer
  inline std::vector<TokenPair> subTokenPairs(
      const std::vector<TokenPair> &buffer, size_t actBegin, size_t actEnd);

  /// get terminal symbol type
  std::string getSymbolType(TokenPair A);

  /// detect action
  bool detectAction(int symbol, bool &enable_block,
                    std::vector<TokenPair> &blockBuffer, bool &enable_buffer,
                    std::vector<TokenPair> &tempBuffer);

  Scanner scanner;        /// code scanner
  int numErrors = 0;      /// how many errors detected
  int currentLevel = -1;  /// current paser code's scope level
  std::pair<std::string, std::string>
      currLoopLabel_;          /// current loop label name
  int numIfs = 0;              /// generate if labels for IR
  int numTemps = 0;            /// generate temp variable for IR
  int numLoops = 0;            /// generate loop label name for IR
  std::string globalFileName;  /// global file name
  std::stack<int> parseStack;  /// parse stack

  std::unordered_map<int, std::string> OperatorMapped;
  std::vector<std::string> IR;  /// IR container

  /// terminal symbol's string output for error info
  std::unordered_map<int, std::string> terminalMapped_;

  /// parse table for parsing
  std::map<SymbolTerminalPair, std::vector<int> > parseTable_;

  /// for begin and end expr stack
  std::stack<TokenPair> tempStack_;

  /// loop label stack
  std::stack<std::pair<std::string, std::string> > labelStack_;

  /// current action begin and end postion in temp buffer
  size_t actBegin_;
  size_t actEnd_;

  /// inside a function return type
  bool isInside_func_ = false;
  /// tracking funcRetType if inside a function
  std::string funcRetType_;
  bool isFuncRet_ = false;
};
