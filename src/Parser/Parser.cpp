/**
 * Parser
 *
 * @author: Gatech Buzz
 */

#include "Parser.hpp"

Parser::Parser(std::string fileName) : scanner(fileName) {
  parseStack.push(Symbol::Terminal::EOFF);
  parseStack.push(Symbol::Nonterminal::TIGER_PROGRAM);

  initializeTerminalMapped();

  globalFileName = fileName;

  // Insert the pair stp and rhsVec inside parseTable
  // Fill in the remaining of the parseTable as above.
  // Recommended- work out the parseTable by hand and then code it here
  // parseTable.insert(std::make_pair(stp, rhsVec));

  initParseTable();
}

void Parser::addToParseTable(const int nonterm,
                             const std::vector<int>& terminals,
                             const std::vector<int>& expand_rule) {
  for (auto& term : terminals) {
    SymbolTerminalPair stp(nonterm, term);
    parseTable[stp] = expand_rule;
  }
}

/**
 * initialize Tiger's parse table by hand-coded.
 */
void Parser::initParseTable() {
  // # tiger-program
  // 1: <tiger-program> -> let <declaration-segment> in <stat-seq> end
  addToParseTable(Symbol::Nonterminal::TIGER_PROGRAM,         // NOLINT
                  {Symbol::Terminal::LET},                    // NOLINT
                  {Symbol::Terminal::LET,                     // NOLINT
                   Symbol::Nonterminal::DECLARATION_SEGMENT,  // NOLINT
                   Symbol::Terminal::IN,                      // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,             // NOLINT
                   Symbol::Terminal::END});                   // NOLINT

  // # declaration-segment
  // 2: <declaration-segment> -> <type-declaration-list> <var-declaration-list>
  // <funct-declaration-list>
  addToParseTable(Symbol::Nonterminal::DECLARATION_SEGMENT,        // NOLINT
                  {Symbol::Terminal::FUNCTION,                     // NOLINT
                   Symbol::Terminal::IN,                           // NOLINT
                   Symbol::Terminal::TYPE,                         // NOLINT
                   Symbol::Terminal::VAR},                         // NOLINT
                  {Symbol::Nonterminal::TYPE_DECLARATION_LIST,     // NOLINT
                   Symbol::Nonterminal::VAR_DECLARATION_LIST,      // NOLINT
                   Symbol::Nonterminal::FUNCT_DECLARATION_LIST});  // NOLINT

  // 3: <type-declaration-list> -> <type-declaration> <type-declaration-list>
  addToParseTable(Symbol::Nonterminal::TYPE_DECLARATION_LIST,     // NOLINT
                  {Symbol::Terminal::TYPE},                       // NOLINT
                  {Symbol::Nonterminal::TYPE_DECLARATION,         // NOLINT
                   Symbol::Nonterminal::TYPE_DECLARATION_LIST});  // NOLINT

  // 4: <type-declaration-list> -> NULL
  // 5: <var-declaration-list> -> <var-declaration> <var-declaration-list>
  // 6: <var-declaration-list> -> NULL
  // 7: <funct-declaration-list> -> <funct-declaration> <funct-declaration-list>
  // 8: <funct-declaration-list> -> NULL

  // # type-declaration
  // 9: <type-declaration> -> type id = <type>;
  // 10: <type> -> <type-id>
  // 11: <type> -> array [INTLIT] of <type-id>
  // 12: <type> -> id

  // 13: <type-id> -> int
  // 14: <type-id> -> float

  // # var-declaration
  // 15: <var-declaration> -> var <id-list> : <type> <optional-init>;
  // 16: <id-list> -> id <id-list-tail>
  // 17: <id-list-tail> -> , id <id-list-tail>
  // 18: <id-list-tail> -> NULL
  // 19: <optional-init> -> := <const>
  // 20: <optional-init> -> NULL

  // # funct-declaration
  // 21: <funct-declaration> -> function id (<param-list>) <ret-type> begin
  // <stat-seq> end;
  // 22: <param-list> -> <param> <param-list-tail>
  // 23: <param-list> -> NULL
  // 24: <param-list-tail> -> , <param> <param-list-tail>
  // 25: <param-list-tail> -> NULL
  // 26: <ret-type> -> : <type>
  // 27: <ret-type> -> NULL
  // 28: <param> -> id : <type>

  // # stat-seq
  // 29: <stat-seq> -> <stat> <stat-seq-tail>
  // 30: <stat-seq-tail> -> <stat> <stat-seq-tail>
  // 31: <stat-seq-tail> -> NULL

  // # stat
  // 32: <stat> -> if <expr> then <stat-seq> <stat-if-tail>
  // 33: <stat-if-tail> -> else <stat-seq> endif;
  // 34: <stat-if-tail> -> endif;

  // 35: <stat> -> id <stat-funct-or-assign>
  // 36: <stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
  // 37: <stat-funct-or-assign> -> (<expr-list>);

  // 38: <stat-assign> -> id <stat-assign-stuff>
  // 39: <stat-assign> -> (<expr>) <stat-assign-tail>
  // 40: <stat-assign> -> <const> <stat-assign-tail>

  // 41: <stat-assign-stuff> -> (<expr-list>)
  // 42: <stat-assign-stuff> -> <lvalue-tail> <stat-assign-tail>

  // 43: <stat-assign-tail> -> <expr-tail>
  // 44: <stat-assign-tail> -> <OR-expr-tail>
  // 45: <stat-assign-tail> -> <AND-expr-tail>
  // 46: <stat-assign-tail> -> <compare-tail>
  // 47: <stat-assign-tail> -> <term-tail>

  // 48: <stat> -> while <expr> do <stat-seq> enddo;
  // 49: <stat> -> for id := <expr> to <expr> do <stat-seq> enddo;

  // 50: <stat> -> break;
  // 51: <stat> -> return <expr>;

  // 52: <stat> -> let <declaration-segment> in <stat-seq> end

  // # expr
  // 53: <expr> -> <OR-expr> <expr-tail>
  // 54: <expr-tail> -> <OR-op> <OR-expr> <expr-tail>
  // 55: <expr-tail> -> NULL

  // 56: <OR-expr> -> <AND-expr> <OR-expr-tail>
  // 57: <OR-expr-tail> -> <AND-op> <AND-expr> <OR-expr-tail>
  // 58: <OR-expr-tail> -> NULL

  // 59: <AND-expr> -> <compare> <AND-expr-tail>
  // 60: <AND-expr-tail> -> <compare-op> <compare> <AND-expr-tail>
  // 61: <AND-expr-tail> -> NULL

  // 62: <compare> -> <term> <compare-tail>
  // 63: <compare-tail> -> <add-op> <term> <compare-tail>
  // 64: <compare-tail> -> NULL

  // 65: <term> -> <factor> <term-tail>
  // 66: <term-tail> -> <mul-op> <factor> <term-tail>
  // 67: <term-tail> -> NULL

  // 68: <factor> -> (<expr>)
  // 69: <factor> -> <const>
  // 70: <factor> -> <lvalue>

  // # const
  // 71: <const> -> INTLIT
  // 72: <const> -> FLOATLIT

  // # binary-operator
  // 73: <OR-op> -> |
  // 74: <AND-op> -> &
  // 75: <compare-op> -> <=
  // 76: <compare-op> -> >=
  // 77: <compare-op> -> <
  // 78: <compare-op> -> >
  // 79: <compare-op> -> <>
  // 80: <compare-op> -> =
  // 81: <add-op> -> -
  // 82: <add-op> -> +
  // 83: <mul-op> -> /
  // 84: <mul-op> -> *

  // 85: <expr-list> -> NULL
  // 86: <expr-list> -> <expr> <expr-list-tail>
  // 87: <expr-list-tail> -> , <expr> <expr-list-tail>
  // 88: <expr-list-tail> -> NULL

  // 89: <lvalue> -> id <lvalue-tail>
  // 90: <lvalue-tail> -> [<expr>]
  // 91: <lvalue-tail> -> NULL
}

