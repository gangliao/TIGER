/**
 * Parser
 *
 * @author: Gatech Buzz
 */

#include "Parser.hpp"

Parser::Parser(std::string fileName) : scanner(fileName) {
  // push tiger program into parse stack
  parseStack.push(Symbol::Terminal::EOFF);
  parseStack.push(Symbol::Nonterminal::TIGER_PROGRAM);
  // initialize terminal map
  initializeTerminalMapped();
  // file name
  globalFileName = fileName;
  // count parse errors
  numErrors = 0;
  // init and create parse table
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
                  {Symbol::Action::InitializeScope,           // NOLINT
                   Symbol::Terminal::LET,                     // NOLINT
                   Symbol::Nonterminal::DECLARATION_SEGMENT,  // NOLINT
                   Symbol::Terminal::IN,                      // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,             // NOLINT
                   Symbol::Terminal::END,                     // NOLINT
                   Symbol::Action::FinalizeScope});           // NOLINT

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
                  {Symbol::Terminal::VAR},                       // NOLINT
                  {Symbol::Nonterminal::VAR_DECLARATION,         // NOLINT
                   Symbol::Nonterminal::VAR_DECLARATION_LIST});  // NOLINT

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
                  {Symbol::Terminal::TYPE},               // NOLINT
                  {Symbol::Action::MakeTypesBegin,        // NOLINT
                   Symbol::Terminal::TYPE,                // NOLINT
                   Symbol::Terminal::ID,                  // NOLINT
                   Symbol::Terminal::EQ,                  // NOLINT
                   Symbol::Nonterminal::TYPE_EXPR,        // NOLINT
                   Symbol::Terminal::SEMI,                // NOLINT
                   Symbol::Action::MakeTypesEnd});        // NOLINT

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
  addToParseTable(Symbol::Nonterminal::VAR_DECLARATION,  // NOLINT
                  {Symbol::Terminal::VAR},               // NOLINT
                  {Symbol::Action::MakeVariablesBegin,   // NOLINT
                   Symbol::Terminal::VAR,                // NOLINT
                   Symbol::Nonterminal::ID_LIST,         // NOLINT
                   Symbol::Terminal::COLON,              // NOLINT
                   Symbol::Nonterminal::TYPE_EXPR,       // NOLINT
                   Symbol::Nonterminal::OPTIONAL_INIT,   // NOLINT
                   Symbol::Terminal::SEMI,               // NOLINT
                   Symbol::Action::MakeVariablesEnd});   // NOLINT

  // 16: <id-list> -> id <id-list-tail>
  addToParseTable(Symbol::Nonterminal::ID_LIST,          // NOLINT
                  {Symbol::Terminal::ID},                // NOLINT
                  {Symbol::Terminal::ID,                 // NOLINT
                   Symbol::Nonterminal::ID_LIST_TAIL});  // NOLINT

  // 17: <id-list-tail> -> , id <id-list-tail>
  addToParseTable(Symbol::Nonterminal::ID_LIST_TAIL,     // NOLINT
                  {Symbol::Terminal::COMMA},             // NOLINT
                  {Symbol::Terminal::COMMA,              // NOLINT
                   Symbol::Terminal::ID,                 // NOLINT
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
                  {Symbol::Action::MakeFunctionsBegin,     // NOLINT
                   Symbol::Terminal::FUNCTION,             // NOLINT
                   Symbol::Terminal::ID,                   // NOLINT
                   Symbol::Terminal::LPAREN,               // NOLINT
                   Symbol::Nonterminal::PARAM_LIST,        // NOLINT
                   Symbol::Terminal::RPAREN,               // NOLINT
                   Symbol::Nonterminal::RET_TYPE,          // NOLINT
                   Symbol::Action::MakeFunctionsEnd,       // NOLINT
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
                   Symbol::Action::MakeExprBegin,        // NOLINT
                   Symbol::Nonterminal::EXPR,            // NOLINT
                   Symbol::Action::MakeExprEnd,          // NOLINT
                   Symbol::Terminal::THEN,               // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,        // NOLINT
                   Symbol::Nonterminal::STAT_IF_TAIL});  // NOLINT

  // 33: <stat-if-tail> -> else <stat-seq> endif;
  addToParseTable(Symbol::Nonterminal::STAT_IF_TAIL,  // NOLINT
                  {Symbol::Terminal::ELSE},           // NOLINT
                  {Symbol::Terminal::ELSE,            // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,     // NOLINT
                   Symbol::Terminal::ENDIF,           // NOLINT
                   Symbol::Terminal::SEMI});          // NOLINT

  // 34: <stat-if-tail> -> endif;
  addToParseTable(Symbol::Nonterminal::STAT_IF_TAIL,  // NOLINT
                  {Symbol::Terminal::ENDIF},          // NOLINT
                  {Symbol::Terminal::ENDIF,           // NOLINT
                   Symbol::Terminal::SEMI});          // NOLINT

  // 35: <stat> -> id <stat-funct-or-assign>
  addToParseTable(Symbol::Nonterminal::STAT,                     // NOLINT
                  {Symbol::Terminal::ID},                        // NOLINT
                  {Symbol::Action::MakeAssignBegin,              // NOLINT
                   Symbol::Terminal::ID,                         // NOLINT
                   Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN});  // NOLINT

  // 36: <stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
  addToParseTable(Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN,  // NOLINT
                  {Symbol::Terminal::LBRACK,                  // NOLINT
                   Symbol::Terminal::ASSIGN},                 // NOLINT
                  {Symbol::Nonterminal::LVALUE_TAIL,          // NOLINT
                   Symbol::Terminal::ASSIGN,                  // NOLINT
                   Symbol::Nonterminal::STAT_ASSIGN,          // NOLINT
                   Symbol::Action::MakeAssignEnd,             // NOLINT
                   Symbol::Terminal::SEMI});                  // NOLINT

  // 37: <stat-funct-or-assign> -> (<expr-list>);
  addToParseTable(Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN,  // NOLINT
                  {Symbol::Terminal::LPAREN},                 // NOLINT
                  {Symbol::Terminal::LPAREN,                  // NOLINT
                   Symbol::Nonterminal::EXPR_LIST,            // NOLINT
                   Symbol::Terminal::RPAREN,                  // NOLINT
                   Symbol::Action::MakeAssignEnd,             // NOLINT
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
                   Symbol::Terminal::DIV,                 // NOLINT
                   Symbol::Terminal::SEMI},               // NOLINT
                  {Symbol::Nonterminal::TERM_TAIL});      // NOLINT

  // 48: <stat> -> while <expr> do <stat-seq> enddo;
  addToParseTable(Symbol::Nonterminal::STAT,       // NOLINT
                  {Symbol::Terminal::WHILE},       // NOLINT
                  {Symbol::Terminal::WHILE,        // NOLINT
                   Symbol::Action::MakeExprBegin,  // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Action::MakeExprEnd,    // NOLINT
                   Symbol::Terminal::DO,           // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,  // NOLINT
                   Symbol::Terminal::ENDDO,        // NOLINT
                   Symbol::Terminal::SEMI});       // NOLINT

  // 49: <stat> -> for id := <expr> to <expr> do <stat-seq> enddo;
  addToParseTable(Symbol::Nonterminal::STAT,       // NOLINT
                  {Symbol::Terminal::FOR},         // NOLINT
                  {Symbol::Terminal::FOR,          // NOLINT
                   Symbol::Terminal::ID,           // NOLINT
                   Symbol::Terminal::ASSIGN,       // NOLINT
                   Symbol::Action::MakeExprBegin,  // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Action::MakeExprEnd,    // NOLINT
                   Symbol::Terminal::TO,           // NOLINT
                   Symbol::Action::MakeExprBegin,  // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Action::MakeExprEnd,    // NOLINT
                   Symbol::Terminal::DO,           // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,  // NOLINT
                   Symbol::Terminal::ENDDO,        // NOLINT
                   Symbol::Terminal::SEMI});       // NOLINT

  // 50: <stat> -> break;
  addToParseTable(Symbol::Nonterminal::STAT,  // NOLINT
                  {Symbol::Terminal::BREAK},  // NOLINT
                  {Symbol::Terminal::BREAK,   // NOLINT
                   Symbol::Terminal::SEMI});  // NOLINT

  // 51: <stat> -> return <expr>;
  addToParseTable(Symbol::Nonterminal::STAT,       // NOLINT
                  {Symbol::Terminal::RETURN},      // NOLINT
                  {Symbol::Terminal::RETURN,       // NOLINT
                   Symbol::Action::MakeExprBegin,  // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Action::MakeExprEnd,    // NOLINT
                   Symbol::Terminal::SEMI});       // NOLINT

  // 52: <stat> -> let <declaration-segment> in <stat-seq> end
  addToParseTable(Symbol::Nonterminal::STAT,                  // NOLINT
                  {Symbol::Terminal::LET},                    // NOLINT
                  {Symbol::Terminal::LET,                     // NOLINT
                   Symbol::Nonterminal::DECLARATION_SEGMENT,  // NOLINT
                   Symbol::Terminal::IN,                      // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,             // NOLINT
                   Symbol::Terminal::END});                   // NOLINT

  // # expr
  // 53: <expr> -> <OR-expr> <expr-tail>
  addToParseTable(Symbol::Nonterminal::EXPR,          // NOLINT
                  {Symbol::Terminal::ID,              // NOLINT
                   Symbol::Terminal::LPAREN,          // NOLINT
                   Symbol::Terminal::INTLIT,          // NOLINT
                   Symbol::Terminal::FLOATLIT},       // NOLINT
                  {Symbol::Nonterminal::OR_EXPR,      // NOLINT
                   Symbol::Nonterminal::EXPR_TAIL});  // NOLINT

  // 54: <expr-tail> -> <OR-op> <OR-expr> <expr-tail>
  addToParseTable(Symbol::Nonterminal::EXPR_TAIL,     // NOLINT
                  {Symbol::Terminal::OR},             // NOLINT
                  {Symbol::Nonterminal::OR_OP,        // NOLINT
                   Symbol::Nonterminal::OR_EXPR,      // NOLINT
                   Symbol::Nonterminal::EXPR_TAIL});  // NOLINT

  // 55: <expr-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::EXPR_TAIL,  // NOLINT
                  {Symbol::Terminal::DO,           // NOLINT
                   Symbol::Terminal::THEN,         // NOLINT
                   Symbol::Terminal::TO,           // NOLINT
                   Symbol::Terminal::SEMI,         // NOLINT
                   Symbol::Terminal::COMMA,        // NOLINT
                   Symbol::Terminal::RPAREN,       // NOLINT
                   Symbol::Terminal::RBRACK},      // NOLINT
                  {Symbol::Terminal::NULLL});      // NOLINT

  // 56: <OR-expr> -> <AND-expr> <OR-expr-tail>
  addToParseTable(Symbol::Nonterminal::OR_EXPR,          // NOLINT
                  {Symbol::Terminal::ID,                 // NOLINT
                   Symbol::Terminal::LPAREN,             // NOLINT
                   Symbol::Terminal::INTLIT,             // NOLINT
                   Symbol::Terminal::FLOATLIT},          // NOLINT
                  {Symbol::Nonterminal::AND_EXPR,        // NOLINT
                   Symbol::Nonterminal::OR_EXPR_TAIL});  // NOLINT

  // 57: <OR-expr-tail> -> <AND-op> <AND-expr> <OR-expr-tail>
  addToParseTable(Symbol::Nonterminal::OR_EXPR_TAIL,     // NOLINT
                  {Symbol::Terminal::AND},               // NOLINT
                  {Symbol::Nonterminal::AND_OP,          // NOLINT
                   Symbol::Nonterminal::AND_EXPR,        // NOLINT
                   Symbol::Nonterminal::OR_EXPR_TAIL});  // NOLINT

  // 58: <OR-expr-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::OR_EXPR_TAIL,  // NOLINT
                  {Symbol::Terminal::OR,              // NOLINT
                   Symbol::Terminal::DO,              // NOLINT
                   Symbol::Terminal::THEN,            // NOLINT
                   Symbol::Terminal::TO,              // NOLINT
                   Symbol::Terminal::SEMI,            // NOLINT
                   Symbol::Terminal::COMMA,           // NOLINT
                   Symbol::Terminal::RPAREN,          // NOLINT
                   Symbol::Terminal::RBRACK},         // NOLINT
                  {Symbol::Terminal::NULLL});         // NOLINT

  // 59: <AND-expr> -> <compare> <AND-expr-tail>
  addToParseTable(Symbol::Nonterminal::AND_EXPR,          // NOLINT
                  {Symbol::Terminal::ID,                  // NOLINT
                   Symbol::Terminal::LPAREN,              // NOLINT
                   Symbol::Terminal::INTLIT,              // NOLINT
                   Symbol::Terminal::FLOATLIT},           // NOLINT
                  {Symbol::Nonterminal::COMPARE,          // NOLINT
                   Symbol::Nonterminal::AND_EXPR_TAIL});  // NOLINT

  // 60: <AND-expr-tail> -> <compare-op> <compare> <AND-expr-tail>
  addToParseTable(Symbol::Nonterminal::AND_EXPR_TAIL,     // NOLINT
                  {Symbol::Terminal::EQ,                  // NOLINT
                   Symbol::Terminal::NEQ,                 // NOLINT
                   Symbol::Terminal::LESSER,              // NOLINT
                   Symbol::Terminal::GREATER,             // NOLINT
                   Symbol::Terminal::LESSEREQ,            // NOLINT
                   Symbol::Terminal::GREATEREQ},          // NOLINT
                  {Symbol::Nonterminal::COMPARE_OP,       // NOLINT
                   Symbol::Nonterminal::COMPARE,          // NOLINT
                   Symbol::Nonterminal::AND_EXPR_TAIL});  // NOLINT

  // 61: <AND-expr-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::AND_EXPR_TAIL,  // NOLINT
                  {Symbol::Terminal::OR,               // NOLINT
                   Symbol::Terminal::AND,              // NOLINT
                   Symbol::Terminal::DO,               // NOLINT
                   Symbol::Terminal::THEN,             // NOLINT
                   Symbol::Terminal::TO,               // NOLINT
                   Symbol::Terminal::SEMI,             // NOLINT
                   Symbol::Terminal::COMMA,            // NOLINT
                   Symbol::Terminal::RPAREN,           // NOLINT
                   Symbol::Terminal::RBRACK},          // NOLINT
                  {Symbol::Terminal::NULLL});          // NOLINT

  // 62: <compare> -> <term> <compare-tail>
  addToParseTable(Symbol::Nonterminal::COMPARE,          // NOLINT
                  {Symbol::Terminal::ID,                 // NOLINT
                   Symbol::Terminal::LPAREN,             // NOLINT
                   Symbol::Terminal::INTLIT,             // NOLINT
                   Symbol::Terminal::FLOATLIT},          // NOLINT
                  {Symbol::Nonterminal::TERM,            // NOLINT
                   Symbol::Nonterminal::COMPARE_TAIL});  // NOLINT

  // 63: <compare-tail> -> <add-op> <term> <compare-tail>
  addToParseTable(Symbol::Nonterminal::COMPARE_TAIL,     // NOLINT
                  {Symbol::Terminal::PLUS,               // NOLINT
                   Symbol::Terminal::MINUS},             // NOLINT
                  {Symbol::Nonterminal::ADD_OP,          // NOLINT
                   Symbol::Nonterminal::TERM,            // NOLINT
                   Symbol::Nonterminal::COMPARE_TAIL});  // NOLINT

  // 64: <compare-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::COMPARE_TAIL,  // NOLINT
                  {Symbol::Terminal::OR,              // NOLINT
                   Symbol::Terminal::AND,             // NOLINT
                   Symbol::Terminal::EQ,              // NOLINT
                   Symbol::Terminal::NEQ,             // NOLINT
                   Symbol::Terminal::LESSER,          // NOLINT
                   Symbol::Terminal::GREATER,         // NOLINT
                   Symbol::Terminal::LESSEREQ,        // NOLINT
                   Symbol::Terminal::GREATEREQ,       // NOLINT
                   Symbol::Terminal::DO,              // NOLINT
                   Symbol::Terminal::THEN,            // NOLINT
                   Symbol::Terminal::TO,              // NOLINT
                   Symbol::Terminal::SEMI,            // NOLINT
                   Symbol::Terminal::COMMA,           // NOLINT
                   Symbol::Terminal::RPAREN,          // NOLINT
                   Symbol::Terminal::RBRACK},         // NOLINT
                  {Symbol::Terminal::NULLL});         // NOLINT

  // 65: <term> -> <factor> <term-tail>
  addToParseTable(Symbol::Nonterminal::TERM,          // NOLINT
                  {Symbol::Terminal::ID,              // NOLINT
                   Symbol::Terminal::LPAREN,          // NOLINT
                   Symbol::Terminal::INTLIT,          // NOLINT
                   Symbol::Terminal::FLOATLIT},       // NOLINT
                  {Symbol::Nonterminal::FACTOR,       // NOLINT
                   Symbol::Nonterminal::TERM_TAIL});  // NOLINT

  // 66: <term-tail> -> <mul-op> <factor> <term-tail>
  addToParseTable(Symbol::Nonterminal::TERM_TAIL,     // NOLINT
                  {Symbol::Terminal::MULT,            // NOLINT
                   Symbol::Terminal::DIV},            // NOLINT
                  {Symbol::Nonterminal::MUL_OP,       // NOLINT
                   Symbol::Nonterminal::FACTOR,       // NOLINT
                   Symbol::Nonterminal::TERM_TAIL});  // NOLINT

  // 67: <term-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::TERM_TAIL,  // NOLINT
                  {Symbol::Terminal::OR,           // NOLINT
                   Symbol::Terminal::AND,          // NOLINT
                   Symbol::Terminal::EQ,           // NOLINT
                   Symbol::Terminal::NEQ,          // NOLINT
                   Symbol::Terminal::LESSER,       // NOLINT
                   Symbol::Terminal::GREATER,      // NOLINT
                   Symbol::Terminal::LESSEREQ,     // NOLINT
                   Symbol::Terminal::GREATEREQ,    // NOLINT
                   Symbol::Terminal::PLUS,         // NOLINT
                   Symbol::Terminal::MINUS,        // NOLINT
                   Symbol::Terminal::DO,           // NOLINT
                   Symbol::Terminal::THEN,         // NOLINT
                   Symbol::Terminal::TO,           // NOLINT
                   Symbol::Terminal::SEMI,         // NOLINT
                   Symbol::Terminal::COMMA,        // NOLINT
                   Symbol::Terminal::RPAREN,       // NOLINT
                   Symbol::Terminal::RBRACK},      // NOLINT
                  {Symbol::Terminal::NULLL});      // NOLINT

  // 68: <factor> -> (<expr>)
  addToParseTable(Symbol::Nonterminal::FACTOR,     // NOLINT
                  {Symbol::Terminal::LPAREN},      // NOLINT
                  {Symbol::Terminal::LPAREN,       // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Terminal::RPAREN});     // NOLINT

  // 69: <factor> -> <const>
  addToParseTable(Symbol::Nonterminal::FACTOR,    // NOLINT
                  {Symbol::Terminal::INTLIT,      // NOLINT
                   Symbol::Terminal::FLOATLIT},   // NOLINT
                  {Symbol::Nonterminal::CONST});  // NOLINT

  // 70: <factor> -> <lvalue>
  addToParseTable(Symbol::Nonterminal::FACTOR,     // NOLINT
                  {Symbol::Terminal::ID},          // NOLINT
                  {Symbol::Nonterminal::LVALUE});  // NOLINT

  // # const
  // 71: <const> -> INTLIT
  addToParseTable(Symbol::Nonterminal::CONST,   // NOLINT
                  {Symbol::Terminal::INTLIT},   // NOLINT
                  {Symbol::Terminal::INTLIT});  // NOLINT

  // 72: <const> -> FLOATLIT
  addToParseTable(Symbol::Nonterminal::CONST,     // NOLINT
                  {Symbol::Terminal::FLOATLIT},   // NOLINT
                  {Symbol::Terminal::FLOATLIT});  // NOLINT

  // # binary-operator
  // 73: <OR-op> -> |
  addToParseTable(Symbol::Nonterminal::OR_OP,  // NOLINT
                  {Symbol::Terminal::OR},      // NOLINT
                  {Symbol::Terminal::OR});     // NOLINT

  // 74: <AND-op> -> &
  addToParseTable(Symbol::Nonterminal::AND_OP,  // NOLINT
                  {Symbol::Terminal::AND},      // NOLINT
                  {Symbol::Terminal::AND});     // NOLINT

  // 75: <compare-op> -> <=
  addToParseTable(Symbol::Nonterminal::COMPARE_OP,  // NOLINT
                  {Symbol::Terminal::LESSEREQ},     // NOLINT
                  {Symbol::Terminal::LESSEREQ});    // NOLINT

  // 76: <compare-op> -> >=
  addToParseTable(Symbol::Nonterminal::COMPARE_OP,  // NOLINT
                  {Symbol::Terminal::GREATEREQ},    // NOLINT
                  {Symbol::Terminal::GREATEREQ});   // NOLINT

  // 77: <compare-op> -> <
  addToParseTable(Symbol::Nonterminal::COMPARE_OP,  // NOLINT
                  {Symbol::Terminal::LESSER},       // NOLINT
                  {Symbol::Terminal::LESSER});      // NOLINT

  // 78: <compare-op> -> >
  addToParseTable(Symbol::Nonterminal::COMPARE_OP,  // NOLINT
                  {Symbol::Terminal::GREATER},      // NOLINT
                  {Symbol::Terminal::GREATER});     // NOLINT

  // 79: <compare-op> -> <>
  addToParseTable(Symbol::Nonterminal::COMPARE_OP,  // NOLINT
                  {Symbol::Terminal::NEQ},          // NOLINT
                  {Symbol::Terminal::NEQ});         // NOLINT

  // 80: <compare-op> -> =
  addToParseTable(Symbol::Nonterminal::COMPARE_OP,  // NOLINT
                  {Symbol::Terminal::EQ},           // NOLINT
                  {Symbol::Terminal::EQ});          // NOLINT

  // 81: <add-op> -> -
  addToParseTable(Symbol::Nonterminal::ADD_OP,  // NOLINT
                  {Symbol::Terminal::MINUS},    // NOLINT
                  {Symbol::Terminal::MINUS});   // NOLINT

  // 82: <add-op> -> +
  addToParseTable(Symbol::Nonterminal::ADD_OP,  // NOLINT
                  {Symbol::Terminal::PLUS},     // NOLINT
                  {Symbol::Terminal::PLUS});    // NOLINT

  // 83: <mul-op> -> /
  addToParseTable(Symbol::Nonterminal::MUL_OP,  // NOLINT
                  {Symbol::Terminal::DIV},      // NOLINT
                  {Symbol::Terminal::DIV});     // NOLINT

  // 84: <mul-op> -> *
  addToParseTable(Symbol::Nonterminal::MUL_OP,  // NOLINT
                  {Symbol::Terminal::MULT},     // NOLINT
                  {Symbol::Terminal::MULT});    // NOLINT

  // 85: <expr-list> -> NULL
  addToParseTable(Symbol::Nonterminal::EXPR_LIST,  // NOLINT
                  {Symbol::Terminal::RPAREN},      // NOLINT
                  {Symbol::Terminal::NULLL});      // NOLINT

  // 86: <expr-list> -> <expr> <expr-list-tail>
  addToParseTable(Symbol::Nonterminal::EXPR_LIST,          // NOLINT
                  {Symbol::Terminal::ID,                   // NOLINT
                   Symbol::Terminal::LPAREN,               // NOLINT
                   Symbol::Terminal::INTLIT,               // NOLINT
                   Symbol::Terminal::FLOATLIT},            // NOLINT
                  {Symbol::Action::MakeExprBegin,          // NOLINT
                   Symbol::Nonterminal::EXPR,              // NOLINT
                   Symbol::Action::MakeExprEnd,            // NOLINT
                   Symbol::Nonterminal::EXPR_LIST_TAIL});  // NOLINT

  // 87: <expr-list-tail> -> , <expr> <expr-list-tail>
  addToParseTable(Symbol::Nonterminal::EXPR_LIST_TAIL,     // NOLINT
                  {Symbol::Terminal::COMMA},               // NOLINT
                  {Symbol::Terminal::COMMA,                // NOLINT
                   Symbol::Action::MakeExprBegin,          // NOLINT
                   Symbol::Nonterminal::EXPR,              // NOLINT
                   Symbol::Action::MakeExprEnd,            // NOLINT
                   Symbol::Nonterminal::EXPR_LIST_TAIL});  // NOLINT

  // 88: <expr-list-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::EXPR_LIST_TAIL,  // NOLINT
                  {Symbol::Terminal::RPAREN},           // NOLINT
                  {Symbol::Terminal::NULLL});           // NOLINT

  // 89: <lvalue> -> id <lvalue-tail>
  addToParseTable(
      Symbol::Nonterminal::LVALUE,                                // NOLINT
      {Symbol::Terminal::ID},                                     // NOLINT
      {Symbol::Terminal::ID, Symbol::Nonterminal::LVALUE_TAIL});  // NOLINT

  // 90: <lvalue-tail> -> [<expr>]
  addToParseTable(Symbol::Nonterminal::LVALUE_TAIL,  // NOLINT
                  {Symbol::Terminal::LBRACK},        // NOLINT
                  {Symbol::Terminal::LBRACK,         // NOLINT
                   Symbol::Action::MakeExprBegin,    // NOLINT
                   Symbol::Nonterminal::EXPR,        // NOLINT
                   Symbol::Action::MakeExprEnd,      // NOLINT
                   Symbol::Terminal::RBRACK});       // NOLINT

  // 91: <lvalue-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::LVALUE_TAIL,  // NOLINT
                  {Symbol::Terminal::DO,             // NOLINT
                   Symbol::Terminal::THEN,           // NOLINT
                   Symbol::Terminal::TO,             // NOLINT
                   Symbol::Terminal::SEMI,           // NOLINT
                   Symbol::Terminal::COMMA,          // NOLINT
                   Symbol::Terminal::RPAREN,         // NOLINT
                   Symbol::Terminal::RBRACK,         // NOLINT
                   Symbol::Terminal::OR,             // NOLINT
                   Symbol::Terminal::AND,            // NOLINT
                   Symbol::Terminal::EQ,             // NOLINT
                   Symbol::Terminal::NEQ,            // NOLINT
                   Symbol::Terminal::LESSER,         // NOLINT
                   Symbol::Terminal::GREATER,        // NOLINT
                   Symbol::Terminal::LESSEREQ,       // NOLINT
                   Symbol::Terminal::GREATEREQ,      // NOLINT
                   Symbol::Terminal::PLUS,           // NOLINT
                   Symbol::Terminal::MINUS,          // NOLINT
                   Symbol::Terminal::MULT,           // NOLINT
                   Symbol::Terminal::DIV,            // NOLINT
                   Symbol::Terminal::ASSIGN},        // NOLINT
                  {Symbol::Terminal::NULLL});        // NOLINT
}

