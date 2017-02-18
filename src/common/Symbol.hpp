/**
 * Terminal
 *
 * @author:  Gatech Buzz
 */
#include<cstdio>
#include<iostream>
#include<fstream>
#include<ctype.h>

#include<algorithm>
#include<string>
#include<set>
#include <stack>
#include <map>


enum Entry {Variables, Constants, Types, Functions, Temporaries
};
  
class Symbol
{
    public:
    enum Terminal{
    // Keyword tokens
	LET=0,
	IN,
	END,
	TYPE,
	ID,
	EQ,
	SEMI,
	ARRAY,
	LBRACK,//[
	INTLIT,
	RBRACK, //]
	OF,
	INT,
	FLOAT,
	VAR,
	COLON,
	COMMA,
	ASSIGN, //:=
	FUNCTION,
	LPAREN, //(
	RPAREN, //)
	BEGIN,
	IF,
	THEN,
	WHILE,
	DO,
	ENDDO,
	FOR,
	TO,
	BREAK,
	RETURN, 
	ELSE,
	ENDIF,  
	AND,
	OR,
	NEQ,
	GREATER,
	LESSER,
	GREATEREQ,
	LESSEREQ,
	PLUS,
	MINUS,
	MULT,
	DIV,
	FLOATLIT,
	// End of file token
	EOFF, //$
	LBRACE,
	RBRACE,
	PERIOD,
	// NULL token
	NULLL
    };

     enum Nonterminal{
	TIGER_PROGRAM=101, 
	DECLARATION_SEGMENT,
	STAT_SEQ,
	TYPE_DECLARATION_LIST,
	VAR_DECLARATION_LIST,
	FUNCT_DECLARATION_LIST,
	TYPE_DECLARATION,
	VAR_DECLARATION,
	FUNCT_DECLARATION,
	TYPE_NONTER,
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
	//Fill in the required Non-Terminals
    };

    enum Action{

	// Dealing with main
	MakeMainLabel, MakeReturn,
    //

	// Scope
	InitializeScope, FinalizeScope,

	//Fill in the required Actions

    };

  void operator=(int value){
	this->value = value;
  }

  int getValue(){
    return value;
  }
private:
  int value;
};
