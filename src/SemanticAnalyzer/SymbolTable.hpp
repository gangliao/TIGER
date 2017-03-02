#pragma once

#include "Level.hpp"
#include "Record.hpp"
#include <list>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Implement the scoping and symbol table details in this file
class SymbolTable {
 private:
  Level topLevel;
  Level currLevel;

  int scopeLevel;

  std::list<Record> recordList;
  
 public:
 	enum symbolType
	{
		INT=0,
		FLOAT=1,
		TYPE=2,
		VARIABLE=3,
		FUNCTION=4
	};
  SymbolTable() {


  	/*
    Record intRecord(scopeLevel);
    Record floatRecord(scopeLevel);
	Record functionRecord(scopeLevel);
    intRecord.type = "int";
    floatRecord.type = "float";
	functionRecord.type = "function";
	*/
    //.... Fill in the other built-in constructs
    // also add any  data structure required
  }

  SymbolTable(int level) {
	this->scopeLevel = level;
  }
  void insert(int type, std::string str1, std::string str2,std::string str3,std::string str4){

	if(type==SymbolTable::symbolType::INT){
		  Record intRecord(scopeLevel);
		  intRecord.type = "int";
		  this->recordList.push_back(intRecord);
		}
	if(type == SymbolTable::symbolType::FLOAT){
		  Record floatRecord(scopeLevel);
		  floatRecord.type = "float";
		  this->recordList.push_back(floatRecord);
		}

	if(type == SymbolTable::symbolType::TYPE){
		  Record typeRecord(scopeLevel);
		  typeRecord.type = "type";
		  typeRecord.name=str1;
		  
		  if(str2.compare(0,5,"array") == 0){
		  	int dimension = stoi(str3);
			std::cout << "got dimension " << dimension << str4;
		  	typeRecord.parameterDimensions.push_back(dimension);
			typeRecord.parameterTypes.push_back(str4);
		  	}
		  else{
		  	typeRecord.parameterTypes.push_back(str2);
		  	}
		  this->recordList.push_back(typeRecord);
		  std::cout << "inserted type record " <<std::endl;
		}
	if(type == SymbolTable::symbolType::FUNCTION){
		  Record functionRecord(scopeLevel);
		  functionRecord.type = "function";
		  functionRecord.name=str1;
		  functionRecord.parameters.push_back(str2);
		  functionRecord.parameterTypes.push_back(str3);
		  this->recordList.push_back(functionRecord);
		  std::cout << "inserted function record " <<std::endl;
		}
	if(type == SymbolTable::symbolType::VARIABLE){
		  Record variableRecord(scopeLevel);
		  variableRecord.type = "variable";
		  variableRecord.name=str1;
		  variableRecord.parameterTypes.push_back(str2);
		  this->recordList.push_back(variableRecord);
		  std::cout << "inserted variable record " <<std::endl;
		}
	}

  bool containsKey(std::string type,std::string name)
  {
	for (std::list<Record>::iterator it=recordList.begin(); it != recordList.end(); ++it)
	{
	  	if(it->type.compare(0,strlen(type.c_str()),type)==0){
			  if(it->name.compare(0,strlen(name.c_str()),name)==0){
			  	return true;
			  	}
		}	
	}
	return false;
	  	
  }
  void print_symTable()
  {
	std::cout << "---------------------------------" <<std::endl;
	for (std::list<Record>::iterator it=recordList.begin(); it != recordList.end(); ++it){
		
    		std::cout << "Type: " << it->type <<std::endl;
			//print param names
			std::cout << "Name: "<< it->name << " ";
			std::cout << "Params: "<< " ";
			for (int i = 0; i < it->parameters.size(); i++) {
            std::cout << it->parameters[i] << " ";
        	}
			std::cout << std::endl;
			//print param types
			std::cout << "Param Type: ";
			for (int i = 0; i < it->parameterTypes.size(); i++) {
            std::cout << it->parameterTypes[i] << " ";
        	}
			std::cout << "Param Dimension: ";
			for (int i = 0; i < it->parameterDimensions.size(); i++) {
            std::cout << it->parameterDimensions[i] << " ";
        	}
			std::cout << std::endl;
			std::cout << "Scope: " << this->scopeLevel <<std::endl;
			std::cout << "---------------------------------" <<std::endl;
		}
  	//std::cout << "Type: " << this->recordList.type <<std::endl;
  }
};