void Parser::error(int expr, TokenPair* word) {
  std::cout << globalFileName << " line " << scanner.getCurrLine() << ": "
            << scanner.getPrefix()
            << " doesn't support token: " << word->getTokenString()
            << std::endl;
}

std::vector<TokenPair> Parser::cvt2PostExpr(std::vector<TokenPair>& tempBuffer,
                                            size_t index) {
  // TODO(gangliao): consider array
  // have a bug when expr includes ()
  auto prec = [&](int op) -> int {
    switch (op) {
      case Symbol::Terminal::PLUS:
      case Symbol::Terminal::MINUS:
        return 1;
      case Symbol::Terminal::MULT:
      case Symbol::Terminal::DIV:
        return 2;
      default:
        return 0;
    }
  };

  /// expression operator stack
  std::stack<TokenPair> stack;
  std::vector<TokenPair> expr;
  expr.reserve(tempBuffer.size());

  for (size_t i = index; i < tempBuffer.size(); ++i) {
    if (tempBuffer[i].getTokenType().getValue() == Symbol::Terminal::ID ||
        tempBuffer[i].getTokenType().getValue() == Symbol::Terminal::INTLIT ||
        tempBuffer[i].getTokenType().getValue() == Symbol::Terminal::FLOATLIT) {
      if (tempBuffer[i].getTokenType().getValue() == Symbol::Terminal::ID) {
        // semantic check
        g_SymbolTable[currentLevel]->lookup(Entry::Variables,
                                            tempBuffer[i].getTokenString());
      }
      expr.push_back(tempBuffer[i]);
    } else if (tempBuffer[i].getTokenType().getValue() ==
               Symbol::Terminal::LPAREN) {
      stack.push(tempBuffer[i]);
    } else if (tempBuffer[i].getTokenType().getValue() ==
               Symbol::Terminal::RPAREN) {
      while (!stack.empty() &&
             stack.top().getTokenType().getValue() !=
                 Symbol::Terminal::LPAREN) {
        TokenPair temp = stack.top();
        expr.push_back(temp);
        stack.pop();
      }
      stack.pop();  // discard left parenthesis
    } else if (tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::MULT ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::DIV ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::PLUS ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::MINUS) {
      if (stack.empty() ||
          stack.top().getTokenType().getValue() == Symbol::Terminal::LPAREN) {
        stack.push(tempBuffer[i]);
      } else {
        while (!stack.empty() &&
               stack.top().getTokenType().getValue() !=
                   Symbol::Terminal::LPAREN &&
               prec(tempBuffer[i].getTokenType().getValue()) <=
                   prec(stack.top().getTokenType().getValue())) {
          TokenPair temp = stack.top();
          expr.push_back(temp);
          stack.pop();
        }
        stack.push(tempBuffer[i]);
      }
    }
  }

  while (!stack.empty()) {
    TokenPair temp = stack.top();
    expr.push_back(temp);
    stack.pop();
  }

  return expr;
}

