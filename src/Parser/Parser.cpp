/**
* Parser
*
* @author: Gatech Buzz
*/
#include  "../Scanner/Scanner.hpp"
#include "../SemanticAnalyzer/SymbolTable.hpp"
#include "../SemanticAnalyzer/SymbolTerminalPair.hpp"
#include "ParseTable.h"
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Parser {

private:
	Scanner scanner;
	int numErrors ;
	std::string globalFileName;
	std::stack<int> parseStack;
	std::map<int, std::string> terminalMapped;
	std::map<SymbolTerminalPair, std::vector<int>> parseTable;
	std::map<int, SymbolTable> globalSymbolTable;
	SymbolTable symTable[5];
	unsigned int scopeCount = 0;
	enum symbolType
	{
		TERMINAL,
		NON_TERMINAL,
		ID
	};
	enum errorType
	{
		NONE=0,
		INPUT_OVER=1,
		STACK_EMPTY=2,
		UNKNOWN
	};
public:
	std::ofstream outFile;
	bool printDebug ;
	Parser(std::string fileName):scanner(fileName){
		parseStack.push(Symbol::Terminal::EOFF);
		parseStack.push(Symbol::Nonterminal::TIGER_PROGRAM);

		initializeTerminalMapped();

		globalFileName = fileName;
		int tmparr[] = { Symbol::Action::FinalizeScope,
			Symbol::Terminal::END,
			Symbol::Nonterminal::STAT_SEQ,
			Symbol::Action::MakeMainLabel,
			Symbol::Terminal::IN,
			Symbol::Nonterminal::DECLARATION_SEGMENT,
			Symbol::Action::InitializeScope,
			Symbol::Terminal::LET};
		SymbolTerminalPair stp(Symbol::Nonterminal::TIGER_PROGRAM, Symbol::Terminal::LET);
		std::vector<int>rhsVec={std::begin(tmparr),std::end(tmparr)};
		//Insert the pair stp and rhsVec inside parseTable
		//Fill in the remaining of the parseTable as above. 
		//Recommended- work out the parseTable by hand and then code it here
	}

	void error(std::string message) {
		numErrors++;
		std::cout<< globalFileName << ":" << scanner.getCurrLine() << ": error: " << message << "\n";
	}
	
	void parse() {
		
		std::string parseStackTemp = "";
		Symbol focus;
		char **identifiers;
		int err=Parser::errorType::NONE;
		int max_size = 50;
		int non_ter_enum_start = 101;

		
		//as of now I am using identifiers as array of sstrings, it should go in symbol table
		identifiers = (char**)malloc(max_size * sizeof(char*));
		for(int i = 0; i < max_size; i++) {
			identifiers[i] = (char*)malloc(max_size * sizeof(char));
			strcpy(identifiers[i], "MY_NULL");
		}
		
		/* Used for the DEBUG FLAG */
		int currLine = scanner.getCurrLine();
		int lastLine = scanner.getCurrLine();
		char **strings;

		strings = (char**)malloc(max_size * sizeof(char*));
		for(int i = 0; i < max_size; i++) {
			strings[i] = (char*)malloc(max_size * sizeof(char));
			strcpy(strings[i], "MY_NULL");
		}
		
		unsigned int get_next_token = 1;
		TokenPair *word;
		std::string tokenStr;
		unsigned int tokenNum=0; 
		int identifier_index=0;
		while (1) {
			
			std::cout << "--------------------------------------------------" <<std::endl;
			if(parseStack.empty() == true){
				err |= Parser::errorType::STACK_EMPTY;
				std::cout << "stack is empty " << err <<std::endl;
				
			}
			if(err & Parser::errorType::STACK_EMPTY){
				std::cout<<"Done: return code is " <<err<<std::endl;
				break;
			}
			//get input token
			if(get_next_token){
				word = scanner.getToken();
				tokenStr = word->getTokenString();	
				tokenNum = get_token_enum(tokenStr);
				strcpy(&identifiers[identifier_index++][0],(char*)tokenStr.c_str());
				
				
				std::cout << "token is: " << word->emit()<<tokenNum <<'\n' <<std::endl;
				if(tokenNum == Symbol::Terminal::EOFF){
					err |= Parser::errorType::INPUT_OVER;
					std::cout << "input is over " << err <<std::endl;
				}
				if(tokenNum == Symbol::Terminal::SEMI){
					//create symbol  table entries atleast after getting a line
					createSymbolTableEntries(identifiers,identifier_index);
					for(int i = 0; i <= identifier_index; i++) {
						strcpy(identifiers[i], "");
					}
					identifier_index = 0;
					
				}
			}
			//pop stack
			int stackSymbolNum = parseStack.top();
			parseStack.pop();

			//check stack symbol type
			symbolType symType= get_symbol_type(stackSymbolNum);
			std::cout << "Stack symbol num is " << stackSymbolNum << ", type = "<<symType <<'\n'<<std::endl;

			
			if(symType == symbolType::NON_TERMINAL){
				
				std::cout << "Symbol type is non-terminal...going to read parse table " << stackSymbolNum << "  "<< tokenNum<<"  " <<TigerParseTable[stackSymbolNum-101][tokenNum] <<'\n' <<std::endl;
				
				//reset input strings
				for(int i = 0; i < max_size; i++) {
					strcpy(strings[i], "MY_NULL");
				}
				//valid non-terminal to read
				if(TigerParseTable[stackSymbolNum - non_ter_enum_start][tokenNum] != -1){
					//get parse rule for stacksymbol
					get_parse_rule(TigerParseTable[stackSymbolNum - non_ter_enum_start][tokenNum],strings);
					int i=max_size-1;
					while((i>=1)&&(strlen(strings[i])>0)){
						if((strcmp(strings[i],"MY_NULL")) != 0){
							//std::cout << "string is ********** " <<strings[i]<<strlen(strings[i])<<std::endl;
							//get token id for this string
							unsigned int tokenId = get_token_enum(strings[i]);
							if(tokenId != Symbol::Terminal::NULLL){
								parseStack.push(tokenId);
								std::cout << "Pushing in stack ---------->  " << tokenId << std::endl;
							}
						}
						i=i-1;
					}
					get_next_token = 0;
				 }
				else{
					std::cout << "need to check in identifier list " << std::endl;
				}
			}
			else if(symType == symbolType::TERMINAL)
			{
				std::cout << "Symbol type is terminal " << "stack sym is "<< stackSymbolNum <<'\n'<<std::endl;
				//match with input token
				if(stackSymbolNum == tokenNum){
					std::cout<< "Input symbol matched with stack top " <<std::endl;
					//move input read pointer
					get_next_token = 1;
				}
				else{
					get_next_token = 0;
				}
			}
			else if(symType == symbolType::ID)
			{
				std::cout << "Symbol type is identifier " << '\n' <<std::endl; 
				if(stackSymbolNum == tokenNum)
				{
					std::cout<< "Input symbol matched with stack top " <<std::endl;
					
					//move input read pointer
					get_next_token = 1;
				}
			}
			
		//complete parsing before going to semantic analysis
		//then embed the semantic actions in the parsing table and 
		//accordingly take actions and generate the IR.
		//Output the IR, only if there are no errors in the input.
		}

		for(int i=0;i<=scopeCount;i++)
			globalSymbolTable.insert(std::pair<int,SymbolTable>(scopeCount,symTable[i]));

		typedef std::map<int,SymbolTable>::const_iterator MapIterator;
		for (MapIterator iter = globalSymbolTable.begin(); iter != globalSymbolTable.end(); iter++)
		{
			globalSymbolTable[iter->first].print_symTable();
		}
		
	}



private:
	bool isPresentInSymbolTable(int scopeCount,std::string type,std::string name)
	{
		bool ret = false;
		typedef std::map<int,SymbolTable>::const_iterator MapIterator;
		for (MapIterator iter = globalSymbolTable.begin(); iter != globalSymbolTable.end(); iter++){
			ret = globalSymbolTable[iter->first].containsKey(type, name);
			if(ret == true)
				return ret;
		}
		
		return ret;
	}
	void createSymbolTableEntries(char** p,int len)
	{
		
		//std::stringstream ss(p[0]);
		  std::string to[10];
		  int num_line=0;

		  for(int i = 0; i < len; i++){

			if(*p[i] != ';'){
				to[num_line].append(p[i]);
				to[num_line].append(" ");
				}
			else{
				//to[num_line].append(" ");
				to[num_line].append(p[i]);
				num_line=num_line+1;
				}
		  }
		  
		  for(int i = 0; i <= num_line; i++){
		  	
  		  		  std::cout << to[i] <<std::endl;
				  std::string p = to[i];
				  std::size_t line_pos=0; 
				  while(p[line_pos] != ';'){

						//std::cout << "starting for line pos " << line_pos<<std::endl;
					  //create symbol table
					  std::size_t pos0 = to[i].find("let",line_pos,3);
					  if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
				      	//std::cout << "got let " << pos0 ;
						symTable[scopeCount] = SymbolTable(scopeCount);
						
				      }
					  
				  	  //find and create type record
					  pos0 = to[i].find("type",line_pos,4);     
				      if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
					      	//std::cout << "got type " << pos0 <<std::endl;
							pos0 = pos0+5;
							std::string type_var;
							while(to[i][pos0] != ' '){
								type_var += to[i][pos0];
								pos0++;
							} //end of while
							type_var += '\n';
							std::cout << "type var is " << type_var <<std::endl;
							pos0 = pos0+3;
							std::string type_type;
							while(to[i][pos0] != ' '){
								type_type += to[i][pos0];
								pos0++;
							} //end of while
							type_type += '\n';
							std::cout << "type type is " << type_type <<std::endl;
							if(type_type.compare(0,5,"array") == 0){
								
								pos0 = pos0+3;
								std::string dim;
								while(to[i][pos0] != ' '){
									dim += to[i][pos0];
									pos0++;
								} //end of while
								dim += '\n';
								
								pos0 = pos0+6;
								std::string array_type;
								while(to[i][pos0] != ' '){
									array_type += to[i][pos0];
									pos0++;
								} //end of while
								array_type += '\n';
								std::cout << "here array type is " << array_type ;
								if(isPresentInSymbolTable(scopeCount,"type",type_var) == false)
									symTable[scopeCount].insert(SymbolTable::symbolType::TYPE,type_var,type_type,dim,array_type);
							}
							else{

								if(isPresentInSymbolTable(scopeCount,"type",type_var) == false)
									symTable[scopeCount].insert(SymbolTable::symbolType::TYPE,type_var,type_type,"","");
							}
							//globalSymbolTable.insert(std::pair<int,SymbolTable>(scopeCount,symTable[scopeCount]));
							
				      }
					  
					  //find and create var record
					  pos0 = to[i].find("var",line_pos,3);      
				      if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
				      	//std::cout << "got var " << pos0 <<std::endl;
						pos0 = pos0+3;
						int var_index = 0;
						std::string var_name[10];
						//find var name
						while(to[i][pos0] != ':'){
								if((to[i][pos0] != ',')&&(to[i][pos0] != ' ')){
									while(to[i][pos0] != ' '){
										var_name[var_index] += to[i][pos0];
										pos0++;
										} //end of while
										var_name[var_index] += '\n';
										std::cout << "var name is " << var_name[var_index] <<std::endl;
										var_index++;
										
									}//end of not , and space
								else{
									pos0++;
									}
								
							}//end of :

						//find var type
						pos0 = pos0+2;
						std::string var_type;
						while(to[i][pos0] != ' '){
							var_type += to[i][pos0];
							//std::cout << "in while " << var_type;
							pos0++;
						} //end of while
						var_type += '\n';
						std::cout << "var type is " << var_type <<std::endl;
						
						for(int k=0;k<var_index;k++){
							if(isPresentInSymbolTable(scopeCount,"variable",var_name[k]) == false)
								symTable[scopeCount].insert(SymbolTable::symbolType::VARIABLE,var_name[k], var_type,"","");
						}
						//globalSymbolTable.insert(std::pair<int,SymbolTable>(scopeCount,symTable[scopeCount]));
					
				      }

					  //find and create function record
					  pos0 = to[i].find("function",line_pos,8); 
				      if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
				      	//std::cout << "got function " << pos0 <<std::endl;
						pos0 = pos0+9;
						int j = 0;
						std::string function_name;
						while(to[i][pos0] != ' '){
								function_name += to[i][pos0];
								pos0++;
								j++;
							}
						function_name += '\n';
						
						//search for param name
						pos0 = pos0 + 3;
						std::string param_name;
						while(to[i][pos0] != ' '){
								param_name += to[i][pos0];
								pos0++;
								j++;
							}
						param_name += '\n';
						//search for param type
						pos0 = pos0 + 3;
						std::string param_type;
						while(to[i][pos0] != ' '){
								param_type += to[i][pos0];
								pos0++;
								j++;
							}
						param_type += '\n';
						//std::cout << "function: " <<function_name << param_name <<param_type;
						if(isPresentInSymbolTable(scopeCount,"function",function_name) == false)
							symTable[scopeCount].insert(SymbolTable::symbolType::FUNCTION,function_name,param_name, param_type,"");
						  //globalSymbolTable.insert(std::pair<int,SymbolTable>(scopeCount,symTable[scopeCount]));
				      }//end of function

					  pos0 = to[i].find("begin",line_pos,5);
					  if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
				      	//std::cout << "got begin " << pos0<<std::endl;
						scopeCount++;
						symTable[scopeCount] = SymbolTable(scopeCount);
				      }
					  pos0 = to[i].find("end",line_pos,3);
					  if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
				      	//std::cout << "got end " << pos0 <<std::endl;
						
						if(scopeCount>0)
							scopeCount--;
				      }

					  line_pos++;
					  //std::cout << "line pos is "<<line_pos <<std::endl;
					  
				  	}//end of current line
			  
		  	}//end of all lines loop
		
	}
	void initializeTerminalMapped() {
		terminalMapped.insert(std::make_pair(Symbol::Terminal::COMMA, ","));
		//Map the other terminals accordingly
	}
	symbolType get_symbol_type(int stackSymbol)
	{
		if(stackSymbol ==  4)
			return symbolType::ID;
		else if((stackSymbol >=  0) && (stackSymbol <= 100))
			return symbolType::TERMINAL;
		else if(stackSymbol >  100)
			return symbolType::NON_TERMINAL;
			
	}
	unsigned int get_token_enum(std::string inString){

		
		std::cout << "input string is " << inString <<std::endl;
		
		if(strcmp("let",inString.c_str())==0)
		{
			return (Symbol::Terminal::LET);
		}
		else if(strcmp("in",inString.c_str())==0)
		{
			return (Symbol::Terminal::IN);
		}
		else if(strcmp("end",inString.c_str())==0)
		{
			return (Symbol::Terminal::END);
		}
		else if(strcmp("type",inString.c_str())==0)
		{
			return (Symbol::Terminal::TYPE);
		}
		else if(strcmp("id",inString.c_str())==0)
		{
			return (Symbol::Terminal::ID);
		}
		else if(strcmp("=",inString.c_str())==0)
		{
			return (Symbol::Terminal::EQ);
		}
		else if(strcmp(";",inString.c_str())==0)
		{
			return (Symbol::Terminal::SEMI);
		}

		else if(strcmp("array",inString.c_str())==0)
		{
			return (Symbol::Terminal::ARRAY);
		}
		else if(strcmp("[",inString.c_str())==0)
		{
			return (Symbol::Terminal::LBRACK);
		}
		else if(strcmp("intlit",inString.c_str())==0)
		{
			return (Symbol::Terminal::INTLIT);
		}
		else if(strcmp("]",inString.c_str())==0)
		{
			return (Symbol::Terminal::RBRACK);
		}

		else if(strcmp("of",inString.c_str())==0)
		{
			return (Symbol::Terminal::OF);
		}
		else if(strcmp("int",inString.c_str())==0)
		{
			return (Symbol::Terminal::INT);
		}
		else if(strcmp("float",inString.c_str())==0)
		{
			return (Symbol::Terminal::FLOAT);
		}
		else if(strcmp("var",inString.c_str())==0)
		{
			return (Symbol::Terminal::VAR);
		}
		else if(strcmp(":",inString.c_str())==0)
		{
			return (Symbol::Terminal::COLON);
		}
		else if(strcmp(",",inString.c_str())==0)
		{
			return (Symbol::Terminal::COMMA);
		}
		else if(strcmp(":=",inString.c_str())==0)
		{
			return (Symbol::Terminal::ASSIGN);
		}
		else if(strcmp("function",inString.c_str())==0)
		{
			return (Symbol::Terminal::FUNCTION);
		}
		else if(strcmp("(",inString.c_str())==0)
		{
			return (Symbol::Terminal::LPAREN);
		}
		
		else if(strcmp(")",inString.c_str())==0)
		{
			return (Symbol::Terminal::RPAREN);
		}
		else if(strcmp("begin",inString.c_str())==0)
		{
			return (Symbol::Terminal::BEGIN);
		}
		else if(strcmp("if",inString.c_str())==0)
		{
			return (Symbol::Terminal::IF);
		}
		else if(strcmp("then",inString.c_str())==0)
		{
			return (Symbol::Terminal::THEN);
		}
		else if(strcmp("while",inString.c_str())==0)
		{
			return (Symbol::Terminal::WHILE);
		}
		else if(strcmp("do",inString.c_str())==0)
		{
			return (Symbol::Terminal::DO);
		}
		else if(strcmp("enddo",inString.c_str())==0)
		{
			return (Symbol::Terminal::ENDDO);
		}
		else if(strcmp("for",inString.c_str())==0)
		{
			return (Symbol::Terminal::FOR);
		}
		else if(strcmp("to",inString.c_str())==0)
		{
			return (Symbol::Terminal::TO);
		}
		else if(strcmp("break",inString.c_str())==0)
		{
			return (Symbol::Terminal::BREAK);
		}
		else if(strcmp("return",inString.c_str())==0)
		{
			return (Symbol::Terminal::RETURN);
		}
		else if(strcmp("else",inString.c_str())==0)
		{
			return (Symbol::Terminal::ELSE);
		}
		else if(strcmp("endif",inString.c_str())==0)
		{
			return (Symbol::Terminal::ENDIF);
		}
		else if(strcmp("&",inString.c_str())==0)
		{
			return (Symbol::Terminal::AND);
		}
		else if(strcmp("|",inString.c_str())==0)
		{
			return (Symbol::Terminal::OR);
		}
		else if(strcmp("<>",inString.c_str())==0)
		{
			return (Symbol::Terminal::NEQ);
		}
		else if(strcmp(">",inString.c_str())==0)
		{
			return (Symbol::Terminal::GREATER);
		}
		else if(strcmp("<",inString.c_str())==0)
		{
			return (Symbol::Terminal::LESSER);
		}
		else if(strcmp(">=",inString.c_str())==0)
		{
			return (Symbol::Terminal::GREATEREQ);
		}
		else if(strcmp("<=",inString.c_str())==0)
		{
			return (Symbol::Terminal::LESSEREQ);
		}
		else if(strcmp("+",inString.c_str())==0)
		{
			return (Symbol::Terminal::PLUS);
		}
		else if(strcmp("-",inString.c_str())==0)
		{
			return (Symbol::Terminal::MINUS);
		}
		else if(strcmp("*",inString.c_str())==0)
		{
			return (Symbol::Terminal::MULT);
		}
		else if(strcmp("/",inString.c_str())==0)
		{
			return (Symbol::Terminal::DIV);
		}
		else if(strcmp("floatlit",inString.c_str())==0)
		{
			return (Symbol::Terminal::FLOATLIT);
		}
		else if(strcmp("epsilon",inString.c_str())==0)
		{
			return (Symbol::Terminal::NULLL);
		}
		else if(strcmp("",inString.c_str())==0)
		{
			return (Symbol::Terminal::EOFF);
		}
		else if(strcmp("tiger_program",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::TIGER_PROGRAM);
		}
		else if(strcmp("declaration-segment",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::DECLARATION_SEGMENT);
		}
		else if(strcmp("stat",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT);
		}
		else if(strcmp("stat-seq",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT_SEQ);
		}
		else if(strcmp("type-declaration-list",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::TYPE_DECLARATION_LIST);
		}
		else if(strcmp("var-declaration-list",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::VAR_DECLARATION_LIST);
		}
		else if(strcmp("funct-declaration-list",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::FUNCT_DECLARATION_LIST);
		}
		else if(strcmp("type-declaration",inString.c_str())==0)  
		{
			return (Symbol::Nonterminal::TYPE_DECLARATION);
		}
		else if(strcmp("var-declaration",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::VAR_DECLARATION);
		}
		else if(strcmp("funct-declaration",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::FUNCT_DECLARATION);
		}
		else if(strcmp("type'",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::TYPE_NONTER);
		}
		else if(strcmp("type-id",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::TYPE_ID);
		}
		else if(strcmp("id-list",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::ID_LIST);
		}
		else if(strcmp("optional-init",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::OPTIONAL_INIT);
		}
		else if(strcmp("id-list-tail",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::ID_LIST_TAIL);
		}
		else if(strcmp("const",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::CONST);
		}
		else if(strcmp("param-list",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::PARAM_LIST);
		}
		else if(strcmp("ret-type",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::RET_TYPE);
		}
		else if(strcmp("param",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::PARAM);
		}
		else if(strcmp("param-list-tail",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::PARAM_LIST_TAIL);
		}
		else if(strcmp("stat-seq-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT_SEQ_TAIL);
		}
		else if(strcmp("expr",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::EXPR);
		}
		else if(strcmp("stat-if-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT_IF_TAIL);
		}
		else if(strcmp("stat-funct-or-assign",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT_FUNCT_OR_ASSIGN);
		}
		else if(strcmp("lvalue",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::LVALUE);
		}
		else if(strcmp("lvalue-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::LVALUE_TAIL);
		}
		else if(strcmp("stat-assign",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT_ASSIGN);
		}
		else if(strcmp("stat-assign-stuff",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT_ASSIGN_STUFF);
		}
		else if(strcmp("stat-assign-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::STAT_ASSIGN_TAIL);
		}
		else if(strcmp("expr-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::EXPR_TAIL);
		}
		else if(strcmp("OR-expr",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::OR_EXPR);
		}
		else if(strcmp("OR-expr-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::OR_EXPR_TAIL);
		}
		else if(strcmp("AND-expr",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::AND_EXPR);
		}
		else if(strcmp("AND-expr-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::AND_EXPR_TAIL);
		}
		else if(strcmp("compare",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::COMPARE);
		}
		else if(strcmp("compare-op",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::COMPARE_OP);
		}
		else if(strcmp("compare-tail",inString.c_str())==0) 
		{
			return (Symbol::Nonterminal::COMPARE_TAIL);
		}
		else if(strcmp("term",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::TERM);
		}
		else if(strcmp("term-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::TERM_TAIL);
		}
		else if(strcmp("OR-op",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::OR_OP);
		}
		else if(strcmp("AND-op",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::AND_OP);
		}
		else if(strcmp("factor",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::FACTOR);
		}
		else if(strcmp("add-op",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::ADD_OP);
		}
		else if(strcmp("mul-op",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::MUL_OP);
		}
		
		else if(strcmp("expr-list",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::EXPR_LIST);
		}
		else if(strcmp("expr-list-tail",inString.c_str())==0)
		{
			return (Symbol::Nonterminal::EXPR_LIST_TAIL);
		}
		else{ //need to discuss this part
			//check if it is a const	
			int ret = checkType(inString.c_str());
			//std::cout << "came here for " << inString.c_str() << " " << ret <<std::endl;
			
			//??? return identifier
			if(ret == -1)
				return (Symbol::Terminal::ID);
			else
				return ret;
		}
		
	}
	int checkType(const char *input)
	{
	    short int isInt=0;//we use int as a boolean value;
	    short int isFloat=0;
	    short int isUndifined=0;
	    int count;
		int len = strlen(input);
	    for(count = 0 ; count<len; count++)
	    {
	        if(isdigit(input[count])){//you should include ctype.h at the beginning of this program
	            isInt=1;
				//std::cout << "got int ";
	        }
	        else if(input[count] == '.'){
	            isFloat=1;
				//std::cout << "got float ";
	        	}
	        else{
				//std::cout << "dnt know ";
	            return -1;//some character that neither int nor '.' char.
	        	}
	    }
	    if(isInt == 1 && isFloat ==1){
			//std::cout << "Returning float ";
	        return (Symbol::Terminal::FLOATLIT);
	    	}
	    else if(isInt == 1 && isFloat ==0){
			//std::cout << "Returning int ";
	        return (Symbol::Terminal::INTLIT);
	    	}
	    else{
			
			//std::cout << "Returning unknown";
	        return -1;//illegal format
	    	}
	}
	void get_parse_rule(unsigned int ruleNum, char** p)
	{
			
	    std::ifstream file;
		file.open("TigerGrammar.txt");
	    std::string line; 
		if(file.is_open())
		{
			int rule_count = 0;
			while(1)
			{
				if(rule_count == ruleNum)
					break;
				std::getline(file, line);
				rule_count+=1;
			}
			while(std::getline(file, line))
			{
				//std::cout << line;
				const char * c = line.c_str();
				std::string delimiter = " ";
				unsigned int pos = 0;
				std::string token;
				int i = 0;
				int rem_len = 0;
				
				rem_len = line.length();
				
				while(line.empty()==false) {
		
					pos = line.find(delimiter);
					token = line.substr(0, pos);
					line.erase(0, pos + delimiter.length());
					rem_len = line.length();
					
					strcpy(&p[i][0],(char*)token.c_str());
					//std::cout <<p[i] << strlen(p[i])<<'\n';
					i=i+1;
				}
				break;
			}
		}
		else
			std::cerr << "Error: " << strerror(errno);
		//std::cout<<"-------------------------------------"<<std::endl;
	}
};


/**
* Prints helpful information for the user if they try to run the
* program with a bad number of arguments
*/
void printHelp() {
	std::cout<<
		"\n" << "You have entered an incorrect number of arguments."
		<< "\n" << "\n"
		<< "Please specify the file you wish to parse and" << "\n" <<
		"optionally whether you want to flag for debugging." << "\n"
		<< "\n" << "> parser <filename> -d" << "\n"
		;
}
/**
* There are two arguments: one mandatory and one optional
*
* The mandatory argument is the file to perform a parse on while the
* optional flag -d indicates that the user wants to print debug info.,
* namely the sequences of Tokens as the Scanner reads them
*/
int main(int argc, char** argv){
	// The user has given us a bad number of args
	if (argc > 3 || argc < 1) {
		printHelp();
		return 0;
	}

	// Initialize the Parser with the given filename
	Parser parser (argv[1]);

	// Print debug info. if flagged
	if (argc > 2 && strcmp(argv[2],"-d") == 0)
		parser.printDebug = true;

	if (argc > 2 && strcmp(argv[2],"-d") != 0)
		std::cout<<"\nPlease use \"-d\" as flag for debugging.\n";

	

	// Pass
	parser.parse();

	//Close all open files like
	parser.outFile.close();
	return 0;
}
