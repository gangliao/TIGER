#include <cmath>
#include <set>
#include <sstream>
#include "Generator.hpp"

bool is_num(std::string s) {
  std::stringstream sin(s);
  double t;
  char p;
  if (!(sin >> t)) return false;
  if (sin >> p)
    return false;
  else
    return true;
}

void GenNaive::data_seg() {
  asm_.push_back("# Beginning of the data section\n");
  asm_.push_back(".data");
  std::vector<std::string> data_float;
  std::vector<std::string> data_word;
  std::set<std::string> labels;
  for (auto& line : ir_) {
    if (line.find(":") != std::string::npos) {
      labels.insert(line.substr(0, line.size() - 1));
    }
  }
  for (auto& line : ir_) {
    std::istringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(ss, token, ',')) {
      token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }
    if (tokens[0] == "assign") {
      if (data_map_.find(tokens[1]) == data_map_.end()) {
        if (tokens.size() == 4) {
          data_float.push_back(tokens[1] + ": \t\t.space \t" +
                               std::to_string(4 * atoi(tokens[2].c_str())));
          data_map_[tokens[1]] = std::make_pair(
              tokens[1],
              (tokens[3].find(".") != std::string::npos ? FLOAT : INT));
        } else if (tokens.size() == 3) {
          if (tokens[2].find(".") != std::string::npos) {
            data_float.push_back(tokens[1] + ": \t\t.float \t" + tokens[2]);
            data_map_[tokens[1]] = std::make_pair(tokens[1], FLOAT);
          } else {
            data_word.push_back(tokens[1] + ": \t\t.word \t" + tokens[2]);
            data_map_[tokens[1]] = std::make_pair(tokens[1], INT);
          }
        }
      }
    }
    size_t i = 0;
    for (auto& token : tokens) {
      if (i++ == 0) continue;
      if (tokens[0] == "call" && i == 2) continue;
      if (tokens[0] == "callr" && i == 3) continue;
      if (token.find(":") != std::string::npos) continue;
      if (labels.find(token) != labels.end()) continue;
      if (data_map_.find(token) != data_map_.end()) continue;
      if (is_num(token)) {
        if (token.find(".") != std::string::npos) {
          int pos = token.find(".");
          std::string digit = token.substr(0, pos);
          std::string decimal = token.substr(pos + 1, token.size());
          data_float.push_back("num_" + digit + "_" + decimal +
                               ": \t.float \t" + token);
          std::string asm_code = "num_" + digit + "_" + decimal;
          data_map_[token] = std::make_pair(asm_code, FLOAT);
        } else {
          data_word.push_back("num_" + std::to_string(atoi(token.c_str())) +
                              ": \t\t.word \t" + token);
          std::string asm_code = "num_" + std::to_string(atoi(token.c_str()));
          data_map_[token] = std::make_pair(asm_code, INT);
        }
      } else {
        if (token.find("f") != std::string::npos) {
          data_float.push_back(token + ": \t\t.float \t0.0");
          data_map_[token] = std::make_pair(token, FLOAT);
        } else {
          data_word.push_back(token + ": \t\t.word \t0");
          data_map_[token] = std::make_pair(token, INT);
        }
      }
    }
  }
  asm_.reserve(data_word.size() + data_float.size());
  for (auto& data : data_float) {
    asm_.push_back(data);
  }
  for (auto& data : data_word) {
    asm_.push_back(data);
  }
}

void GenNaive::assign_asm(std::vector<std::string>& tokens) {
  if (tokens.size() == 4) {  // array
    size_t size = atoi(tokens[2].c_str());
    if (data_map_[tokens[3]].second == FLOAT) {
      asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
      asm_.push_back("    lwc1 $f1, 0($t0)");
      asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
      for (size_t i = 0; i < size; ++i) {
        asm_.push_back("    swc1 $f1, " + std::to_string(4 * (i + 1)) +
                       "($t0)");
      }
    } else {
      asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
      asm_.push_back("    lw $t1, 0($t0)");
      asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
      for (size_t i = 0; i < size; ++i) {
        asm_.push_back("    lw $t1, " + std::to_string(4 * (i + 1)) + "($t0)");
      }
    }
  } else if (tokens.size() == 3) {
    if (data_map_[tokens[2]].second == FLOAT) {
      asm_.push_back("    la $t0, " + data_map_[tokens[2]].first);
      asm_.push_back("    lwc1 $f1, 0($t0)");
      asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
      asm_.push_back("    swc1 $f1, 0($t0)");
    } else {
      asm_.push_back("    la $t0, " + data_map_[tokens[2]].first);
      asm_.push_back("    lw $t1, 0($t0)");
      asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
      asm_.push_back("    sw $t1, 0($t0)");
    }
  }
}