std::string Parser::getSymbolType(TokenPair A) {
  std::string typeA;
  if (A.getTokenType().getValue() == Symbol::Terminal::ID) {
    RecordPtr record = g_SymbolTable[currentLevel]->lookup(Entry::Variables,
                                                           A.getTokenString());
    typeA = record->getType();
  } else {
    if (A.getTokenType().getValue() == Symbol::Terminal::INTLIT) {
      typeA = "int";
    } else if (A.getTokenType().getValue() == Symbol::Terminal::FLOATLIT) {
      typeA = "float";
    }
  }
  return typeA;
}

int Parser::evaPostfix(std::vector<TokenPair>& expr) {
  std::stack<TokenPair> stack;
  for (size_t i = 0; i < expr.size(); ++i) {
    if (expr[i].getTokenType().getValue() == Symbol::Terminal::MULT ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::DIV ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::PLUS ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::MINUS) {
      TokenPair A = stack.top();
      std::string typeA;
      stack.pop();
      TokenPair B = stack.top();
      std::string typeB;
      stack.pop();

      // produce a type for temp var
      std::string temp = new_temp();
      typeA = getSymbolType(A);
      typeB = getSymbolType(B);

      RecordPtr record = std::make_shared<Record>(currentLevel);
      SymbolTablePair idx(Entry::Variables, temp);
      if (typeA == typeB) {
        record->type = typeA;
      } else {
        record->type = "float";
      }
      record->dimension = 0;
      g_SymbolTable[currentLevel]->insert(idx, record);

      // generate IR: op A B temp
      std::cout << terminalMapped[expr[i].getTokenType().getValue()]
                << " " << A.getTokenString()
                << " " << B.getTokenString()
                << " " << temp << std::endl;

      // push temp var into stack
      TokenPair token(Symbol::Terminal::ID, temp);
      stack.push(token);
    } else if (expr[i].getTokenType().getValue() ==
                   Symbol::Terminal::ID ||
               expr[i].getTokenType().getValue() ==
                   Symbol::Terminal::INTLIT ||
               expr[i].getTokenType().getValue() ==
                   Symbol::Terminal::FLOATLIT) {
      stack.push(expr[i]);
    }
  }

  // final elem in stack
  TokenPair res = stack.top();
  stack.pop();
  return res.getTokenType().getValue();
}

