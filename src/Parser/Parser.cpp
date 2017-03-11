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
  initializeIRMapped();
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
    parseTable_[stp] = expand_rule;
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
                   Symbol::Action::MakeMainLabel,             // NOLINT
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
                   Symbol::Action::MakeFunctionsMid,       // NOLINT
                   Symbol::Terminal::BEGIN,                // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,          // NOLINT
                   Symbol::Action::MakeFunctionsEnd,       // NOLINT
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
  addToParseTable(Symbol::Nonterminal::STAT,           // NOLINT
                  {Symbol::Terminal::IF},              // NOLINT
                  {Symbol::Terminal::IF,               // NOLINT
                   Symbol::Action::MakeIfBegin,        // NOLINT
                   Symbol::Nonterminal::EXPR,          // NOLINT
                   Symbol::Action::MakeIfMid,          // NOLINT
                   Symbol::Terminal::THEN,             // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,      // NOLINT
                   Symbol::Nonterminal::STAT_IF_TAIL,  // NOLINT
                   Symbol::Action::MakeIfEnd});        // NOLINT

  // 33: <stat-if-tail> -> else <stat-seq> endif;
  addToParseTable(Symbol::Nonterminal::STAT_IF_TAIL,  // NOLINT
                  {Symbol::Terminal::ELSE},           // NOLINT
                  {Symbol::Terminal::ELSE,            // NOLINT
                   Symbol::Action::MakeElseLabel,     // NOLINT
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
                   Symbol::Action::BeforeAssignLabel,            // NOLINT
                   Symbol::Terminal::ID,                         // NOLINT
                   Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN});  // NOLINT

  // 36: <stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
  addToParseTable(Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN,  // NOLINT
                  {Symbol::Terminal::LBRACK,                  // NOLINT
                   Symbol::Terminal::ASSIGN},                 // NOLINT
                  {Symbol::Nonterminal::LVALUE_TAIL,          // NOLINT
                   Symbol::Action::BeforeAssignLabel,         // NOLINT
                   Symbol::Terminal::ASSIGN,                  // NOLINT
                   Symbol::Nonterminal::STAT_ASSIGN,          // NOLINT
                   Symbol::Action::MakeAssignEnd,             // NOLINT
                   Symbol::Terminal::SEMI});                  // NOLINT

  // 37: <stat-funct-or-assign> -> (<expr-list>);
  addToParseTable(Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN,  // NOLINT
                  {Symbol::Terminal::LPAREN},                 // NOLINT
                  {Symbol::Action::EndAssignLabel,            // NOLINT
                   Symbol::Terminal::LPAREN,                  // NOLINT
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
  addToParseTable(Symbol::Nonterminal::STAT,        // NOLINT
                  {Symbol::Terminal::WHILE},        // NOLINT
                  {Symbol::Terminal::WHILE,         // NOLINT
                   Symbol::Action::MakeWhileBegin,  // NOLINT
                   Symbol::Nonterminal::EXPR,       // NOLINT
                   Symbol::Action::MakeWhileMid,    // NOLINT
                   Symbol::Terminal::DO,            // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,   // NOLINT
                   Symbol::Terminal::ENDDO,         // NOLINT
                   Symbol::Action::MakeWhileEnd,    // NOLINT
                   Symbol::Terminal::SEMI});        // NOLINT

  // 49: <stat> -> for id := <expr> to <expr> do <stat-seq> enddo;
  addToParseTable(Symbol::Nonterminal::STAT,       // NOLINT
                  {Symbol::Terminal::FOR},         // NOLINT
                  {Symbol::Action::MakeForBegin,   // NOLINT
                   Symbol::Terminal::FOR,          // NOLINT
                   Symbol::Terminal::ID,           // NOLINT
                   Symbol::Terminal::ASSIGN,       // NOLINT
                   Symbol::Action::EmdExprBegin,   // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Action::EmdExprEnd,     // NOLINT
                   Symbol::Terminal::TO,           // NOLINT
                   Symbol::Action::EmdExprBegin,   // NOLINT
                   Symbol::Nonterminal::EXPR,      // NOLINT
                   Symbol::Action::EmdExprEnd,     // NOLINT
                   Symbol::Action::MakeForMid,     // NOLINT
                   Symbol::Terminal::DO,           // NOLINT
                   Symbol::Nonterminal::STAT_SEQ,  // NOLINT
                   Symbol::Terminal::ENDDO,        // NOLINT
                   Symbol::Terminal::SEMI,         // NOLINT
                   Symbol::Action::MakeForEnd});   // NOLINT

  // 50: <stat> -> break;
  addToParseTable(Symbol::Nonterminal::STAT,   // NOLINT
                  {Symbol::Terminal::BREAK},   // NOLINT
                  {Symbol::Terminal::BREAK,    // NOLINT
                   Symbol::Action::MakeBreak,  // NOLINT
                   Symbol::Terminal::SEMI});   // NOLINT

  // 51: <stat> -> return <expr>;
  addToParseTable(Symbol::Nonterminal::STAT,         // NOLINT
                  {Symbol::Terminal::RETURN},        // NOLINT
                  {Symbol::Terminal::RETURN,         // NOLINT
                   Symbol::Action::MakeReturnBegin,  // NOLINT
                   Symbol::Nonterminal::EXPR,        // NOLINT
                   Symbol::Action::MakeReturnEnd,    // NOLINT
                   Symbol::Terminal::SEMI});         // NOLINT

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
  addToParseTable(Symbol::Nonterminal::FACTOR,  // NOLINT
                  {Symbol::Terminal::LPAREN},   // NOLINT
                  {Symbol::Terminal::LPAREN,    // NOLINT
                   Symbol::Nonterminal::EXPR,   // NOLINT
                   Symbol::Terminal::RPAREN});  // NOLINT

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
                  {Symbol::Nonterminal::EXPR,              // NOLINT
                   Symbol::Nonterminal::EXPR_LIST_TAIL});  // NOLINT

  // 87: <expr-list-tail> -> , <expr> <expr-list-tail>
  addToParseTable(Symbol::Nonterminal::EXPR_LIST_TAIL,     // NOLINT
                  {Symbol::Terminal::COMMA},               // NOLINT
                  {Symbol::Terminal::COMMA,                // NOLINT
                   Symbol::Nonterminal::EXPR,              // NOLINT
                   Symbol::Nonterminal::EXPR_LIST_TAIL});  // NOLINT

  // 88: <expr-list-tail> -> NULL
  addToParseTable(Symbol::Nonterminal::EXPR_LIST_TAIL,  // NOLINT
                  {Symbol::Terminal::RPAREN},           // NOLINT
                  {Symbol::Terminal::NULLL});           // NOLINT

  // 89: <lvalue> -> id <lvalue-tail>
  addToParseTable(Symbol::Nonterminal::LVALUE,          // NOLINT
                  {Symbol::Terminal::ID},               // NOLINT
                  {Symbol::Terminal::ID,                // NOLINT
                   Symbol::Nonterminal::LVALUE_TAIL});  // NOLINT

  // 90: <lvalue-tail> -> [<expr>]
  addToParseTable(Symbol::Nonterminal::LVALUE_TAIL,  // NOLINT
                  {Symbol::Terminal::LBRACK},        // NOLINT
                  {Symbol::Action::MakeArrayBegin,   // NOLINT
                   Symbol::Terminal::LBRACK,         // NOLINT
                   Symbol::Nonterminal::EXPR,        // NOLINT
                   Symbol::Terminal::RBRACK,         // NOLINT
                   Symbol::Action::MakeArrayEnd});   // NOLINT

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
  auto prec = [&](int op) -> int {
    switch (op) {
      case Symbol::Terminal::LESSER:
      case Symbol::Terminal::LESSEREQ:
      case Symbol::Terminal::GREATER:
      case Symbol::Terminal::GREATEREQ:
      case Symbol::Terminal::EQ:
      case Symbol::Terminal::NEQ:
        return 0;
      case Symbol::Terminal::PLUS:
      case Symbol::Terminal::MINUS:
        return 1;
      case Symbol::Terminal::MULT:
      case Symbol::Terminal::DIV:
        return 2;
      case Symbol::Terminal::AND:
      case Symbol::Terminal::OR:
        return 3;
      default:
        return -1;
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
                   Symbol::Terminal::MINUS ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::AND ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::OR ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::LESSER ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::LESSEREQ ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::GREATER ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::GREATEREQ ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::EQ ||
               tempBuffer[i].getTokenType().getValue() ==
                   Symbol::Terminal::NEQ) {
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

TokenPair Parser::evaPostfix(std::vector<TokenPair>& expr) {
  std::stack<TokenPair> stack;
  for (size_t i = 0; i < expr.size(); ++i) {
    if (expr[i].getTokenType().getValue() == Symbol::Terminal::MULT ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::DIV ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::PLUS ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::MINUS ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::AND ||
        expr[i].getTokenType().getValue() == Symbol::Terminal::OR) {
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
      std::string code =
          "    " + OperatorMapped[expr[i].getTokenType().getValue()] + ", " +
          A.getTokenString() + ", " + B.getTokenString() + ", " + temp;
      IR.push_back(code);

      // push temp var into stack
      TokenPair token(Symbol::Terminal::ID, temp);
      stack.push(token);
    } else if (expr[i].getTokenType().getValue() == Symbol::Terminal::ID ||
               expr[i].getTokenType().getValue() == Symbol::Terminal::INTLIT ||
               expr[i].getTokenType().getValue() ==
                   Symbol::Terminal::FLOATLIT) {
      stack.push(expr[i]);
    } else if (expr[i].getTokenType().getValue() == Symbol::Terminal::LESSER ||
               expr[i].getTokenType().getValue() ==
                   Symbol::Terminal::LESSEREQ ||
               expr[i].getTokenType().getValue() == Symbol::Terminal::GREATER ||
               expr[i].getTokenType().getValue() ==
                   Symbol::Terminal::GREATEREQ ||
               expr[i].getTokenType().getValue() == Symbol::Terminal::EQ ||
               expr[i].getTokenType().getValue() == Symbol::Terminal::NEQ) {
      if (i != expr.size() - 1) {
        std::cerr << "\nError: if boolean operation exists in if or while "
                     "condition statement, "
                  << " it must be the last operation in this expression!"
                  << " for example, if (a + b >= c * d) is correct."
                  << std::endl;
        std::exit(EXIT_FAILURE);
      } else {
        TokenPair A = stack.top();
        std::string typeA;
        stack.pop();
        TokenPair B = stack.top();
        std::string typeB;
        stack.pop();
        //     assign, $t0, 0,
        //     brneq, 0a, 0b, if_label0
        //     assign, $t0, 1,
        // if_label0:
        //     breq, $t0, 0, if_label1
        // generate cmp IR code
        std::string temp = new_temp();
        RecordPtr record = std::make_shared<Record>(currentLevel);
        SymbolTablePair idx(Entry::Variables, temp);
        record->type = "int";
        record->dimension = 0;
        g_SymbolTable[currentLevel]->insert(idx, record);

        std::string code = "    assign, " + temp + ", 0,";
        IR.push_back(code);

        auto ifLabel = labelStack_.top();

        code = "    " + OperatorMapped[expr[i].getTokenType().getValue()] +
               ", " + A.getTokenString() + ", " + B.getTokenString() + ", " +
               ifLabel.first;
        IR.push_back(code);

        code = "    assign, " + temp + ", 1,";
        IR.push_back(code);

        code = ifLabel.first + ":";
        IR.push_back(code);

        code = "    breq, " + temp + ", 0, " + ifLabel.second;
        IR.push_back(code);
        return TokenPair(Symbol::Terminal::VAR, "unkown");
      }
    }
  }

  // final elem in stack
  TokenPair res = stack.top();
  stack.pop();

  return res;
}

void Parser::parseAction(int expr, std::vector<TokenPair>& tempBuffer) {
  // for (auto& tokenPair : tempBuffer) {
  //   std::cout << tokenPair.emit();
  // }
  // std::cout << std::endl;
  if (expr == Symbol::Action::MakeExprEnd ||
      expr == Symbol::Action::EmdExprEnd) {
    // expression
    std::vector<TokenPair> postExpr = cvt2PostExpr(tempBuffer, 0);
    auto res = evaPostfix(postExpr);
    tempStack_.push(res);
  }
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
      if (tempBuffer.size() <= i + 5) { /* var a : id := 10; */
        SymbolTablePair idx(Entry::Variables, tempBuffer[j].getTokenString());

        if (g_SymbolTable[currentLevel]->find(Entry::Variables,
                                              tempBuffer[j].getTokenString())) {
          std::cout << "\n\nError: Redeclaration of the same name in the same "
                       "scope is illegal.\n"
                    << std::endl;
          exit(EXIT_FAILURE);
        }
        // insert var into symbol table
        g_SymbolTable[currentLevel]->insertVariables(
            idx, tempBuffer[i + 1].getTokenString());

        // generate IR code
        auto record = g_SymbolTable[currentLevel]->lookup(
            Entry::Types, tempBuffer[i + 1].getTokenString());
        auto type = record->getType();
        int dims = record->getDimension();

        // get the init value
        std::string code =
            "    assign, " + tempBuffer[j].getTokenString() + ", ";
        std::string value = tempBuffer[tempBuffer.size() - 2].getTokenString();
        if (tempBuffer.size() <= i + 3) {
          value = (type == "int") ? "0" : "0.0";
        }
        if (dims > 0) {
          code += std::to_string(dims) + ", " + value;
        } else {
          code += value + ",";
        }
        IR.push_back(code);
      } else { /* var a : array[100] of id := 10; */
        SymbolTablePair idx(Entry::Variables, tempBuffer[j].getTokenString());
        g_SymbolTable[currentLevel]->insertVariables(
            idx, tempBuffer[i + 1].getTokenString(),
            tempBuffer[i + 3].getTokenString(),
            tempBuffer[i + 6].getTokenString());
        // generate IR code
        std::string value = tempBuffer[tempBuffer.size() - 2].getTokenString();
        if (value == "int") {
          value = "0";
        } else if (value == "float") {
          value = "0.0";
        }
        std::string code = "    assign, " + tempBuffer[j].getTokenString() +
                           ", " + tempBuffer[i + 3].getTokenString() + ", " +
                           value;
        IR.push_back(code);
      }
    }
  } else if (expr == Symbol::Action::MakeAssignEnd) {
    if (tempBuffer[1].getTokenString() == "(") { /* function */
      RecordPtr record = g_SymbolTable[currentLevel]->lookup(
          Entry::Functions, tempBuffer[0].getTokenString());
      auto& dims = record->getParameterDimensions();
      auto& paramTypes = record->getParameterTypes();

      // semantic checking: param size
      // IR: code
      std::string code = "    call, " + tempBuffer[0].getTokenString();
      size_t j = 0;
      for (size_t i = 2; i < tempBuffer.size() - 1; i += 2, ++j) {
        code += ", " + tempBuffer[i].getTokenString();
        auto type = getSymbolType(tempBuffer[i]);
        if (j >= paramTypes.size()) {
          std::cerr << "\nError: function " << tempBuffer[0].getTokenString()
                    << " has too many parameters! \n"
                    << std::endl;
          std::exit(EXIT_FAILURE);
        }

        if (type != paramTypes[j]) {
          std::cerr << "\n Error: " << tempBuffer[i].getTokenString() << ": "
                    << type << " mismatched to function "
                    << tempBuffer[0].getTokenString()
                    << " parameter: " << paramTypes[j] << std::endl;
          std::exit(EXIT_FAILURE);
        }
      }
      if (j != paramTypes.size()) {
        std::cerr << "\nError: function " << tempBuffer[0].getTokenString()
                  << " parameter numbers is not matched! \n"
                  << std::endl;
        std::exit(EXIT_FAILURE);
      }

      // save IR code
      IR.push_back(code);
    } else { /* assignment */
      if (tempBuffer[tempBuffer.size() - 1].getTokenString() == ")" &&
          tempBuffer[3].getTokenString() == "(") {
        // assignment function
        RecordPtr record = g_SymbolTable[currentLevel]->lookup(
            Entry::Functions, tempBuffer[2].getTokenString());
        auto& dims = record->getParameterDimensions();
        auto& paramTypes = record->getParameterTypes();

        // semantic checking: param size
        // IR: code
        std::string code = "    callr, " + tempBuffer[0].getTokenString() +
                           ", " + tempBuffer[2].getTokenString();
        size_t j = 0;
        for (size_t i = 4; i < tempBuffer.size() - 1; i += 2, ++j) {
          code += ", " + tempBuffer[i].getTokenString();
          auto type = getSymbolType(tempBuffer[i]);
          if (j >= paramTypes.size()) {
            std::cerr << "\nError: function " << tempBuffer[2].getTokenString()
                      << " has too many parameters! \n"
                      << std::endl;
            std::exit(EXIT_FAILURE);
          }

          if (type != paramTypes[j]) {
            std::cerr << "\n Error: " << tempBuffer[i].getTokenString() << ": "
                      << type << " mismatched to function "
                      << tempBuffer[2].getTokenString()
                      << " parameter: " << paramTypes[j] << std::endl;
            std::exit(EXIT_FAILURE);
          }
        }

        if (j != paramTypes.size()) {
          std::cerr << "\nError: function " << tempBuffer[2].getTokenString()
                    << " parameter numbers is not matched! \n"
                    << std::endl;
          std::exit(EXIT_FAILURE);
        }

        if (record->getReturnType() == "-") {
          std::cerr << "\nError: function " << tempBuffer[2].getTokenString()
                    << " does not have an return value! \n"
                    << std::endl;
          std::exit(EXIT_FAILURE);
        }
        std::string type = getSymbolType(tempBuffer[0]);
        if (record->getReturnType() != type) {
          std::cerr << "\nError: function " << tempBuffer[2].getTokenString()
                    << " return type is different to var: "
                    << tempBuffer[0].getTokenString() << " !\n"
                    << std::endl;
          std::exit(EXIT_FAILURE);
        }
        // save IR code
        IR.push_back(code);
      } else {
        // assignment expression
        std::vector<TokenPair> postExpr = cvt2PostExpr(tempBuffer, 2);
        auto res = evaPostfix(postExpr);
        if (is_arr_ == 1 && !array_store_.empty()) {
          IR.push_back(array_store_ + res.getTokenString());
          array_store_.clear();
        } else {
          if (getSymbolType(tempBuffer[0]) != getSymbolType(res)) {
            std::cout << "Error: left and right type between assignment is "
                         "mismatched!\n"
                      << std::endl;
          }

          std::string code = "    assgin, " + tempBuffer[0].getTokenString() +
                             ", " + res.getTokenString() + ",";
          IR.push_back(code);
        }
      }
    }
  }
}

void Parser::parseForActionEnd(std::vector<TokenPair>& blockBuffer) {
  // generate label's IR code
  // add, 0i, 1, 0i
  // goto, loop_label0, ,
  // loop_label1
  std::string code = "    add, " + blockBuffer[1].getTokenString() + ", " +
                     "1, " + blockBuffer[1].getTokenString();
  IR.push_back(code);
  auto labelPair = labelStack_.top();
  code = "    goto, " + labelPair.first + ", ,";
  IR.push_back(code);
  IR.push_back(labelPair.second + ":");
}

void Parser::parseForAction(std::vector<TokenPair>& blockBuffer) {
  // semantic checking for types
  auto type1 = getSymbolType(blockBuffer[3]);
  auto type2 = getSymbolType(blockBuffer[5]);
  if (type1 != "int" || type2 != "int") {
    std::cerr << "\nError: for statement begin or end value is not int type!\n"
              << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // generate IR code
  std::string code = "    assign, " + blockBuffer[1].getTokenString() + ", " +
                     blockBuffer[3].getTokenString() + ",";
  IR.push_back(code);

  auto loopLabel = labelStack_.top();

  IR.push_back(loopLabel.first + ":");

  code = "    brgt, " + blockBuffer[1].getTokenString() + ", " +
         blockBuffer[5].getTokenString() + ", " + loopLabel.second;
  IR.push_back(code);
}

void Parser::parseFuncAction(std::vector<TokenPair>& tempBuffer) {
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
    retType =
        g_SymbolTable[currentLevel]
            ->lookup(Entry::Types, tempBuffer[paramEndIdx + 2].getTokenString())
            ->getType();
  }
  funcRetType_ = retType;

  for (size_t i = paramBeginIdx + 1; i < paramEndIdx; i += 4) {
    params.push_back(tempBuffer[i].getTokenString());
    paramTypes.push_back(tempBuffer[i + 2].getTokenString());
  }
  g_SymbolTable[currentLevel]->insertFunctions(idx, retType, paramTypes,
                                               params);

  // insert var into function symbol table
  for (size_t i = 0; i < params.size(); ++i) {
    std::string varName = params[i];
    std::string varType = g_SymbolTable[currentLevel]
                              ->lookup(Entry::Types, paramTypes[i])
                              ->getType();
    int varDim = g_SymbolTable[currentLevel]
                     ->lookup(Entry::Types, paramTypes[i])
                     ->getDimension();

    // insert function parameters as function symbol table elems
    // define record
    SymbolTablePair idx(Entry::Variables, varName);
    RecordPtr record = std::make_shared<Record>(currentLevel + 1);
    // set record attributes
    record->type = varType;
    record->dimension = varDim;
    // insert into symbol table
    g_SymbolTable[currentLevel + 1]->insert(idx, record);
  }
  // function print ( n : int, me : ArrayInt)
  std::string code = tempBuffer[1].getTokenString() + ":";
  IR.push_back(code);
}

void Parser::parseIfAction(std::vector<TokenPair>& tempBuffer) {
  std::vector<TokenPair> postExpr = cvt2PostExpr(tempBuffer, 0);
  auto res = evaPostfix(postExpr);
  if (res.getTokenString() != "unkown") {
    std::string temp = new_temp();
    RecordPtr record = std::make_shared<Record>(currentLevel);
    SymbolTablePair idx(Entry::Variables, temp);
    record->type = "int";
    record->dimension = 0;
    g_SymbolTable[currentLevel]->insert(idx, record);

    std::string code = "    assign, " + temp + ", 0,";
    IR.push_back(code);

    auto ifLabel = labelStack_.top();
    code = "    brneq, " + res.getTokenString() + ", 0, " + ifLabel.first;
    IR.push_back(code);

    code = "    assign, " + temp + ", 1,";
    IR.push_back(code);

    code = ifLabel.first + ":";
    IR.push_back(code);

    code = "    breq, " + temp + ", 0, " + ifLabel.second;
    IR.push_back(code);
  }
}

void Parser::parseReturnAction(std::vector<TokenPair>& tempBuffer) {
  std::vector<TokenPair> postExpr = cvt2PostExpr(tempBuffer, 0);
  auto res = evaPostfix(postExpr);

  // semantic checking return type
  if (isInside_func_) {
    if (funcRetType_ == "-") {
      std::cerr << "\nError: this function is no return !\n" << std::endl;
      std::exit(EXIT_FAILURE);
    } else if (getSymbolType(res) != funcRetType_) {
      std::cerr << "\nError: return type in function should be " << funcRetType_
                << "!\n"
                << std::endl;
      std::exit(EXIT_FAILURE);
    }
  } else {
    if (getSymbolType(res) != "int") {
      std::cerr << "\nError: main function return type must be an integer !\n"
                << std::endl;
      std::exit(EXIT_FAILURE);
    }
    isMainRet_ = true;
  }
  // generate code
  std::string code = "    return, " + res.getTokenString() + ", ,";
  IR.push_back(code);
  isFuncRet_ = true;
}

void Parser::parseWhileAction(std::vector<TokenPair>& tempBuffer) {
  std::vector<TokenPair> postExpr = cvt2PostExpr(tempBuffer, 0);
  auto res = evaPostfix(postExpr);
  if (res.getTokenString() != "unkown") {
    std::string temp = new_temp();
    RecordPtr record = std::make_shared<Record>(currentLevel);
    SymbolTablePair idx(Entry::Variables, temp);
    record->type = "int";
    record->dimension = 0;
    g_SymbolTable[currentLevel]->insert(idx, record);

    std::string code = "    assign, " + temp + ", 0,";
    IR.push_back(code);

    auto ifLabel = labelStack_.top();
    code = "    brneq, " + res.getTokenString() + ", 0, " + ifLabel.first;
    IR.push_back(code);

    code = "    assign, " + temp + ", 1,";
    IR.push_back(code);

    code = ifLabel.first + ":";
    IR.push_back(code);

    code = "    breq, " + temp + ", 0, " + ifLabel.second;
    IR.push_back(code);
  }
}

std::vector<TokenPair> Parser::subTokenPairs(
    const std::vector<TokenPair>& buffer, size_t actBegin, size_t actEnd) {
  std::vector<TokenPair> arr;
  arr.reserve(actEnd - actBegin);
  for (size_t i = actBegin; i < actEnd; ++i) {
    arr.push_back(buffer[i]);
  }
  return arr;
}

bool Parser::detectAction(int symbol, bool& enable_block,
                          std::vector<TokenPair>& blockBuffer,
                          bool& enable_buffer,
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
  if (symbol == Symbol::Action::MakeMainLabel) {
    std::string code = "main:";
    IR.push_back(code);
    return true;
  }
  if (symbol == Symbol::Action::MakeBreak) {
    if (isInside_for_) {
      IR.push_back("    goto, " + blockStack_.top().second + ", ,");
    } else if (isInside_while_) {
      IR.push_back("    goto, " + blockStack_.top().first + ", ,");
    } else {
      std::cout << "\nError: break is not inside a for or while block!\n"
                << std::endl;
    }
    return true;
  }
  if (symbol == Symbol::Action::MakeWhileBegin) {
    enable_buffer = true;
    isInside_while_ = true;
    auto labels = std::make_pair<std::string, std::string>(new_loop_label(),
                                                           new_loop_label());
    currLoopLabel_ = labels;
    IR.push_back(labels.first + ":");

    labels.first = labels.second;
    labels.second = new_loop_label();
    labelStack_.push(labels);
    blockStack_.push(labels);
    return true;
  }
  if (symbol == Symbol::Action::MakeWhileMid) {
    enable_buffer = false;
    parseWhileAction(tempBuffer);
    tempBuffer.clear();
    return true;
  }
  if (symbol == Symbol::Action::MakeWhileEnd) {
    isInside_while_ = false;
    auto labels = labelStack_.top();
    IR.push_back("    goto, " + currLoopLabel_.first + ", ,");
    IR.push_back(labels.second + ":");
    labelStack_.pop();
    blockStack_.pop();
    return true;
  }
  if (symbol == Symbol::Action::MakeReturnBegin) {
    enable_buffer = true;
    return true;
  }
  if (symbol == Symbol::Action::MakeReturnEnd) {
    parseReturnAction(tempBuffer);
    tempBuffer.clear();
    enable_buffer = false;
    return true;
  }
  if (symbol == Symbol::Action::MakeIfBegin) {
    enable_buffer = true;
    auto ifLabel = std::make_pair<std::string, std::string>(new_if_label(),
                                                            new_if_label());
    labelStack_.push(ifLabel);
    return true;
  }
  if (symbol == Symbol::Action::MakeIfMid) {
    parseIfAction(tempBuffer);
    tempBuffer.clear();
    enable_buffer = false;
    return true;
  }
  if (symbol == Symbol::Action::MakeElseLabel) {
    // goto, if_label2, ,
    // if_label1:
    std::string new_label = new_if_label();
    std::string code = "    goto, " + new_label + ", ,";
    IR.push_back(code);

    auto lblStack = labelStack_.top();
    labelStack_.pop();
    code = lblStack.second + ":";
    IR.push_back(code);

    lblStack.second = new_label;
    labelStack_.push(lblStack);
    return true;
  }
  if (symbol == Symbol::Action::MakeIfEnd) {
    auto lblStack = labelStack_.top();
    labelStack_.pop();
    IR.push_back(lblStack.second + ":");
    return true;
  }
  if (symbol == Symbol::Action::MakeFunctionsBegin) {
    enable_buffer = true;
    currentLevel++;
    SymbolTablePtr st = std::make_shared<SymbolTable>(currentLevel);
    g_SymbolTable[currentLevel] = st;
    currentLevel--;
    return true;
  }
  if (symbol == Symbol::Action::MakeFunctionsMid) {
    // init sepcial symbol table for function
    enable_buffer = false;
    isInside_func_ = true;
    parseFuncAction(tempBuffer);
    currentLevel++;
    tempBuffer.clear();
    return true;
  }
  if (symbol == Symbol::Action::MakeFunctionsEnd) {
    if (isFuncRet_ == false) {
      // no return type
      IR.push_back("    return, , ,");
    }
    isFuncRet_ = false;
    isInside_func_ = false;
    currentLevel--;
    return true;
  }
  if (symbol == Symbol::Action::EmdExprBegin) {
    enable_block = false;
    enable_buffer = true;
    return true;
  }
  if (symbol == Symbol::Action::EmdExprEnd) {
    enable_block = true;
    enable_buffer = false;
    parseAction(symbol, tempBuffer);
    blockBuffer.push_back(tempStack_.top());
    tempStack_.pop();
    tempBuffer.clear();
    return true;
  }
  if (symbol == Symbol::Action::MakeForBegin) {
    isInside_for_ = true;
    enable_block = true;
    auto loopLabel = std::make_pair<std::string, std::string>(new_loop_label(),
                                                              new_loop_label());
    labelStack_.push(loopLabel);
    blockStack_.push(loopLabel);
    return true;
  }
  if (symbol == Symbol::Action::MakeForMid) {
    // parsing for x := t0 to t1
    // assign, 0i, 0,
    // loop_label0:
    // brgt, 0i, 100, loop_label1
    enable_block = false;
    parseForAction(blockBuffer);
    return true;
  }
  if (symbol == Symbol::Action::MakeForEnd) {
    // generate label's IR code
    // add, 0i, 1, 0i
    // goto, loop_label0, ,
    // loop_label1:
    isInside_for_ = false;
    parseForActionEnd(blockBuffer);
    labelStack_.pop();
    blockStack_.pop();
    blockBuffer.clear();
    return true;
  }
  if (symbol == Symbol::Action::MakeArrayBegin) {  // [expr]
    actBegin_ = tempBuffer.size() + 1;
    return true;
  }
  if (symbol == Symbol::Action::MakeArrayEnd) {  // [expr]
    actEnd_ = tempBuffer.size() - 1;
    std::vector<TokenPair> expr = subTokenPairs(tempBuffer, actBegin_, actEnd_);

    std::vector<TokenPair> postExpr = cvt2PostExpr(expr, 0);
    auto res = evaPostfix(postExpr);

    if (is_arr_ == 0) {
      array_store_ = "    array_store, " +
                     tempBuffer[actBegin_ - 2].getTokenString() + ", " +
                     res.getTokenString() + ", ";
    } else {
      // generate temp var
      std::string temp = new_temp();
      // define record
      SymbolTablePair idx(Entry::Variables, temp);
      RecordPtr record = std::make_shared<Record>(currentLevel);
      // set record attributes
      record->type = getSymbolType(tempBuffer[actBegin_ - 2]);
      record->dimension = 0;
      // insert into symbol table
      g_SymbolTable[currentLevel]->insert(idx, record);

      // generate IR code
      std::string code = "    array_load, " + temp + ", " +
                         tempBuffer[actBegin_ - 2].getTokenString() + ", " +
                         res.getTokenString();
      IR.push_back(code);

      // push it into temp buffer
      tempBuffer.erase(tempBuffer.begin() + actBegin_ - 2,
                       tempBuffer.begin() + actEnd_ + 1);

      TokenPair tempPair(Symbol::Terminal::ID, temp);
      tempBuffer.push_back(tempPair);
    }
    return true;
  }
  if (symbol == Symbol::Action::MakeTypesBegin ||
      symbol == Symbol::Action::MakeVariablesBegin ||
      symbol == Symbol::Action::MakeExprBegin ||
      symbol == Symbol::Action::MakeAssignBegin) {
    enable_buffer = true;
    return true;
  }
  if (symbol == Symbol::Action::MakeTypesEnd ||
      symbol == Symbol::Action::MakeVariablesEnd ||
      symbol == Symbol::Action::MakeExprEnd ||
      symbol == Symbol::Action::MakeAssignEnd) {
    enable_buffer = false;
    parseAction(symbol, tempBuffer);
    tempBuffer.clear();
    is_arr_ = -1;
    return true;
  }
  if (symbol == Symbol::Action::BeforeAssignLabel) {
    is_arr_++;
    return true;
  }
  if (symbol == Symbol::Action::EndAssignLabel) {
    is_arr_ = -1;
    return true;
  }
  return false;
}

void Parser::parse() {
  TokenPair* word = scanner.getToken();
  std::cout << "\n\n[ RUN ] parsing code... \n\n";
  if (printDebug == true) {
    std::cout << terminalMapped_[word->getTokenType().getValue()] << " ";
  }

  int focus;

  // NULL symbol
  std::vector<int> null = {Symbol::Terminal::NULLL};

  // temp buffer to expr, type, var declaration
  bool enable_buffer = false;
  std::vector<TokenPair> tempBuffer;

  // temp buffer to for statement
  bool enable_block = false;
  std::vector<TokenPair> blockBuffer;

  tempBuffer.reserve(100);
  blockBuffer.reserve(300);
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
    if (detectAction(expr, enable_block, blockBuffer, enable_buffer,
                     tempBuffer))
      continue;

    focus = word->getTokenType().getValue();

    if (expr == focus) {
      if (enable_block == true) {
        blockBuffer.push_back(*word);
      } else if (enable_buffer == true) {
        tempBuffer.push_back(*word);
      }
      if (focus == Symbol::Terminal::EOFF && parseStack.empty()) {
        if (isMainRet_ == false) {
          IR.push_back("    return, , ,");
        }
        std::cout << "\n\n[ OK ] successful parse..." << std::endl;
        break;
      } else {
        word = scanner.getToken();
        if (printDebug == true) {
          std::cout << terminalMapped_[word->getTokenType().getValue()] << " ";
        }
      }
    } else {
      auto it = parseTable_.find(SymbolTerminalPair(expr, focus));
      if (it != parseTable_.end()) {
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
      } /* it != parseTable_.end() */
    }   /* expr != word->getTokenType().getValue() */
  }     /* while */
}

