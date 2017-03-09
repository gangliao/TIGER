/**
 * Terminal
 *
 * @author:  Gatech Buzz
 */

#pragma once

#include <ctype.h>
#include <cstdio>
#include <fstream>
#include <iostream>

#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <string>

enum Entry { Variables, Constants, Types, Functions, Temporaries };

class Symbol {
 public:
  enum Terminal {
    // Keyword tokens
    ARRAY = 0,
    BREAK,
    DO,
    ELSE,
    END,
    FOR,
    FUNCTION,
    IF,
    IN,
    LET,
    OF,
    THEN,
    TO,
    TYPE,
    VAR,
    WHILE,
    ENDIF,
    BEGIN,
    ENDDO,
    RETURN,
    INT,
    FLOAT,

    // Regular tokens
    COMMA,
    COLON,
    SEMI,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
    PERIOD,
    PLUS,
    MINUS,
    MULT,
    DIV,
    EQ,
    NEQ,
    LESSER,
    GREATER,
    LESSEREQ,
    GREATEREQ,
    AND,
    OR,
    ASSIGN,
    //
    // Type tokens
    ID,
    INTLIT,
    FLOATLIT,

    // NULL token
    NULLL,

    // End of file token
    EOFF
  };

  enum Nonterminal {
    TIGER_PROGRAM = 100,
    DECLARATION_SEGMENT,
    STAT_SEQ,
    TYPE_DECLARATION_LIST,
    VAR_DECLARATION_LIST,
    FUNCT_DECLARATION_LIST,
    TYPE_DECLARATION,
    VAR_DECLARATION,
    FUNCT_DECLARATION,
    TYPE_EXPR,
    TYPE_ID,
    ID_LIST,
    OPTIONAL_INIT,
    ID_LIST_TAIL,
    CONST,
    PARAM_LIST,
    RET_TYPE,
    PARAM,
    PARAM_LIST_TAIL,
    STAT,
    STAT_SEQ_TAIL,
    EXPR,
    STAT_IF_TAIL,
    STAT_FUNCT_OR_ASSIGN,
    LVALUE_TAIL,
    STAT_ASSIGN,
    EXPR_LIST,
    STAT_ASSIGN_STUFF,
    STAT_ASSIGN_TAIL,
    EXPR_TAIL,
    OR_EXPR_TAIL,
    AND_EXPR_TAIL,
    COMPARE_TAIL,
    TERM_TAIL,
    OR_EXPR,
    OR_OP,
    AND_EXPR,
    AND_OP,
    COMPARE,
    COMPARE_OP,
    TERM,
    ADD_OP,
    FACTOR,
    MUL_OP,
    LVALUE,
    EXPR_LIST_TAIL
  };

  enum Action {
    // Dealing with main
    MakeMainLabel = 200,
    MakeReturn,

    // Scope
    InitializeScope,
    FinalizeScope,

    // type
    MakeTypesBegin,
    MakeTypesEnd,

    // variable
    MakeVariablesBegin,
    MakeVariablesEnd,

    // function
    MakeFunctionsBegin,
    MakeFunctionsEnd,

    // assign
    MakeAssignBegin,
    MakeAssignEnd,

    // expression
    MakeExprBegin,
    MakeExprEnd
  };

  void operator=(int value) { this->value = value; }

  int getValue() const { return value; }

 private:
  int value;
};