void Parser::parseAction(int expr, std::vector<TokenPair>& tempBuffer) {
  for (auto& tokenPair : tempBuffer) {
    std::cout << tokenPair.emit();
  }
  std::cout << std::endl;
  if (expr == Symbol::Action::MakeTypesEnd) {
    SymbolTablePair idx(Entry::Types, tempBuffer[1].getTokenString());
    if (tempBuffer.size() <= 5) {
      g_SymbolTable[currentLevel]->insertTypes(idx,
                                               tempBuffer[3].getTokenString());
    } else {
      g_SymbolTable[currentLevel]->insertTypes(
          idx, tempBuffer[3].getTokenString(), tempBuffer[5].getTokenString(),
          tempBuffer[8].getTokenString());
    }
  } else if (expr == Symbol::Action::MakeVariablesEnd) {
    size_t i = 0;
    for (i = 1; i < tempBuffer.size(); ++i) {
      if (tempBuffer[i].getTokenString() == ":") {
        break;
      }
    }
    for (size_t j = 1; j < i; j += 2) {
      if (tempBuffer.size() <= i + 4) {
        SymbolTablePair idx(Entry::Variables, tempBuffer[j].getTokenString());
        g_SymbolTable[currentLevel]->insertVariables(
            idx, tempBuffer[i + 1].getTokenString());
      } else {
        SymbolTablePair idx(Entry::Variables, tempBuffer[j].getTokenString());
        g_SymbolTable[currentLevel]->insertVariables(
            idx, tempBuffer[i + 1].getTokenString(),
            tempBuffer[i + 3].getTokenString(),
            tempBuffer[i + 6].getTokenString());
      }
    }
  } else if (expr == Symbol::Action::MakeFunctionsEnd) {
    SymbolTablePair idx(Entry::Functions, tempBuffer[1].getTokenString());
    std::vector<std::string> params;
    std::vector<std::string> paramTypes;
    size_t paramBeginIdx, paramEndIdx;
    std::string retType;
    for (size_t i = 0; i < tempBuffer.size(); ++i) {
      if (tempBuffer[i].getTokenString() == "(") {
        paramBeginIdx = i;
      } else if (tempBuffer[i].getTokenString() == ")") {
        paramEndIdx = i;
      }
    }
    if (paramEndIdx + 1 == tempBuffer.size()) {
      retType = "-";
    } else {
      retType = tempBuffer[paramEndIdx + 1].getTokenString();
    }
    for (size_t i = paramBeginIdx + 1; i < paramEndIdx; i += 4) {
      params.push_back(tempBuffer[i].getTokenString());
      paramTypes.push_back(tempBuffer[i + 2].getTokenString());
    }
    g_SymbolTable[currentLevel]->insertFunctions(idx, retType, paramTypes,
                                                 params);
  } else if (expr == Symbol::Action::MakeAssignEnd) {
    if (tempBuffer[1].getTokenString() == "(") { /* function */
      // ....
    } else { /* assignment */
      std::vector<TokenPair> postExpr = cvt2PostExpr(tempBuffer, 2);
        for (auto& tokenPair : postExpr) {
    std::cout << tokenPair.emit();
  }
  std::cout << std::endl;
      evaPostfix(postExpr);
    }
  }
}