void Parser::ir_code() {
  std::cout << "\n\n----------------------------------------" << std::endl;
  std::cout << "Generate IR CODE ..." << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  for (size_t i = 0; i < IR.size(); ++i) {
    std::cout << IR[i] << std::endl;
  }
  std::cout << "----------------------------------------\n" << std::endl;
}

void Parser::initializeIRMapped() {
  // op: (op, y, z, x)
  OperatorMapped[Symbol::Terminal::MULT] = "mult";
  OperatorMapped[Symbol::Terminal::DIV] = "div";
  OperatorMapped[Symbol::Terminal::PLUS] = "add";
  OperatorMapped[Symbol::Terminal::MINUS] = "sub";
  OperatorMapped[Symbol::Terminal::AND] = "and";
  OperatorMapped[Symbol::Terminal::OR] = "or";
  // assign: (op, x, y, _)
  OperatorMapped[Symbol::Terminal::ASSIGN] = "assign";
  // goto: (op, label, _, _)
  OperatorMapped[Symbol::Terminal::BREAK] = "goto";
  // branch (op, y, z, label)
  OperatorMapped[Symbol::Terminal::NEQ] = "breq";
  OperatorMapped[Symbol::Terminal::EQ] = "brneq";
  OperatorMapped[Symbol::Terminal::GREATEREQ] = "brlt";
  OperatorMapped[Symbol::Terminal::LESSEREQ] = "brgt";
  OperatorMapped[Symbol::Terminal::LESSER] = "brgeq";
  OperatorMapped[Symbol::Terminal::GREATER] = "brleq";
  // return: (op, x, _, _)
  OperatorMapped[Symbol::Terminal::RETURN] = "return";
}