void GenNaive::operator_asm(std::vector<std::string>& tokens) {
  if (data_map_[tokens[3]].second == INT) {
    asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
    asm_.push_back("    lw $t1, 0($t0)");
    asm_.push_back("    la $t0, " + data_map_[tokens[2]].first);
    asm_.push_back("    lw $t2, 0($t0)");
    if (tokens[0] != "mult" && tokens[0] != "div") {
      asm_.push_back("    " + tokens[0] + " $t1, $t1, $t2");
      asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
      asm_.push_back("    sw $t1, 0($t0)");
    } else {
      asm_.push_back("    " + tokens[0] + " $t1, $t2");
      asm_.push_back("    mflo $t1");
      asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
      asm_.push_back("    sw $t1, 0($t0)");
    }
  } else {
    asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
    asm_.push_back("    lwc1 $f1, 0($t0)");
    asm_.push_back("    la $t0, " + data_map_[tokens[2]].first);
    asm_.push_back("    lwc1 $f2, 0($t0)");
    if (tokens[0] != "mult" && tokens[0] != "div") {
      asm_.push_back("    " + tokens[0] + ".s $f1, $f1, $f2");
      asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
      asm_.push_back("    swc1 $f1, 0($t0)");
    } else {
      asm_.push_back("    " + tokens[0] + " $f1, $f2");
      asm_.push_back("    mflo $f1");
      asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
      asm_.push_back("    swc1 $f1, 0($t0)");
    }
  }
}

void GenNaive::return_asm(std::vector<std::string>& tokens) {
  if (tokens.size() == 2) {
    asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
    asm_.push_back("    lw $t1, 0($t0)");
    asm_.push_back("    move $v0, $t1");
  }
  asm_.push_back("    addi $sp, $sp, 4");
  asm_.push_back("    lw $ra, -4($sp)");
  asm_.push_back("    addi $sp, $sp, 32");
  asm_.push_back("    lw $s0, -4($sp)");
  asm_.push_back("    lw $s1, -8($sp)");
  asm_.push_back("    lw $s2, -12($sp)");
  asm_.push_back("    lw $s3, -16($sp)");
  asm_.push_back("    lw $s4, -20($sp)");
  asm_.push_back("    lw $s5, -24($sp)");
  asm_.push_back("    lw $s6, -28($sp)");
  asm_.push_back("    lw $s7, -32($sp)");
  asm_.push_back("    jr $ra");
}

void GenNaive::call_asm(std::vector<std::string>& tokens) {
  asm_.push_back("    sw $t4, -4($sp)");
  asm_.push_back("    sw $t5, -8($sp)");
  asm_.push_back("    sw $t6, -12($sp)");
  asm_.push_back("    sw $t7, -16($sp)");
  asm_.push_back("    sw $t8, -20($sp)");
  asm_.push_back("    sw $t9, -24($sp)");
  asm_.push_back("    addi $sp, $sp, -24");
  asm_.push_back("    swc1 $f12, -4($sp)");
  asm_.push_back("    swc1 $f13, -8($sp)");
  asm_.push_back("    swc1 $f14, -12($sp)");
  asm_.push_back("    addi $sp, $sp, -12");
  asm_.push_back("    sw $a0, -4($sp)");
  asm_.push_back("    sw $a1, -8($sp)");
  asm_.push_back("    sw $a2, -12($sp)");
  asm_.push_back("    sw $a3, -16($sp)");
  asm_.push_back("    addi $sp, $sp, -16");
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
    asm_.push_back("    la $t0, " + data_map_[tokens[param_idx + i]].first);
    if (data_map_[tokens[param_idx]].second == INT) {
      asm_.push_back("    lw $s" + std::to_string(i) + ", 0($t0)");
    } else {
      asm_.push_back("    lwc1 $f0, 0($t0)");
      asm_.push_back("    mov.s $a" + std::to_string(i) + ", $f0");
    }
  }
  asm_.push_back("    jal " + (tokens[0] == "callr" ? tokens[2] : tokens[1]));
  asm_.push_back("    addi $sp, $sp, 16");
  asm_.push_back("    lw $a0, -4($sp)");
  asm_.push_back("    lw $a1, -8($sp)");
  asm_.push_back("    lw $a2, -12($sp)");
  asm_.push_back("    lw $a3, -16($sp)");
  asm_.push_back("    addi $sp, $sp, 12");
  asm_.push_back("    lwc1 $f12, -4($sp)");
  asm_.push_back("    lwc1 $f13, -8($sp)");
  asm_.push_back("    lwc1 $f14, -12($sp)");
  asm_.push_back("    addi $sp, $sp, 24");
  asm_.push_back("    lw $t4, -4($sp)");
  asm_.push_back("    lw $t5, -8($sp)");
  asm_.push_back("    lw $t6, -12($sp)");
  asm_.push_back("    lw $t7, -16($sp)");
  asm_.push_back("    lw $t8, -20($sp)");
  asm_.push_back("    lw $t9, -24($sp)");
  if (tokens[0] == "callr") {
    asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
    if (data_map_[tokens[1]].second == INT) {
      asm_.push_back("    sw $v0, 0($t0)");
    } else {
      asm_.push_back("    swc1 $v0, 0($t0)");
    }
  }
}