bool Parser::detectAction(int symbol, bool& enable_buffer,
                          std::vector<TokenPair>& tempBuffer) {
  // check symbol action: scoping
  if (symbol == Symbol::Action::InitializeScope) {
    initScoping();
    return true;
  }
  if (symbol == Symbol::Action::FinalizeScope) {
    finalizeScoping();
    return true;
  }
  if (symbol == Symbol::Action::MakeTypesBegin ||
      symbol == Symbol::Action::MakeVariablesBegin ||
      symbol == Symbol::Action::MakeFunctionsBegin ||
      symbol == Symbol::Action::MakeExprBegin ||
      symbol == Symbol::Action::MakeAssignBegin) {
    enable_buffer = true;
    return true;
  }
  if (symbol == Symbol::Action::MakeTypesEnd ||
      symbol == Symbol::Action::MakeVariablesEnd ||
      symbol == Symbol::Action::MakeFunctionsEnd ||
      symbol == Symbol::Action::MakeExprEnd ||
      symbol == Symbol::Action::MakeAssignEnd) {
    enable_buffer = false;
    parseAction(symbol, tempBuffer);
    tempBuffer.clear();
    return true;
  }

  return false;
}

void Parser::parse() {
  TokenPair* word = scanner.getToken();
  if (printDebug == true) {
    std::cout << "\n\n[ RUN ] parsing code... \n\n"
              << terminalMapped[word->getTokenType().getValue()] << " ";
  }

  int focus;
  bool enable_buffer = false;
  std::vector<int> null = {Symbol::Terminal::NULLL};
  std::vector<TokenPair> tempBuffer;
  tempBuffer.reserve(100);

  while (true) {
    // get the token and parse
    // complete parsing before going to semantic analysis
    // then embed the semantic actions in the parsing table and
    // accordingly take actions and generate the IR.
    // Output the IR, only if there are no errors in the input.

    // pop grammar rule from stack
    auto& expr = parseStack.top();
    parseStack.pop();

    // detect action and its post process
    if (detectAction(expr, enable_buffer, tempBuffer)) continue;

    focus = word->getTokenType().getValue();

    if (expr == focus) {
      if (enable_buffer == true) {
        tempBuffer.push_back(*word);
      }
      if (focus == Symbol::Terminal::EOFF && parseStack.empty()) {
        std::cout << "\n\n[ OK ] successful parse..." << std::endl;
        break;
      } else {
        word = scanner.getToken();
        if (printDebug == true) {
          std::cout << terminalMapped[word->getTokenType().getValue()] << " ";
        }
      }
    } else {
      auto it = parseTable.find(SymbolTerminalPair(expr, focus));
      if (it != parseTable.end()) {
        auto& rule = it->second;
        if (rule != null) {
          for (auto& word : reverse_iterate(rule)) {
            parseStack.push(word);
          }
        }
      } else {
        numErrors++;
        error(expr, word);
        break;
      } /* it != parseTable.end() */
    }   /* expr != word->getTokenType().getValue() */
  }     /* while */
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
