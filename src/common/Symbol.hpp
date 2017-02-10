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
    TIGER_PROGRAM,
    DECLARATION_SEGMENT,
    STAT_SEQ,
    CONST,
    EXPR,
    // Fill in the required Non-Terminals
  };

  enum Action {

    // Dealing with main
    MakeMainLabel,
    MakeReturn,
    //

    // Scope
    InitializeScope,
    FinalizeScope,

    // Fill in the required Actions

  };

  void operator=(int value) { this->value = value; }

  int getValue() { return value; }

private:
  int value;
};
