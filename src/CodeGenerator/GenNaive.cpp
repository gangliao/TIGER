#include "Generator.hpp"
#include <sstream>
#include <cmath>

bool is_num(std::string s) {  
  std::stringstream sin(s);  
  double t;
  char p;
  if(!(sin >> t))
    return false;
  if(sin >> p)
    return false;
  else
    return true;
}

void GenNaive::data_seg() {
  asm_.push_back("# Beginning of the data section");
  asm_.push_back(".data");
  for (auto& line : ir_) {
    std::istringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;
    while(std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    if (tokens[0] == "assign") {
      if (data_map_.find(tokens[1]) == data_map_.end()) {
        if (tokens.size() == 4) {
          asm_.push_back(tokens[1] + ": .space " + std::to_string(4 * atoi(tokens[2].c_str())));
          data_map_[tokens[1]] = std::make_pair(tokens[1],
            (tokens[3].find(".") != std::string::npos ? FLOAT : INT));
        } else if (tokens.size() == 3) {
          asm_.push_back(tokens[1] + ": " + (tokens[2].find(".") != std::string::npos ? ".float" : ".word"));
          data_map_[tokens[1]] = std::make_pair(tokens[1],
            (tokens[2].find(".") != std::string::npos ? FLOAT : INT));
        }
        
      }
    }
    for (auto& token : tokens) {
      if (is_num(token) && data_map_.find(token) == data_map_.end()) {
        if (token.find(".") != std::string::npos) {
          std::string decimal = std::to_string(atof(token.c_str()) - atoi(token.c_str()));
          decimal = decimal.substr(2);
          asm_.push_back("num_" + std::to_string(atoi(token.c_str())) + "_" + decimal + ": .float");
          std::string asm_code = "num_" + std::to_string(atoi(token.c_str())) + "_" + decimal;
          data_map_[token] = std::make_pair(asm_code, FLOAT);
        } else {
          asm_.push_back("num_" + std::to_string(atoi(token.c_str())) + ": .word");
          std::string asm_code = "num_" + std::to_string(atoi(token.c_str()));
          data_map_[token] = std::make_pair(asm_code, INT);
        }
      }
    }
  }
}

void GenNaive::assign_asm(std::vector<std::string> tokens) {
  bool is_float = false;
  if (tokens.size() == 4) { // array
    if (tokens[3].find(".") != std::string::npos) {
      is_float = true;
    }
    size_t size = atoi(tokens[2].c_str());
    if (is_float) {
      asm_.push_back("la $t0, " + data_map_[tokens[3]].first);
      asm_.push_back("lwc1 $f1, 0($t0)");
      asm_.push_back("la $t0, " + tokens[1]);
      for (size_t i = 0; i < size; ++i) {
        asm_.push_back("swc1 $f1, " + std::to_string(4 * (i + 1)) + "($t0)");
      }
    } else {
      asm_.push_back("la $t0, " + data_map_[tokens[3]].first);
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
      asm_.push_back("la $t0, " + data_map_[tokens[2]].first);
      asm_.push_back("lwc1 $f1, 0($t0)");
      asm_.push_back("la $t0, " + tokens[1]);
      asm_.push_back("swc1 $f1, 0($t0)");
    } else {
      asm_.push_back("la $t0, " + data_map_[tokens[2]].first);
      asm_.push_back("lw $t1, 0($t0)");
      asm_.push_back("la $t0, " + tokens[1]);
      asm_.push_back("sw $t1, 0($t0)");
    }
  }
}

void GenNaive::operator_asm(std::vector<std::string> tokens) {

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
      operator_asm(tokens);
    } else if (tokens[0] == "mult" || tokens[0] == "div") {

    } else if (tokens[0] == "goto") {
      asm_.push_back("j " + tokens[1]);
    } else if (tokens[0] == "breq" || tokens[0] == "brneq"
            || tokens[0] == "brlt" || tokens[0] == "brgt"
            || tokens[0] == "brgeq" || tokens[0] == "brleq") {

    } else if (tokens[0] == "return") {
      asm_.push_back("# IR: " + line);
      if (tokens.size() == 2) {
        asm_.push_back("la $t0, " + data_map_[tokens[1]].first);
        asm_.push_back("lw $t1, 0($t0)");
        asm_.push_back("move $v0, $t1");
      }
      asm_.push_back("addi $sp, $sp, 4");
      asm_.push_back("lw $ra, -4($sp)");
      asm_.push_back("addi $sp, $sp, 32");
      asm_.push_back("lw $s0, -4($sp)");
      asm_.push_back("lw $s1, -8($sp)");
      asm_.push_back("lw $s2, -12($sp)");
      asm_.push_back("lw $s3, -16($sp)");
      asm_.push_back("lw $s4, -20($sp)");
      asm_.push_back("lw $s5, -24($sp)");
      asm_.push_back("lw $s6, -28($sp)");
      asm_.push_back("lw $s7, -32($sp)");
      asm_.push_back("jr $ra");
    } else if (tokens[0] == "call" || tokens[0] == "callr") {
      asm_.push_back("sw $t4, -4($sp)");
      asm_.push_back("sw $t5, -8($sp)");
      asm_.push_back("sw $t6, -12($sp)");
      asm_.push_back("sw $t7, -16($sp)");
      asm_.push_back("sw $t8, -20($sp)");
      asm_.push_back("sw $t9, -24($sp)");
      asm_.push_back("addi $sp, $sp, -24");
      asm_.push_back("swc1 $f12, -4($sp)");
      asm_.push_back("swc1 $f13, -8($sp)");
      asm_.push_back("swc1 $f14, -12($sp)");
      asm_.push_back("addi $sp, $sp, -12");
      asm_.push_back("sw $a0, -4($sp)");
      asm_.push_back("sw $a1, -8($sp)");
      asm_.push_back("sw $a2, -12($sp)");
      asm_.push_back("sw $a3, -16($sp)");
      asm_.push_back("addi $sp, $sp, -16");
      size_t param_size = 0;
      size_t param_idx = 0;
      if (tokens[0] == "callr") {
        param_size = tokens.size() - 3;
        param_idx = 3;
      } else {
        param_size = tokens.size() - 2;
        param_idx = 2;
      }
      for (size_t i = 0; i < param_size; ++i) {
        asm_.push_back("la $t9, " + data_map_[tokens[param_idx + i]].first);
        if (data_map_[tokens[param_idx]].second == INT) {
          asm_.push_back("lw $a" + std::to_string(i) + ", 0($t9)");
        } else {
          asm_.push_back("lwc1 $a" + std::to_string(i) + ", 0($t9)");
        }
      }
      asm_.push_back("jal " + (tokens[0] == "callr" ? tokens[2] : tokens[1]));
      asm_.push_back("addi $sp, $sp, 16");
      asm_.push_back("lw $a0, -4($sp)");
      asm_.push_back("lw $a1, -8($sp)");
      asm_.push_back("lw $a2, -12($sp)");
      asm_.push_back("lw $a3, -16($sp)");
      asm_.push_back("addi $sp, $sp, 12");
      asm_.push_back("lwc1 $f12, -4($sp)");
      asm_.push_back("lwc1 $f13, -8($sp)");
      asm_.push_back("lwc1 $f14, -12($sp)");
      asm_.push_back("addi $sp, $sp, 24");
      asm_.push_back("lw $t4, -4($sp)");
      asm_.push_back("lw $t5, -8($sp)");
      asm_.push_back("lw $t6, -12($sp)");
      asm_.push_back("lw $t7, -16($sp)");
      asm_.push_back("lw $t8, -20($sp)");
      asm_.push_back("lw $t9, -24($sp)");
      if (tokens[0] == "callr") {
        asm_.push_back("la $t0, " + data_map_[tokens[1]].first);
        if (data_map_[tokens[1]].second == INT) {
          asm_.push_back("sw $v0, 0($t0)");
        } else {
          asm_.push_back("swc1 $v0, 0($t0)");
        }
      }
    } else if (tokens[0] == "array_store") {

    } else if (tokens[0] == "array_load") {

    }
  }
}

void GenNaive::generate() {}