void Parser::error(std::string message) {
  numErrors++;
  std::cout << globalFileName << ":" << scanner.getCurrLine()
            << ": error: " << message << "\n";
}

void Parser::parse() {
  TokenPair* word = scanner.getToken();
  std::string parseStackTemp = "";
  Symbol focus;

  /* Used for the DEBUG FLAG */
  int currLine = scanner.getCurrLine();
  int lastLine = scanner.getCurrLine();

  while (true) {
    // get the token and parse
    // complete parsing before going to semantic analysis
    // then embed the semantic actions in the parsing table and
    // accordingly take actions and generate the IR.
    // Output the IR, only if there are no errors in the input.
  }
}

void Parser::initializeTerminalMapped() {
  terminalMapped[Symbol::Terminal::ARRAY] = "array";
  terminalMapped[Symbol::Terminal::BREAK] = "break";
  terminalMapped[Symbol::Terminal::DO] = "do";
  terminalMapped[Symbol::Terminal::ELSE] = "else";
  terminalMapped[Symbol::Terminal::END] = "end";
  terminalMapped[Symbol::Terminal::FOR] = "for";
  terminalMapped[Symbol::Terminal::FUNCTION] = "function";
  terminalMapped[Symbol::Terminal::IF] = "if";
  terminalMapped[Symbol::Terminal::IN] = "in";
  terminalMapped[Symbol::Terminal::LET] = "let";
  terminalMapped[Symbol::Terminal::OF] = "of";
  terminalMapped[Symbol::Terminal::THEN] = "then";
  terminalMapped[Symbol::Terminal::TO] = "to";
  terminalMapped[Symbol::Terminal::TYPE] = "type";
  terminalMapped[Symbol::Terminal::VAR] = "var";
  terminalMapped[Symbol::Terminal::WHILE] = "while";
  terminalMapped[Symbol::Terminal::ENDIF] = "endif";
  terminalMapped[Symbol::Terminal::BEGIN] = "begin";
  terminalMapped[Symbol::Terminal::ENDDO] = "enddo";
  terminalMapped[Symbol::Terminal::RETURN] = "return";
  terminalMapped[Symbol::Terminal::INT] = "int";
  terminalMapped[Symbol::Terminal::FLOAT] = "float";
  terminalMapped[Symbol::Terminal::COMMA] = ",";
  terminalMapped[Symbol::Terminal::COLON] = ":";
  terminalMapped[Symbol::Terminal::SEMI] = ";";
  terminalMapped[Symbol::Terminal::LPAREN] = "(";
  terminalMapped[Symbol::Terminal::RPAREN] = ")";
  terminalMapped[Symbol::Terminal::LBRACK] = "[";
  terminalMapped[Symbol::Terminal::RBRACK] = "]";
  terminalMapped[Symbol::Terminal::LBRACE] = "{";
  terminalMapped[Symbol::Terminal::RBRACE] = "}";
  terminalMapped[Symbol::Terminal::PERIOD] = ".";
  terminalMapped[Symbol::Terminal::PLUS] = "+";
  terminalMapped[Symbol::Terminal::MINUS] = "-";
  terminalMapped[Symbol::Terminal::MULT] = "*";
  terminalMapped[Symbol::Terminal::DIV] = "/";
  terminalMapped[Symbol::Terminal::EQ] = "=";
  terminalMapped[Symbol::Terminal::NEQ] = "<>";
  terminalMapped[Symbol::Terminal::LESSER] = "<";
  terminalMapped[Symbol::Terminal::GREATER] = ">";
  terminalMapped[Symbol::Terminal::LESSEREQ] = "<=";
  terminalMapped[Symbol::Terminal::GREATEREQ] = ">=";
  terminalMapped[Symbol::Terminal::AND] = "&";
  terminalMapped[Symbol::Terminal::OR] = "|";
  terminalMapped[Symbol::Terminal::ASSIGN] = ":=";
  terminalMapped[Symbol::Terminal::ID] = "id";
  terminalMapped[Symbol::Terminal::INTLIT] = "intlit";
  terminalMapped[Symbol::Terminal::FLOATLIT] = "floatlit";
  terminalMapped[Symbol::Terminal::NULLL] = "epsilon";
  terminalMapped[Symbol::Terminal::EOFF] = "";
}