void GenNaive::array_load_asm(std::vector<std::string>& tokens) {
  asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
  asm_.push_back("    lw $t1, 0($t0)");
  asm_.push_back("    sll $t1, $t1, 2");
  asm_.push_back("    la $t0, " + data_map_[tokens[2]].first);
  asm_.push_back("    add $t0, $t0, $t1");
  asm_.push_back("    srl $t1, $t1, 2");
  if (data_map_[tokens[2]].second == FLOAT) {
    asm_.push_back("    lwc1 $f1, 0($t0)");
    asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
    asm_.push_back("    swc1 $f1, 0($t0)");
  } else {
    asm_.push_back("    lw  $t1, 0($t0)");
    asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
    asm_.push_back("    sw $t1, 0($t0)");
  }
}

void GenNaive::array_store_asm(std::vector<std::string>& tokens) {
  asm_.push_back("    la $t0, " + data_map_[tokens[3]].first);
  if (data_map_[tokens[1]].second == FLOAT) {
    asm_.push_back("    lwc1 $f1, 0($t0)");
  } else {
    asm_.push_back("    lw $t1, 0($t0)");
  }
  asm_.push_back("    la $t0, " + data_map_[tokens[2]].first);
  asm_.push_back("    lw $t2, 0($t0)");
  asm_.push_back("    sll $t2, $t2, 2");
  asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
  asm_.push_back("    add $t0, $t0, $t2");
  asm_.push_back("    srl $t2, $t2, 2");
  if (data_map_[tokens[1]].second == FLOAT) {
    asm_.push_back("    swc1 $f1, 0($t0)");
  } else {
    asm_.push_back("    sw $t1, 0($t0)");
  }
}

void GenNaive::condition_asm(std::vector<std::string>& tokens) {
  std::string cond_asm;
  if (tokens[0] == "brgt") {
    cond_asm = "bgt";
  } else if (tokens[0] == "breq") {
    cond_asm = "beq";
  } else if (tokens[0] == "brlt") {
    cond_asm = "blt";
  } else if (tokens[0] == "brgeq") {
    cond_asm = "bge";
  } else if (tokens[0] == "brleq") {
    cond_asm = "ble";
  } else if (tokens[0] == "brneq") {
    cond_asm = "bne";
  }

  asm_.push_back("    la $t0, " + data_map_[tokens[1]].first);
  asm_.push_back("    lw $t1, 0($t0)");
  asm_.push_back("    la $t0, " + data_map_[tokens[2]].first);
  asm_.push_back("    lw $t2, 0($t0)");
  asm_.push_back("    " + cond_asm + ", $t1, $t2, " + tokens[3]);
}

void GenNaive::text_seg() {
  asm_.push_back("\n# Beginning of the code section\n");
  asm_.push_back(".text");

  built_in_printi();
  built_in_exit();
  built_in_not();

  // main entrance
  asm_.push_back("\nmain:\n");

  asm_.push_back("    sw $s0, -4($sp)");
  asm_.push_back("    sw $s1, -8($sp)");
  asm_.push_back("    sw $s2, -12($sp)");
  asm_.push_back("    sw $s3, -16($sp)");
  asm_.push_back("    sw $s4, -20($sp)");
  asm_.push_back("    sw $s5, -24($sp)");
  asm_.push_back("    sw $s6, -28($sp)");
  asm_.push_back("    sw $s7, -32($sp)");
  asm_.push_back("    addi $sp, $sp, -32");
  asm_.push_back("    sw $ra, -4($sp)");
  asm_.push_back("    addi $sp, $sp, -4");

  for (size_t i = 0; i < ir_.size(); ++i) {
    auto& line = ir_[i];
    std::istringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(ss, token, ',')) {
      token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }
    asm_.push_back("\n    # IR:" + line);
    if (tokens[0] == "assign") {
      assign_asm(tokens);
    } else if (tokens[0] == "add" || tokens[0] == "sub" || tokens[0] == "and" ||
               tokens[0] == "or" || tokens[0] == "mult" || tokens[0] == "div") {
      operator_asm(tokens);
    } else if (tokens[0] == "goto") {
      asm_.push_back("    j " + tokens[1]);
    } else if (tokens[0] == "breq" || tokens[0] == "brneq" ||
               tokens[0] == "brlt" || tokens[0] == "brgt" ||
               tokens[0] == "brgeq" || tokens[0] == "brleq") {
      condition_asm(tokens);
    } else if (tokens[0] == "return") {
      return_asm(tokens);
    } else if (tokens[0] == "call" || tokens[0] == "callr") {
      call_asm(tokens);
    } else if (tokens[0] == "array_store") {
      array_store_asm(tokens);
    } else if (tokens[0] == "array_load") {
      array_load_asm(tokens);
    } else if (tokens[0] != "main:") {
      if (tokens[0].find("label") == std::string::npos) {  // function procedure
        asm_.push_back(tokens[0].substr(0, tokens[0].size() - 1) + "0:");
        std::vector<std::string> func_ir;
        while (line.find("return") == std::string::npos) {
          func_ir.push_back(line);
          ++i;
          line = ir_[i];
        }
        func_ir.push_back(line);
      } else {
        asm_.push_back(tokens[0]);
      }
    }
  }
}

void GenNaive::generate() {
  data_seg();
  text_seg();
}
