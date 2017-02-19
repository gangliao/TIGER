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
  addToParseTable(Symbol::Nonterminal::TYPE_DECLARATION_LIST,  // NOLINT
                  {Symbol::Terminal::FUNCTION,                 // NOLINT
                   Symbol::Terminal::IN,                       // NOLINT
                   Symbol::Terminal::VAR},                     // NOLINT
                  {Symbol::Terminal::NULLL});                  // NOLINT

  // 5: <var-declaration-list> -> <var-declaration> <var-declaration-list>
  addToParseTable(Symbol::Nonterminal::VAR_DECLARATION_LIST,     // NOLINT
                  {Symbol::Nonterminal::VAR_DECLARATION},        // NOLINT
                  {Symbol::Nonterminal::VAR_DECLARATION_LIST});  // NOLINT

  // 6: <var-declaration-list> -> NULL
  addToParseTable(Symbol::Nonterminal::VAR_DECLARATION_LIST,  // NOLINT
                  {Symbol::Terminal::FUNCTION,                // NOLINT
                   Symbol::Terminal::IN},                     // NOLINT
                  {Symbol::Terminal::NULLL});                 // NOLINT

  // 7: <funct-declaration-list> -> <funct-declaration> <funct-declaration-list>
  addToParseTable(Symbol::Nonterminal::FUNCT_DECLARATION_LIST,     // NOLINT
                  {Symbol::Terminal::FUNCTION},                    // NOLINT
                  {Symbol::Nonterminal::FUNCT_DECLARATION,         // NOLINT
                   Symbol::Nonterminal::FUNCT_DECLARATION_LIST});  // NOLINT

  // 8: <funct-declaration-list> -> NULL
  addToParseTable(Symbol::Nonterminal::FUNCT_DECLARATION_LIST,  // NOLINT
                  {Symbol::Terminal::IN},                       // NOLINT
                  {Symbol::Terminal::NULLL});                   // NOLINT

  // # type-declaration
  // 9: <type-declaration> -> type id = <type>;
  addToParseTable(Symbol::Nonterminal::TYPE_DECLARATION,  // NOLINT
                  {Symbol::Terminal::IN},                 // NOLINT
                  {Symbol::Terminal::TYPE,                // NOLINT
                   Symbol::Terminal::ID,                  // NOLINT
                   Symbol::Terminal::EQ,                  // NOLINT
                   Symbol::Nonterminal::TYPE_EXPR,        // NOLINT
                   Symbol::Terminal::SEMI});              // NOLINT

  // 10: <type> -> <type-id>
  addToParseTable(Symbol::Nonterminal::TYPE_EXPR,   // NOLINT
                  {Symbol::Terminal::INT,           // NOLINT
                   Symbol::Terminal::FLOAT},        // NOLINT
                  {Symbol::Nonterminal::TYPE_ID});  // NOLINT

  // 11: <type> -> array [INTLIT] of <type-id>
  addToParseTable(Symbol::Nonterminal::TYPE_EXPR,   // NOLINT
                  {Symbol::Terminal::ARRAY},        // NOLINT
                  {Symbol::Terminal::ARRAY,         // NOLINT
                   Symbol::Terminal::LBRACK,        // NOLINT
                   Symbol::Terminal::INTLIT,        // NOLINT
                   Symbol::Terminal::RBRACK,        // NOLINT
                   Symbol::Terminal::OF,            // NOLINT
                   Symbol::Nonterminal::TYPE_ID});  // NOLINT

  // 12: <type> -> id
  addToParseTable(Symbol::Nonterminal::TYPE_EXPR,  // NOLINT
                  {Symbol::Terminal::ID},          // NOLINT
                  {Symbol::Terminal::ID});         // NOLINT

  // 13: <type-id> -> int
  addToParseTable(Symbol::Nonterminal::TYPE_ID,  // NOLINT
                  {Symbol::Terminal::INT},       // NOLINT
                  {Symbol::Terminal::INT});      // NOLINT

  // 14: <type-id> -> float
  addToParseTable(Symbol::Nonterminal::TYPE_ID,  // NOLINT
                  {Symbol::Terminal::FLOAT},     // NOLINT
                  {Symbol::Terminal::FLOAT});    // NOLINT

  // # var-declaration
  // 15: <var-declaration> -> var <id-list> : <type> <optional-init>;
  addToParseTable(Symbol::Nonterminal::TYPE_ID,  // NOLINT
                  {Symbol::Terminal::FLOAT},     // NOLINT
                  {Symbol::Terminal::FLOAT});    // NOLINT

  // 16: <id-list> -> id <id-list-tail>
  addToParseTable(Symbol::Nonterminal::ID_LIST,          // NOLINT
                  {Symbol::Terminal::ID},                // NOLINT
                  {Symbol::Terminal::ID,                 // NOLINT
                   Symbol::Nonterminal::ID_LIST_TAIL});  // NOLINT

  // 17: <id-list-tail> -> , id <id-list-tail>
  addToParseTable(Symbol::Nonterminal::ID_LIST_TAIL,     // NOLINT
                  {Symbol::Terminal::COMMA},             // NOLINT
                  {Symbol::Terminal::COMMA,              // NOLINT
                   Symbol::Nonterminal::ID_LIST_TAIL});  // NOLINT

  // 18: <id-list-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::ID_LIST_TAIL,  // NOLINT
                  {Symbol::Terminal::COLON},          // NOLINT
                  {Symbol::Terminal::NULLL});         // NOLINT

  // 19: <optional-init> -> := <const>
  addToParseTable(Symbol::Nonterminal::OPTIONAL_INIT,  // NOLINT
                  {Symbol::Terminal::ASSIGN},          // NOLINT
                  {Symbol::Terminal::ASSIGN,           // NOLINT
                   Symbol::Nonterminal::CONST});       // NOLINT

  // 20: <optional-init> -> NULL
  addToParseTable(Symbol::Nonterminal::OPTIONAL_INIT,  // NOLINT
                  {Symbol::Terminal::SEMI},            // NOLINT
                  {Symbol::Terminal::NULLL});          // NOLINT

  // # funct-declaration
  // 21: <funct-declaration> -> function id (<param-list>) <ret-type> begin
  // <stat-seq> end;
  addToParseTable(Symbol::Nonterminal::FUNCT_DECLARATION,  // NOLINT
                  {Symbol::Terminal::FUNCTION},            // NOLINT
                  {Symbol::Terminal::FUNCTION,             // NOLINT
                   Symbol::Terminal::ID,                   // NOLINT
                   Symbol::Terminal::LPAREN,               // NOLINT
                   Symbol::Nonterminal::PARAM_LIST,        // NOLINT
                   Symbol::Terminal::RPAREN,               // NOLINT
                   Symbol::Nonterminal::RET_TYPE,          // NOLINT
                   Symbol::Terminal::BEGIN,                // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,          // NOLINT
                   Symbol::Terminal::END,                  // NOLINT
                   Symbol::Terminal::SEMI});               // NOLINT

  // 22: <param-list> -> <param> <param-list-tail>
  addToParseTable(Symbol::Nonterminal::PARAM_LIST,          // NOLINT
                  {Symbol::Terminal::ID},                   // NOLINT
                  {Symbol::Nonterminal::PARAM,              // NOLINT
                   Symbol::Nonterminal::PARAM_LIST_TAIL});  // NOLINT

  // 23: <param-list> -> NULL
  addToParseTable(Symbol::Nonterminal::PARAM_LIST,  // NOLINT
                  {Symbol::Terminal::RPAREN},       // NOLINT
                  {Symbol::Terminal::NULLL});       // NOLINT

  // 24: <param-list-tail> -> , <param> <param-list-tail>
  addToParseTable(Symbol::Nonterminal::PARAM_LIST_TAIL,     // NOLINT
                  {Symbol::Terminal::COMMA},                // NOLINT
                  {Symbol::Terminal::COMMA,                 // NOLINT
                   Symbol::Nonterminal::PARAM,              // NOLINT
                   Symbol::Nonterminal::PARAM_LIST_TAIL});  // NOLINT

  // 25: <param-list-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::PARAM_LIST_TAIL,  // NOLINT
                  {Symbol::Terminal::RPAREN},            // NOLINT
                  {Symbol::Terminal::NULLL});            // NOLINT

  // 26: <ret-type> -> : <type>
  addToParseTable(Symbol::Nonterminal::RET_TYPE,      // NOLINT
                  {Symbol::Terminal::COLON},          // NOLINT
                  {Symbol::Terminal::COLON,           // NOLINT
                   Symbol::Nonterminal::TYPE_EXPR});  // NOLINT

  // 27: <ret-type> -> NULL
  addToParseTable(Symbol::Nonterminal::RET_TYPE,  // NOLINT
                  {Symbol::Terminal::BEGIN},      // NOLINT
                  {Symbol::Terminal::NULLL});     // NOLINT

  // 28: <param> -> id : <type>
  addToParseTable(Symbol::Nonterminal::PARAM,         // NOLINT
                  {Symbol::Terminal::ID},             // NOLINT
                  {Symbol::Terminal::ID,              // NOLINT
                   Symbol::Terminal::COLON,           // NOLINT
                   Symbol::Nonterminal::TYPE_EXPR});  // NOLINT

  // # stat-seq
  // 29: <stat-seq> -> <stat> <stat-seq-tail>
  addToParseTable(Symbol::Nonterminal::STAT_SEQ,          // NOLINT
                  {Symbol::Terminal::IF,                  // NOLINT
                   Symbol::Terminal::WHILE,               // NOLINT
                   Symbol::Terminal::FOR,                 // NOLINT
                   Symbol::Terminal::BREAK,               // NOLINT
                   Symbol::Terminal::RETURN,              // NOLINT
                   Symbol::Terminal::LET,                 // NOLINT
                   Symbol::Terminal::ID},                 // NOLINT
                  {Symbol::Nonterminal::STAT,             // NOLINT
                   Symbol::Nonterminal::STAT_SEQ_TAIL});  // NOLINT

  // 30: <stat-seq-tail> -> <stat> <stat-seq-tail>
  addToParseTable(Symbol::Nonterminal::STAT_SEQ_TAIL,     // NOLINT
                  {Symbol::Terminal::IF,                  // NOLINT
                   Symbol::Terminal::WHILE,               // NOLINT
                   Symbol::Terminal::FOR,                 // NOLINT
                   Symbol::Terminal::BREAK,               // NOLINT
                   Symbol::Terminal::RETURN,              // NOLINT
                   Symbol::Terminal::LET,                 // NOLINT
                   Symbol::Terminal::ID},                 // NOLINT
                  {Symbol::Nonterminal::STAT,             // NOLINT
                   Symbol::Nonterminal::STAT_SEQ_TAIL});  // NOLINT

  // 31: <stat-seq-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::STAT_SEQ_TAIL,  // NOLINT
                  {Symbol::Terminal::ENDIF,            // NOLINT
                   Symbol::Terminal::ELSE,             // NOLINT
                   Symbol::Terminal::ENDDO,            // NOLINT
                   Symbol::Terminal::END},             // NOLINT
                  {Symbol::Terminal::NULLL});          // NOLINT

  // # stat
  // 32: <stat> -> if <expr> then <stat-seq> <stat-if-tail>
  addToParseTable(Symbol::Nonterminal::STAT,             // NOLINT
                  {Symbol::Terminal::IF},                // NOLINT
                  {Symbol::Terminal::IF,                 // NOLINT
                   Symbol::Nonterminal::EXPR,            // NOLINT
                   Symbol::Terminal::THEN,               // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,        // NOLINT
                   Symbol::Nonterminal::STAT_IF_TAIL});  // NOLINT

  // 33: <stat-if-tail> -> else <stat-seq> endif;
  addToParseTable(Symbol::Nonterminal::STAT_IF_TAIL,  // NOLINT
                  {Symbol::Terminal::ELSE},           // NOLINT
                  {Symbol::Terminal::ELSE,            // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,     // NOLINT
                   Symbol::Terminal::ENDIF});         // NOLINT

  // 34: <stat-if-tail> -> endif;
  addToParseTable(Symbol::Nonterminal::STAT_IF_TAIL,  // NOLINT
                  {Symbol::Terminal::ENDIF},          // NOLINT
                  {Symbol::Terminal::ENDIF});         // NOLINT

  // 35: <stat> -> id <stat-funct-or-assign>
  addToParseTable(Symbol::Nonterminal::STAT,                     // NOLINT
                  {Symbol::Terminal::ID},                        // NOLINT
                  {Symbol::Terminal::ID,                         // NOLINT
                   Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN});  // NOLINT

  // 36: <stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
  addToParseTable(Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN,  // NOLINT
                  {Symbol::Terminal::LBRACK},                 // NOLINT
                  {Symbol::Nonterminal::LVALUE_TAIL,          // NOLINT
                   Symbol::Terminal::ASSIGN,                  // NOLINT
                   Symbol::Nonterminal::STAT_ASSIGN,          // NOLINT
                   Symbol::Terminal::SEMI});                  // NOLINT

  // 37: <stat-funct-or-assign> -> (<expr-list>);
  addToParseTable(Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN,  // NOLINT
                  {Symbol::Terminal::LBRACK},                 // NOLINT
                  {Symbol::Terminal::LPAREN,                  // NOLINT
                   Symbol::Nonterminal::EXPR_LIST,            // NOLINT
                   Symbol::Terminal::RPAREN,                  // NOLINT
                   Symbol::Terminal::SEMI});                  // NOLINT

  // 38: <stat-assign> -> id <stat-assign-stuff>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN,           // NOLINT
                  {Symbol::Terminal::ID},                     // NOLINT
                  {Symbol::Terminal::ID,                      // NOLINT
                   Symbol::Nonterminal::STAT_ASSIGN_STUFF});  // NOLINT

  // 39: <stat-assign> -> (<expr>) <stat-assign-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN,          // NOLINT
                  {Symbol::Terminal::LPAREN},                // NOLINT
                  {Symbol::Terminal::LPAREN,                 // NOLINT
                   Symbol::Nonterminal::EXPR,                // NOLINT
                   Symbol::Terminal::RPAREN,                 // NOLINT
                   Symbol::Nonterminal::STAT_ASSIGN_TAIL});  // NOLINT

  // 40: <stat-assign> -> <const> <stat-assign-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN,          // NOLINT
                  {Symbol::Terminal::INTLIT,                 // NOLINT
                   Symbol::Terminal::FLOATLIT},              // NOLINT
                  {Symbol::Nonterminal::CONST,               // NOLINT
                   Symbol::Nonterminal::STAT_ASSIGN_TAIL});  // NOLINT

  // 41: <stat-assign-stuff> -> (<expr-list>)
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN_STUFF,  // NOLINT
                  {Symbol::Terminal::LPAREN},              // NOLINT
                  {Symbol::Terminal::LPAREN,               // NOLINT
                   Symbol::Nonterminal::EXPR_LIST,         // NOLINT
                   Symbol::Terminal::RPAREN});             // NOLINT

  // 42: <stat-assign-stuff> -> <lvalue-tail> <stat-assign-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN_STUFF,    // NOLINT
                  {Symbol::Terminal::SEMI,                   // NOLINT
                   Symbol::Terminal::LBRACK,                 // NOLINT
                   Symbol::Terminal::PLUS,                   // NOLINT
                   Symbol::Terminal::MINUS,                  // NOLINT
                   Symbol::Terminal::MULT,                   // NOLINT
                   Symbol::Terminal::DIV,                    // NOLINT
                   Symbol::Terminal::EQ,                     // NOLINT
                   Symbol::Terminal::NEQ,                    // NOLINT
                   Symbol::Terminal::LESSER,                 // NOLINT
                   Symbol::Terminal::GREATER,                // NOLINT
                   Symbol::Terminal::LESSEREQ,               // NOLINT
                   Symbol::Terminal::GREATEREQ,              // NOLINT
                   Symbol::Terminal::AND,                    // NOLINT
                   Symbol::Terminal::OR,                     // NOLINT
                   Symbol::Terminal::ASSIGN},                // NOLINT
                  {Symbol::Nonterminal::LVALUE_TAIL,         // NOLINT
                   Symbol::Nonterminal::STAT_ASSIGN_TAIL});  // NOLINT

  // 43: <stat-assign-tail> -> <expr-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN_TAIL,  // NOLINT
                  {Symbol::Terminal::OR},                 // NOLINT
                  {Symbol::Nonterminal::EXPR_TAIL});      // NOLINT

  // 44: <stat-assign-tail> -> <OR-expr-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN_TAIL,  // NOLINT
                  {Symbol::Terminal::AND},                // NOLINT
                  {Symbol::Nonterminal::OR_EXPR_TAIL});   // NOLINT

  // 45: <stat-assign-tail> -> <AND-expr-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN_TAIL,  // NOLINT
                  {Symbol::Terminal::EQ,                  // NOLINT
                   Symbol::Terminal::NEQ,                 // NOLINT
                   Symbol::Terminal::LESSER,              // NOLINT
                   Symbol::Terminal::GREATER,             // NOLINT
                   Symbol::Terminal::LESSEREQ,            // NOLINT
                   Symbol::Terminal::GREATEREQ},          // NOLINT
                  {Symbol::Nonterminal::AND_EXPR_TAIL});  // NOLINT

  // 46: <stat-assign-tail> -> <compare-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN_TAIL,  // NOLINT
                  {Symbol::Terminal::PLUS,                // NOLINT
                   Symbol::Terminal::MINUS},              // NOLINT
                  {Symbol::Nonterminal::COMPARE_TAIL});   // NOLINT

  // 47: <stat-assign-tail> -> <term-tail>
  addToParseTable(Symbol::Nonterminal::STAT_ASSIGN_TAIL,  // NOLINT
                  {Symbol::Terminal::MULT,                // NOLINT
                   Symbol::Terminal::DIV},                // NOLINT
                  {Symbol::Nonterminal::TERM_TAIL});      // NOLINT

  // 48: <stat> -> while <expr> do <stat-seq> enddo;
  addToParseTable(Symbol::Nonterminal::STAT,       // NOLINT
                  {Symbol::Terminal::WHILE},       // NOLINT
                  {Symbol::Terminal::WHILE,        // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Terminal::DO,           // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,  // NOLINT
                   Symbol::Terminal::ENDDO,        // NOLINT
                   Symbol::Terminal::SEMI});       // NOLINT

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
