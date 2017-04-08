#include "Generator.hpp"
#include <sstream>
#include <cmath>

void GenNaive::data_seg() {
  asm_.push_back("# Beginning of the data section");
  asm_.push_back(".data");
}

void GenNaive::assign_asm(std::vector<std::string> tokens) {
  bool is_float = false;
  if (tokens.size() == 4) { // array
    if (tokens[3].find(".") != std::string::npos) {
      is_float = true;
    }
    size_t size = atoi(tokens[2].c_str());
    if (is_float) {
      std::string decimal = std::to_string(atof(tokens[3].c_str()) - atoi(tokens[3].c_str()));
      decimal = decimal.substr(2);
      asm_.push_back("la $t0, num_" + std::to_string(atoi(tokens[3].c_str())) + "_" + decimal);
      asm_.push_back("lwc1 $f1, 0($t0)");
      asm_.push_back("la $t0, " + tokens[1]);
      for (size_t i = 0; i < size; ++i) {
        asm_.push_back("swc1 $f1, " + std::to_string(4 * (i + 1)) + "($t0)");
      }
    } else {
      asm_.push_back("la $t0, num_" + std::to_string(atoi(tokens[3].c_str())));
      asm_.push_back("lw $t1, 0($t0)");
      asm_.push_back("la $t0, " + tokens[1]);
      for (size_t i = 0; i < size; ++i) {
        asm_.push_back("lw $t1, " + std::to_string(4 * (i + 1)) + "($t0)");
      }
    }
  } else if (tokens.size() == 3) {
    if (tokens[2].find(".") != std::string::npos) {
      is_float = true;
    }
    if (is_float) {
      std::string decimal = std::to_string(atof(tokens[2].c_str()) - atoi(tokens[2].c_str()));
      decimal = decimal.substr(2);
      asm_.push_back("la $t0, num_" + std::to_string(atoi(tokens[2].c_str())) + "_" + decimal);
      asm_.push_back("lwc1 $f1, 0($t0)");
      asm_.push_back("la $t0, " + tokens[1]);
      asm_.push_back("swc1 $f1, 0($t0)");
    } else {
      asm_.push_back("la $t0, num_" + std::to_string(atoi(tokens[2].c_str())));
      asm_.push_back("lw $t1, 0($t0)");
      asm_.push_back("la $t0, " + tokens[1]);
      asm_.push_back("sw $t1, 0($t0)");
    }
  }
}

void GenNaive::text_seg() {
  asm_.push_back("# Beginning of the code section");
  asm_.push_back(".text");

  // built in funcs
  for (auto& func : func_map_) {
    for (auto& line : func.second) {
      asm_.push_back(line);
    }
    asm_.push_back("\n");
  }

  // main entrance
  asm_.push_back("main:\n");
  
  for (auto& line : ir_) {
    std::istringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;
    while(std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    asm_.push_back("# IR: " + line);
    if (tokens[0] == "assign") {
      assign_asm(tokens);
    } else if (tokens[0] == "add" || tokens[0] == "sub"
            || tokens[0] == "and" || tokens[0] == "or") {

    } else if (tokens[0] == "mult" || tokens[0] == "div") {

    } else if (tokens[0] == "goto") {

    } else if (tokens[0] == "breq" || tokens[0] == "brneq"
            || tokens[0] == "brlt" || tokens[0] == "brgt"
            || tokens[0] == "brgeq" || tokens[0] == "brleq") {

    } else if (tokens[0] == "return") {

    } else if (tokens[0] == "call" || tokens[0] == "callr") {

    } else if (tokens[0] == "array_store") {

    } else if (tokens[0] == "array_load") {

    }
  }
}

void GenNaive::generate() {}