void Parser::initializeTerminalMapped() {
  terminalMapped_[Symbol::Terminal::ARRAY] = "array";
  terminalMapped_[Symbol::Terminal::BREAK] = "break";
  terminalMapped_[Symbol::Terminal::DO] = "do";
  terminalMapped_[Symbol::Terminal::ELSE] = "else";
  terminalMapped_[Symbol::Terminal::END] = "end";
  terminalMapped_[Symbol::Terminal::FOR] = "for";
  terminalMapped_[Symbol::Terminal::FUNCTION] = "function";
  terminalMapped_[Symbol::Terminal::IF] = "if";
  terminalMapped_[Symbol::Terminal::IN] = "in";
  terminalMapped_[Symbol::Terminal::LET] = "let";
  terminalMapped_[Symbol::Terminal::OF] = "of";
  terminalMapped_[Symbol::Terminal::THEN] = "then";
  terminalMapped_[Symbol::Terminal::TO] = "to";
  terminalMapped_[Symbol::Terminal::TYPE] = "type";
  terminalMapped_[Symbol::Terminal::VAR] = "var";
  terminalMapped_[Symbol::Terminal::WHILE] = "while";
  terminalMapped_[Symbol::Terminal::ENDIF] = "endif";
  terminalMapped_[Symbol::Terminal::BEGIN] = "begin";
  terminalMapped_[Symbol::Terminal::ENDDO] = "enddo";
  terminalMapped_[Symbol::Terminal::RETURN] = "return";
  terminalMapped_[Symbol::Terminal::INT] = "int";
  terminalMapped_[Symbol::Terminal::FLOAT] = "float";
  terminalMapped_[Symbol::Terminal::COMMA] = ",";
  terminalMapped_[Symbol::Terminal::COLON] = ":";
  terminalMapped_[Symbol::Terminal::SEMI] = ";";
  terminalMapped_[Symbol::Terminal::LPAREN] = "(";
  terminalMapped_[Symbol::Terminal::RPAREN] = ")";
  terminalMapped_[Symbol::Terminal::LBRACK] = "[";
  terminalMapped_[Symbol::Terminal::RBRACK] = "]";
  terminalMapped_[Symbol::Terminal::LBRACE] = "{";
  terminalMapped_[Symbol::Terminal::RBRACE] = "}";
  terminalMapped_[Symbol::Terminal::PERIOD] = ".";
  terminalMapped_[Symbol::Terminal::PLUS] = "+";
  terminalMapped_[Symbol::Terminal::MINUS] = "-";
  terminalMapped_[Symbol::Terminal::MULT] = "*";
  terminalMapped_[Symbol::Terminal::DIV] = "/";
  terminalMapped_[Symbol::Terminal::EQ] = "=";
  terminalMapped_[Symbol::Terminal::NEQ] = "<>";
  terminalMapped_[Symbol::Terminal::LESSER] = "<";
  terminalMapped_[Symbol::Terminal::GREATER] = ">";
  terminalMapped_[Symbol::Terminal::LESSEREQ] = "<=";
  terminalMapped_[Symbol::Terminal::GREATEREQ] = ">=";
  terminalMapped_[Symbol::Terminal::AND] = "&";
  terminalMapped_[Symbol::Terminal::OR] = "|";
  terminalMapped_[Symbol::Terminal::ASSIGN] = ":=";
  terminalMapped_[Symbol::Terminal::ID] = "id";
  terminalMapped_[Symbol::Terminal::INTLIT] = "intlit";
  terminalMapped_[Symbol::Terminal::FLOATLIT] = "floatlit";
  terminalMapped_[Symbol::Terminal::NULLL] = "epsilon";
  terminalMapped_[Symbol::Terminal::EOFF] = "";
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

  // output IR
  parser.ir_code();

  // Close all open files like
  parser.outFile.close();
  return 0;
}
