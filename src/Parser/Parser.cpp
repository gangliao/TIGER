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
	std::stack<std::string> tempStack;
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
	enum idType
	{
		INT=0,
		FLOAT,
		ID_NONE
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
		char **codeForIr;
		int err=Parser::errorType::NONE;
		int max_size = 100;
		int non_ter_enum_start = 101;
		bool enableSemCheckForBinaryOperands=false;
		bool enableSemCheckForLoops = false;
		bool enableSemCheckFunction = false;
		
		identifiers = (char**)malloc(max_size * sizeof(char*));
		for(int i = 0; i < max_size; i++) {
			identifiers[i] = (char*)malloc(max_size * sizeof(char));
			strcpy(identifiers[i], "MY_NULL");
		}
		//this array stores the code for IR code generation
		codeForIr = (char**)malloc(max_size * sizeof(char*));
		for(int i = 0; i < max_size; i++) {
			codeForIr[i] = (char*)malloc(max_size * sizeof(char));
			strcpy(codeForIr[i], "MY_NULL");
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
		int codeForIr_index=0;
		int declarationSectionComplete = 0;
		
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
				strcpy(&codeForIr[codeForIr_index++][0],(char*)tokenStr.c_str());
				
				std::cout << "token is: " << word->emit()<<tokenNum <<'\n' <<std::endl;
				if(tokenNum == Symbol::Terminal::EOFF){
					err |= Parser::errorType::INPUT_OVER;
					std::cout << "input is over " << err <<std::endl;
				}
				//if tokens are binary operators, enable smantic check
				if((tokenNum == Symbol::Terminal::PLUS)||
					(tokenNum == Symbol::Terminal::MINUS)||
					(tokenNum == Symbol::Terminal::MULT)||
					(tokenNum == Symbol::Terminal::DIV) ||
					(tokenNum == Symbol::Terminal::OR) ||
					(tokenNum == Symbol::Terminal::AND)){
						enableSemCheckForBinaryOperands = true;
					}
				if((tokenNum == Symbol::Terminal::FOR)||
					(tokenNum == Symbol::Terminal::WHILE)||
					(tokenNum == Symbol::Terminal::IF)){
						enableSemCheckForLoops = true;
					}
				if(tokenNum == Symbol::Terminal::SEMI){
					//create symbol  table entries atleast after getting a line
					createSymbolTableEntries(identifiers,identifier_index);
					
					if(enableSemCheckForBinaryOperands){
						bool compatible = performSemanticCheck_BinaryOperands(identifiers,identifier_index);
						if(!compatible)
							std::cout << "!!!! ERROR !!!! Binary operands: LHS, RHS types are not compatible " <<std::endl;
						enableSemCheckForBinaryOperands = 0;
					}
					if(enableSemCheckForLoops){
						bool compatible = performSemanticCheck_Loops(identifiers,identifier_index);
						enableSemCheckForLoops = 0;
						if(!compatible)
							std::cout << "!!!! ERROR !!!! Expression: LHS, RHS types are not compatible " <<std::endl;
					}
					if(enableSemCheckFunction){
						bool compatible = performSemanticCheck_Function(identifiers,identifier_index);
						enableSemCheckFunction = 0;
					}
					for(int i = 0; i <= identifier_index; i++) {
						strcpy(identifiers[i], "");
					}
					identifier_index = 0;	
				}
				
				if(tokenNum == Symbol::Terminal::IN){
					for(int i=0;i<=scopeCount;i++)
						globalSymbolTable.insert(std::pair<int,SymbolTable>(scopeCount,symTable[i]));
					declarationSectionComplete=1;
					}
			}
			//pop stack
			int stackSymbolNum = parseStack.top();
			parseStack.pop();

			//check stack symbol type
			symbolType symType= get_symbol_type(stackSymbolNum);
			std::cout << "Stack symbol num is " << stackSymbolNum << ", type = "<<symType <<'\n'<<std::endl;

			
			if(symType == symbolType::NON_TERMINAL){
				
				std::cout << "Symbol type is non-terminal...going to read parse table " << stackSymbolNum << "  "<< tokenNum<<"  " <<TigerParseTable[stackSymbolNum-non_ter_enum_start][tokenNum] <<'\n' <<std::endl;
				
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
							std::cout << "input string is " << strings[i] <<std::endl;
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
				if(declarationSectionComplete){
					//check identifier for all the types
					if(isPresentInSymbolTable("variable",tokenStr)){

						std::cout << "Got variable name is sym  table " << std::endl;
					}
					else if(isPresentInSymbolTable("function",tokenStr)){

						std::cout << "Got function name is sym  table " << std::endl;
						enableSemCheckFunction=true;
					}
					else if(isPresentInSymbolTable("type",tokenStr)){
						std::cout << "Got type name is sym  table " << std::endl;
					}
				}
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
		std::cout << "********** Symbol table **************" << std::endl;
		typedef std::map<int,SymbolTable>::const_iterator MapIterator;
		for (MapIterator iter = globalSymbolTable.begin(); iter != globalSymbolTable.end(); iter++)
		{
			globalSymbolTable[iter->first].print_symTable();
		}

		generateIRCode(codeForIr,codeForIr_index);
	}



private:
	void generateIRCode(char** input,int codeForIr_index)
	{
		std::string to[100];
		std::string irCode[100];
		bool enable_assign_code = false;
		bool enable_binary_op_code = false;
		bool enable_function_code = false;
		int num_line=0;
		int ir_code_index=0;
		std::string prev_string;
		std::string op_lhs;
		
		for(int i = 0; i < codeForIr_index; i++){
			if(*input[i] != ';'){
				to[num_line].append(input[i]);
				to[num_line].append(" ");
				}
			else{
				to[num_line].append(input[i]);
				num_line=num_line+1;
				}
		}
		
		for (int i=0;i<=num_line;i++){

			std::string p = to[i];
			std::cout << p <<std::endl;
			int line_pos = 0;
			std::string rhs;
			while(p[line_pos] != ';'){
				
				std::size_t pos = to[i].find(":=",line_pos,2);
				if((pos < strlen(to[i].c_str()))&&(pos==line_pos)){

					std::string op;
					op = get_operator(to,i);
					if(strcmp("",op.c_str())==0){
					
							//get rhs
							pos = pos +3;
							std::string var;
							while(p[pos] != ' '){
								var += to[i][pos];
								pos++;
							}
							rhs = var;
							
							//get lhs
							pos = 0;
							while(p[pos] != ';'){
								var = "";
								int tokenNum = 0;
								while(p[pos] != ' '){
									var += to[i][pos];
									pos++;
								}
								tokenNum = get_token_enum(var);
								if(tokenNum == Symbol::Terminal::ID){
									tempStack.push(var);
									//std::cout << "got lhs " <<var <<std::endl;
								}
								pos++;
							}//end of lhs
							tempStack.push(rhs);
							enable_assign_code = true;

						}//operator not present
						else{

							
							//get op lhs
							int op_pos = pos - 2;
							op_lhs = "";
							std::string var;
							while(p[op_pos] != ' '){
								var += p[op_pos];
								op_pos--;
								if(op_pos < 0)
									break;
							}
							
							std::reverse(var.begin(), var.end());
							op_lhs = var;
							//std::cout << "op lhs is " <<  op_lhs;
							
							std::string lhs;
							//get op lhs
							pos = pos + 3;
							var="";
							while(p[pos] != ' '){
								var += p[pos];
								pos++;
							}
							lhs = var;

							//find rhs of op
							pos = pos + 3;
							while(p[pos] != ' '){
								
								std::string var;
								while(p[pos] != ' '){
									var = p[pos];
									pos++;
								}
								rhs = var;
						
							}
							//push poperator
							tempStack.push(lhs);
							tempStack.push(rhs);
							tempStack.push(op);
							enable_binary_op_code = true;
							
						}
					
			   }
				
			   else{
			   		
					std::string var_name;
					while(p[line_pos] != ' '){
						var_name += p[line_pos];
						line_pos++;
					} //end of while
					
					
					int tokenNum = get_token_enum(var_name);
					if(tokenNum == Symbol::Terminal::END){
						break;
					}
					if((tokenNum == Symbol::Terminal::ID)&&(isPresentInSymbolTable("function",var_name))
						&&(prev_string != "function")){
						
						//get the return type
						int idx = line_pos - var_name.length();
						idx = idx - 4;
						std::string ret_var="";
						if(idx >= 0){
							while(p[idx] != ' '){
								ret_var += p[idx];
								idx--;
								if(idx < 0)
									break;
							}
						}
						
						//get the list of arguements
						while(p[line_pos] != ';'){
							std::string var1="";
							while(p[line_pos] != ' '){
								var1 += p[line_pos];
								line_pos++;
							}
							
							int tokenNum = get_token_enum(var1);
							if((tokenNum == Symbol::Terminal::ID)||(tokenNum == Symbol::Terminal::INTLIT)
								||(tokenNum == Symbol::Terminal::FLOATLIT)){
								tempStack.push(var1);
							}
							line_pos++;
						}
						if(p[line_pos] == ';'){
							line_pos--;
						}
						
						enable_function_code = true;
						//push function name
						tempStack.push(var_name);
						
						if(strcmp("",ret_var.c_str())!=0){
							tempStack.push(ret_var);
						}
					}
					prev_string = var_name;
			   	}
			   
			   line_pos++;
			}
			if(enable_assign_code){
				irCode[ir_code_index] = IR_Code_assign();
				enable_assign_code = false;
				ir_code_index++;
			}
			if(enable_binary_op_code){
				irCode[ir_code_index] = IR_Code_binary_operands();
				enable_binary_op_code = false;
				ir_code_index++;
				
				tempStack.push(op_lhs);
				tempStack.push("t1");
				irCode[ir_code_index] = IR_Code_assign();
				ir_code_index++;
			}
			if(enable_function_code){
				
				irCode[ir_code_index] = IR_Code_function();
				ir_code_index++;
				enable_function_code = false;
			}
			
		}
		std::cout << "********** IR code **************" << std::endl;
		for (int i =0;i<ir_code_index;i++)
			std::cout << irCode[i] << std::endl;
	}
	
	std::string get_operator(std::string input[100],int line_num)
	{
		int pos = 0;
		std::string out;
		std::string p = input[line_num];
		
		while(p[pos] != ';'){
			
			int i = pos;
			std::size_t res_pos = input[line_num].find("+",pos,1);
			if((res_pos < strlen(input[line_num].c_str()))&&(pos==res_pos)){
				out = "+";
				break;
			}
			res_pos = input[line_num].find("-",pos,1);
			if((res_pos < strlen(input[line_num].c_str()))&&(pos==res_pos)){
				out = "-";
				break;
			}
			res_pos = input[line_num].find("*",pos,1);
			if((res_pos < strlen(input[line_num].c_str()))&&(pos==res_pos)){
				out = "*";
				break;
			}
			res_pos = input[line_num].find("/",pos,1);
			if((res_pos < strlen(input[line_num].c_str()))&&(pos==res_pos)){
				out = "/";
				break;
			}
			res_pos = input[line_num].find("&",pos,1);
			if((res_pos < strlen(input[line_num].c_str()))&&(pos==res_pos)){
				out = "&";
				break;
			}
			res_pos = input[line_num].find("|",pos,1);
			if((res_pos < strlen(input[line_num].c_str()))&&(pos==res_pos)){
				out = "|";
				break;
			}
			pos++;
		}//end of lhs
		return out;
	}
	std::string IR_Code_function()
	{

		std::string out;
		std::string func_name;
		
		std::string ret = tempStack.top();
		tempStack.pop();

		int tokenNum = get_token_enum(ret);
		if((tokenNum == Symbol::Terminal::ID)&&(isPresentInSymbolTable("function",ret))){
			func_name = ret;
			out = "call";
			out += ",";
			out += func_name;
			out += ",";
		}
		else{	 
			func_name = tempStack.top();
			tempStack.pop();
			out = "callr";
			out += ",";
			out += ret;
			out += ",";
			out += func_name;
			out += ",";
		}

		std::string var;
		int idx=0;
		while(tempStack.empty() == false){
			var = tempStack.top();
			tempStack.pop();
			out += var;
			out += ",";
			idx++;
		}
		return out;	
	}
	std::string IR_Code_assign()
	{
		std::string out;
		std::string rhs = tempStack.top();
		tempStack.pop();

		out = "assign";
		out += ",";
		std::string var;
		while(tempStack.empty() == false){
			var = tempStack.top();
			tempStack.pop();
			out += var;
			out += ",";
		}
		out += rhs;
		//std::cout << "Assign string is "  << " "<<rhs << " " << " " << out <<std::endl;
		return out;
	}
	std::string IR_Code_binary_operands()
	{
		std::string out;
		std::string op = tempStack.top();
		tempStack.pop();

		std::string rhs = tempStack.top();
		tempStack.pop();

		std::string lhs = tempStack.top();
		tempStack.pop();

		std::string var;
		int tokenNum = get_token_enum(var);

		if(strcmp("+",op.c_str())==0){
			out = "add";
		}
		else if(strcmp("-",op.c_str())==0){
			out = "sub";
		}
		else if(strcmp("*",op.c_str())==0){
			out = "mult";
		}
		else if(strcmp("/",op.c_str())==0){
			out = "div";
		}
		else if(strcmp("&",op.c_str())==0){
			out = "and";
		}
		else if(strcmp("|",op.c_str())==0){
			out = "or";
		}
		out += ",";
		out += lhs;
		out += ",";
		out += rhs;
		out += ",";
		out += "t1";
		//std::cout << "binary op string is " << op << " "<<rhs << " " << " " << out <<std::endl;
		return out;
	}
	bool isPresentInSymbolTable(std::string type,std::string name)
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
	
	bool performSemanticCheck_Function(char** p,int len)
	{
		std::string to[50];
		int num_line=0;
		int lhs_actual_type = Parser::idType::ID_NONE;
		int rhs_actual_type = Parser::idType::ID_NONE;
		for(int i = 0; i < len; i++){
			if(*p[i] != ';'){
					to[num_line].append(p[i]);
					to[num_line].append(" ");
					}
				else{
					to[num_line].append(p[i]);
					num_line=num_line+1;
					}
		}
		int pos = 0;
		int tokenNum = 0;
		std::string var;
		int expected_func_param_type[10];
		int actual_func_param_type[10];
		int exp_f_p_index = 0;
		int act_f_p_index = 0;
		int expected_return_type = Parser::idType::ID_NONE;
		int actual_return_type = Parser::idType::ID_NONE;
		bool enable_return_type_check = false;
		bool parsed_function_name = false;
		for(int i = 0; i < num_line; i++){

			std::string in = to[i];
			//get identifier which is function name
			while(in[pos] != ';'){
					var = "";
					while(to[i][pos] != ' '){
						var += to[i][pos];
						pos++;
					} //end of while
					tokenNum = get_token_enum(var);
					if((tokenNum == Symbol::Terminal::ID)&&(isPresentInSymbolTable("function",var))){
							parsed_function_name = true;
							typedef std::map<int,SymbolTable>::const_iterator MapIterator;
							for (MapIterator iter = globalSymbolTable.begin(); iter != globalSymbolTable.end(); iter++){
								Record rec(0);
								globalSymbolTable[iter->first].getRecord("function",var,rec);
								if(rec.name != ""){

									for (int k = 0; k < rec.parameterTypes.size(); k++) {
									  if(strcmp("int",rec.parameterTypes[k].c_str())==0){
									  	expected_func_param_type[exp_f_p_index] = Parser::idType::INT;
										//std::cout << "Record type is " << expected_func_param_type[exp_f_p_index] <<std::endl;
										exp_f_p_index++;
									  }
									  else if(strcmp("float",rec.parameterTypes[k].c_str())==0){
									  	expected_func_param_type[exp_f_p_index] = Parser::idType::FLOAT;
										//std::cout << "Record type is " << expected_func_param_type[exp_f_p_index] <<std::endl;
										exp_f_p_index++;
									  }
									  
									}
									if(strcmp("int",rec.returnType.c_str())==0){
										expected_return_type = Parser::idType::INT;
									}
									else if(strcmp("float",rec.returnType.c_str())==0){
										expected_return_type = Parser::idType::FLOAT;
									}
									else{
										expected_return_type = Parser::idType::ID_NONE;
									}
										
								}
							}
						}
						else if(tokenNum == Symbol::Terminal::FLOATLIT){

							actual_func_param_type[act_f_p_index] = Parser::idType::FLOAT;
							//std::cout << "got floatlit " <<actual_func_param_type[act_f_p_index] << std::endl;
							act_f_p_index++;
						}
						else if(tokenNum == Symbol::Terminal::INTLIT){

							actual_func_param_type[act_f_p_index] = Parser::idType::INT;
							//std::cout << "got intlit " <<actual_func_param_type[act_f_p_index] << std::endl;
							act_f_p_index++;
						}
						else if(tokenNum == Symbol::Terminal::ID){
							
							int var_type = Parser::idType::ID_NONE;
							typedef std::map<int,SymbolTable>::const_iterator MapIterator;
							for (MapIterator iter = globalSymbolTable.begin(); iter != globalSymbolTable.end(); iter++){
								Record rec(0);
								globalSymbolTable[iter->first].getRecord("variable",var,rec);
								if(rec.name != ""){

									  if(strcmp("int",rec.parameterTypes[0].c_str())==0){
									  	var_type = Parser::idType::INT;
										//std::cout << "Record type is Id" << var_type <<std::endl;
									  }
									  else if(strcmp("float",rec.parameterTypes[0].c_str())==0){
									  	var_type = Parser::idType::FLOAT;
										//std::cout << "Record type is Id" << var_type <<std::endl;
									  }
								}
							}
						
							//can be return val or function param
							if(parsed_function_name == false){ //return type
								enable_return_type_check = true;
								actual_return_type = var_type;
							}
							else{
								actual_func_param_type[act_f_p_index] = var_type;
								act_f_p_index++;
							}
						}
						pos++;
					}
				
			}
			if(enable_return_type_check){
				if(expected_return_type != Parser::idType::ID_NONE){
					if(expected_return_type == actual_return_type){
						//std::cout << "Return type match " <<expected_return_type<< " " <<actual_return_type << std::endl;
					}
					else{
						std::cout << "!!!! ERROR !!!! Function Return type mis-match. "<<expected_return_type<< " " <<actual_return_type << std::endl;
					}
				}
			}
			if(exp_f_p_index != act_f_p_index){
				std::cout << "!!!! ERROR !!!! Number of Function parameters are different. " << exp_f_p_index << " " << act_f_p_index <<std::endl;
				return false;
			}
			for(int i=0;i<act_f_p_index;i++){
				if(expected_func_param_type[i] != actual_func_param_type[i]){
					std::cout << "!!!! ERROR !!!! Function parameter type mismatch. " << i << " " << expected_func_param_type[i] << " " << actual_func_param_type[i] <<std::endl;
					return false;
				}
			}
				
			return true;
	}
			
	
	bool performSemanticCheck_Loops(char** p,int len)
	{

		std::string to[10];
		int num_line=0;
		int lhs_actual_type = Parser::idType::ID_NONE;
		int rhs_actual_type = Parser::idType::ID_NONE;
		for(int i = 0; i < len; i++){
			if(*p[i] != ';'){
					to[num_line].append(p[i]);
					to[num_line].append(" ");
					}
				else{
					to[num_line].append(p[i]);
					num_line=num_line+1;
					}
		}
		for(int i = 0; i < num_line; i++){
			int pos = 0;
			std::string var;
			std::string op;
			int tokenNum;
			int rhs_count = 0;
			std::string p = to[i];
			int gotIf=0;
			int gotFor=0;
			int gotWhile = 0;
			std::string lhs;
			std::string rhs[10];
			//move till we get binary operator
			while(p[pos] != ';'){
				
				std::string var;
				while(p[pos] != ' '){
					var += to[i][pos];
					pos++;
				}
				if(var.compare(0,2,"if") == 0){
					gotIf = 1;
					break;
				}
				if(var.compare(0,5,"while") == 0){
					gotWhile=1;
					break;
				}
				if(var.compare(0,3,"for") == 0){
					gotFor =1;
					break;
				}
				pos++;
			}
			pos++;
			if((gotIf == 1)||(gotWhile == 1)){
				//get lhs
				while(p[pos] != ';'){
					var = "";
					while(to[i][pos] != ' '){
						var += to[i][pos];
						pos++;
					} //end of while
					tokenNum = get_token_enum(var);
					if(tokenNum == Symbol::Terminal::ID){
						//std::cout << "lhs got identifier " << var << std::endl;
						lhs = var;
						break;
					}
					pos++;
				}
				//get operator
				while(p[pos] != ';'){
					var = "";
					while(to[i][pos] != ' '){
						var += to[i][pos];
						pos++;
					} //end of while
					tokenNum = get_token_enum(var);
					if((Symbol::Nonterminal::COMPARE_OP)||
						(Symbol::Nonterminal::OR_OP)||
						(Symbol::Nonterminal::AND_OP)){
						//std::cout << "op  " << tokenNum << std::endl;
						op = var;
						break;
					}
					pos++;
				}
				while(p[pos] != ';'){
					var="";
					tokenNum = 0;
					while(to[i][pos] != ' '){
						var += to[i][pos];
						pos++;
					} //end of while
					if(strcmp("then",var.c_str())==0){
						break;
					}
					tokenNum = get_token_enum(var);
					//std::cout << "token num " << tokenNum << std::endl;
					if(tokenNum == Symbol::Terminal::ID){
						//std::cout << "rhs got identifier " << var  <<std::endl;
						rhs[rhs_count] = var;
						rhs_count++;
					}
					if(tokenNum == Symbol::Terminal::FLOATLIT){
						//std::cout << "rhs got floatlit " << var  <<std::endl;
						rhs[rhs_count] = "intlit";
						rhs_count++;
					}
					if(tokenNum == Symbol::Terminal::INTLIT){
						//std::cout << "rhs got intlit " << var  <<std::endl;
						rhs[rhs_count] = "floatit";
						rhs_count++;
					}
					pos++;
				}
				lhs_actual_type = getLHSType(lhs);
				rhs_actual_type = getRHSType(rhs,rhs_count);
				
			}
		}
		if(lhs_actual_type == rhs_actual_type){
				return true;
		}
		else{
				return false;
		}
	}
	
	bool performSemanticCheck_BinaryOperands(char** p,int len)
	{
		std::string to[100];
		int num_line=0;
		std::string lhs;
		std::string rhs[10];
		int lhs_actual_type = Parser::idType::ID_NONE;
		int rhs_actual_type = Parser::idType::ID_NONE;
		int rhs_expected_type = Parser::idType::ID_NONE;
		
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
		for(int i = 0; i < num_line; i++){
			int pos = 0;
			std::string var;
			int tokenNum;
			int rhs_count = 0;
			std::string p = to[i];

			//move till we get binary operator
			while(p[pos] != ':'){
				var="";
				tokenNum = 0;
				while(to[i][pos] != ' '){
					var += to[i][pos];
					pos++;
				} //end of while
				tokenNum = get_token_enum(var);
				if(tokenNum == Symbol::Terminal::ID){
					//std::cout << "lhs got identifier " << var << std::endl;
					lhs = var;
				}
				pos++;
			}
			
			while(p[pos] != ';'){
				var="";
				tokenNum = 0;
				while(to[i][pos] != ' '){
					var += to[i][pos];
					pos++;
				} //end of while
				
				tokenNum = get_token_enum(var);
				//std::cout << "token num " << tokenNum << std::endl;
				if(tokenNum == Symbol::Terminal::ID){
					//std::cout << "rhs got identifier " << var  <<std::endl;
					rhs[rhs_count] = var;
					rhs_count++;
				}
				if(tokenNum == Symbol::Terminal::FLOATLIT){
					//std::cout << "rhs got floatlit " << var  <<std::endl;
					rhs[rhs_count] = "intlit";
					rhs_count++;
				}
				if(tokenNum == Symbol::Terminal::INTLIT){
					//std::cout << "rhs got intlit " << var  <<std::endl;
					rhs[rhs_count] = "floatit";
					rhs_count++;
				}
				pos++;
			}
			
			// get the type of lhs variable from symbol table
			lhs_actual_type = getLHSType(lhs);
			
			//rhs expected type is same as lhs actual type
			rhs_expected_type = lhs_actual_type;

			//get actual rhs type
			rhs_actual_type = getRHSType(rhs,rhs_count);
			
		}
		if(rhs_actual_type == rhs_expected_type){
				return true;
		}
		else{
				return false;
		}
		
	}
	int getLHSType(std::string lhs)
	{
		int lhs_actual_type = Parser::idType::ID_NONE;
		
		// get the type of lhs variable from symbol table
		typedef std::map<int,SymbolTable>::const_iterator MapIterator;
		int getTypeSucess = 0;
		std::string var_type= "variable";
		
		for (MapIterator iter = globalSymbolTable.begin(); iter != globalSymbolTable.end(); iter++){
	
			while(getTypeSucess == 0){
				Record rec(0);
				
				globalSymbolTable[iter->first].getRecord(var_type,lhs,rec);
				if(rec.name != ""){
					for (int k = 0; k < rec.parameterTypes.size(); k++) {
            		  
					  if(strcmp("int",rec.parameterTypes[k].c_str())==0){
					  	lhs_actual_type = Parser::idType::INT;
						getTypeSucess = 1;
					  }
					  else if(strcmp("float",rec.parameterTypes[k].c_str())==0){
					  	lhs_actual_type = Parser::idType::FLOAT;
						getTypeSucess = 1;
					  }

					  lhs = rec.parameterTypes[k];
					  var_type = "type";
					  //std::cout << "got lhs type " << lhs << " "<<rec.parameterTypes[k]  <<std::endl;
        		  	}	
				}
				else{
					getTypeSucess = 1;
				}
			}
		}
		return lhs_actual_type;
	}
	
	int getRHSType(std::string rhs[10], int rhs_count)
	{
		int rhs_actual_type = Parser::idType::ID_NONE;
		
		//now get rhs actual  type
		for (int j = 0;j<rhs_count;j++){
			std::string var_type= "variable";
			//std::cout << "got rhs " << rhs[j] <<std::endl;
			if(strcmp("intlit",rhs[j].c_str())==0){
				rhs_actual_type = Parser::idType::INT;
			}
			if(strcmp("floatlit",rhs[j].c_str())==0){
				rhs_actual_type = Parser::idType::FLOAT;
			}
			typedef std::map<int,SymbolTable>::const_iterator MapIterator;
			int getTypeSucess = 0;
			for (MapIterator iter = globalSymbolTable.begin(); iter != globalSymbolTable.end(); iter++){
				
				while(getTypeSucess == 0){
					
					Record rec(0);
					globalSymbolTable[iter->first].getRecord(var_type,rhs[j],rec);
					if(rec.name != ""){
						
							for (int k = 0; k < rec.parameterTypes.size(); k++) {
							  int new_type = Parser::idType::ID_NONE;
		            		  //std::cout << "got record type " << rec.parameterTypes[k] <<std::endl;

							  if(strcmp("int",rec.parameterTypes[k].c_str())==0)
						  		new_type = Parser::idType::INT;
							  else if(strcmp("float",rec.parameterTypes[k].c_str())==0)
							  	new_type = Parser::idType::FLOAT;
						  
							  if(rhs_actual_type == Parser::idType::ID_NONE){
								  rhs_actual_type = new_type;
							  }
							  if((rhs_actual_type == Parser::idType::INT)
							  					&&(new_type == Parser::idType::INT)){
								  	rhs_actual_type = Parser::idType::INT;
									getTypeSucess = 1;
							  }
							  if((rhs_actual_type == Parser::idType::FLOAT)
							  					||(new_type == Parser::idType::FLOAT)){
								  	rhs_actual_type = Parser::idType::FLOAT;
									getTypeSucess = 1;
							  }
							  rhs[j] = rec.parameterTypes[k];
							  var_type = "type";
							  //std::cout << "got rhs type " << rec.parameterTypes[k] << rhs_actual_type <<std::endl;
		        		  	}
							
						}
						else{
							getTypeSucess = 1;
						}
					}
				}
			}
		return rhs_actual_type;
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
				to[num_line].append(p[i]);
				num_line=num_line+1;
				}
		  }
		  
		  for(int i = 0; i <= num_line; i++){
		  	
  		  		  std::cout << to[i] <<std::endl;
				  std::string p = to[i];
				  std::size_t line_pos=0; 
				  while(p[line_pos] != ';'){

					  //create symbol table
					  std::size_t pos0 = to[i].find("let",line_pos,3);
					  if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
						symTable[scopeCount] = SymbolTable(scopeCount);
						
				      }
					  
				  	  //find and create type record
					  pos0 = to[i].find("type",line_pos,4);     
				      if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){

							pos0 = pos0+5;
							std::string type_var;
							while(to[i][pos0] != ' '){
								type_var += to[i][pos0];
								pos0++;
							} //end of while

							//std::cout << " type var is " << type_var <<std::endl;
							pos0 = pos0+3;
							std::string type;
							while(to[i][pos0] != ' '){
								type += to[i][pos0];
								pos0++;
							} //end of while
							std::string type_type[10];
							type_type[0] = type;
							//std::cout << " type type is " << type_type[0] <<std::endl;
							if(type_type[0].compare(0,5,"array") == 0){
								
								pos0 = pos0+3;
								std::string dim[10];
								int dim_index = 0;
								while(to[i][pos0] != ' '){
									dim[dim_index] += to[i][pos0];
									pos0++;
									dim_index++;
								} //end of while
								
								pos0 = pos0+6;
								std::string array_type;
								while(to[i][pos0] != ' '){
									array_type += to[i][pos0];
									pos0++;
								} //end of while
								//std::cout << "here array type is " << array_type ;
								if(isPresentInSymbolTable("type",type_var) == false)
									symTable[scopeCount].insert(SymbolTable::symbolType::TYPE,type_var,type_type,dim,array_type);
							}
							else{
								std::string dim[10];
								if(isPresentInSymbolTable("type",type_var) == false)
									symTable[scopeCount].insert(SymbolTable::symbolType::TYPE,type_var,type_type,dim,"");
							}
							
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
										//std::cout << "var name is " << var_name[var_index] <<std::endl;
										var_index++;
										
									}//end of not , and space
								else{
									pos0++;
									}
								
							}//end of :

						//find var type
						pos0 = pos0+2;
						std::string var_type[10];
						std::string temp[10];
						std::string var;
						while(to[i][pos0] != ' '){
							var += to[i][pos0];
							pos0++;
						} //end of while
						var_type[0] = var;
						//std::cout << "var type is " << var_type[0] <<std::endl;
						
						for(int k=0;k<var_index;k++){
							if(isPresentInSymbolTable("variable",var_name[k]) == false)
								symTable[scopeCount].insert(SymbolTable::symbolType::VARIABLE,var_name[k],var_type,temp,"");
						}
					
				      }

					  //find and create function record
					  pos0 = to[i].find("function",line_pos,8); 
				      if((pos0 < strlen(to[i].c_str()))&&(pos0==line_pos)){
						pos0 = pos0+9;
						int j = 0;
						std::string function_name;
						while(to[i][pos0] != ' '){
								function_name += to[i][pos0];
								pos0++;
								j++;
							}
						//search for param name and type
						std::string param_name[10];
						std::string param_type[10];
						int param_name_index = 0;
						int param_type_index = 0;
						while(to[i][pos0] != ')'){
						
							std::string var;
							while(to[i][pos0] != ' '){
									var += to[i][pos0];
									pos0++;
									j++;
								}
							int tokenNum = get_token_enum(var);
		  					if(tokenNum == Symbol::Terminal::ID){
								param_name[param_name_index] = var;
								param_name_index++;
		  					}
							if((strcmp("int",var.c_str())==0)||(strcmp("float",var.c_str())==0)){
									param_type[param_type_index] = var;
									param_type_index++;
							}
							pos0++;;
						}
						pos0 = pos0 + 2;
						std::string return_type;
						while(to[i][pos0] != ' '){
								return_type += to[i][pos0];
								pos0++;
								j++;
							}
						if((strcmp("int",return_type.c_str())==0)||(strcmp("float",return_type.c_str())==0)){
							//valid return type
						}
						else{
							return_type = "";
						}
						//std::cout << "function: " <<function_name << param_name[0] <<param_type[0] <<return_type <<std::endl;
						if(isPresentInSymbolTable("function",function_name) == false)
							symTable[scopeCount].insert(SymbolTable::symbolType::FUNCTION,function_name,param_name, param_type,return_type);
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

		
		//std::cout << "input string is " << inString <<std::endl;
		
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