/**
 * Prints helpful information for the user if they try to run the
 * program with a bad number of arguments
 */
void printHelp() {
  std::cout << "\n"
            << "You have entered an incorrect number of arguments."
            << "\n"
            << "\n"
            << "Please specify the file you wish to parse and"
            << "\n"
            << "optionally whether you want to flag for debugging."
            << "\n"
            << "\n"
            << "> parser <filename> -d"
            << "\n";
}

/**
 * There are two arguments: one mandatory and one optional
 *
 * The mandatory argument is the file to perform a parse on while the
 * optional flag -d indicates that the user wants to print debug info.,
 * namely the sequences of Tokens as the Scanner reads them
 */
int main(int argc, char** argv) {
  // The user has given us a bad number of args
  if (argc > 3 || argc < 1) {
    printHelp();
    return 0;
  }

  // Initialize the Parser with the given filename
  Parser parser(argv[1]);

  // Print debug info. if flagged
  if (argc > 2 && strcmp(argv[2], "-d") == 0) parser.printDebug = true;

  if (argc > 2 && strcmp(argv[2], "-d") != 0)
    std::cout << "\nPlease use \"-d\" as flag for debugging.\n";

  // Pass
  parser.parse();

  // Close all open files like
  parser.outFile.close();
  return 0;
}
