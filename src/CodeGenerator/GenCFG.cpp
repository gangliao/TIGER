#include "Generator.hpp"

void GenCFG::data_seg() {
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
    std::vector<std::string> tokens;
    tokens = cvt2tokens(line);

    if (tokens[0] == "assign") {
      if (data_map_.find(tokens[1]) == data_map_.end()) {
        if (tokens.size() == 4) {
          data_float.push_back(tokens[1] + ": \t\t.space \t" +
                               std::to_string(4 * atoi(tokens[2].c_str())));
          data_map_[tokens[1]] = std::make_pair(
              tokens[1],
              (tokens[3].find(".") != std::string::npos ? FLOAT : INT));
          arrays_.insert(tokens[1]);
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
  for (auto& data : data_word) {
    asm_.push_back(data);
  }
  for (auto& data : data_float) {
    asm_.push_back(data);
  }
}

void GenCFG::find_blocks() {
  std::vector<size_t> line_num;
  for (size_t i = 0; i < ir_.size(); ++i) {
    std::vector<std::string> tokens;
    tokens = cvt2tokens(i);
    if (tokens[0].find("main:") != std::string::npos ||
        (tokens[0].find(":") != std::string::npos &&
         tokens[0].find("label") != std::string::npos)) {
      line_num.push_back(i);
    }
  }
  for (size_t i = 0; i < line_num.size(); ++i) {
    size_t range_beg = line_num[i] + 1;
    size_t range_end =
        line_num.size() == (i + 1) ? ir_.size() : line_num[i + 1];
    block_t block = {range_beg, range_end};
    std::cout << "\n# Detect block " << i << " IR line : " << range_beg << " ~ "
              << range_end << std::endl;
    blocks_.push_back(block);
  }
}

void GenCFG::graph_coloring(size_t id, graph_ptr graph) {
  size_t int_regs[8];
  size_t float_regs[16];
  memset(int_regs, 0, sizeof(size_t) * 8);
  memset(float_regs, 0, sizeof(size_t) * 16);
  std::unordered_map<std::string, reg_t> regs;

  auto max_priority = [&]() {
    static size_t i = 0;
    std::string var_name;
    for (auto& node : *graph) {
      if (i == 0) {
        var_name = node.first;
        break;
      }
    }
    size_t size = -1;
    for (auto& node : *graph) {
      auto& var = node.first;
      auto& vec = node.second;
      if (size < vec.size()) {
        // max spill cost
        var_name = var;
        size = vec.size();
      }
    }
    return var_name;
  };

  while (!graph->empty()) {
    auto var = max_priority();
    auto table = data_map_[var].second == INT ? int_regs : float_regs;
    auto size = data_map_[var].second == INT ? 8 : 16;
    for (size_t i = 0; i < size; ++i) {
      if (table[i] == 0) {
        regs[var] = i;  // store into register
        table[i] = 1;
        break;
      }
    }
    if (regs.find(var) == regs.end()) {
      regs[var] = -1;  // store into memory
    }
    regs_[{id, var}] = regs[var];
    if (!var.empty()) {
      graph->erase(var);
    }
  }
}

graph_ptr GenCFG::build_graph(size_t id) {
  // graph data structure
  graph_ptr graph = std::make_shared<graph_t>();

  size_t size = vars_[id].size();
  for (size_t i = 0; i < size; ++i) {
    std::string var = vars_[id][i];
    size_t beg_pos = live_ranges_[{id, var}].start_pos_;
    size_t end_pos = live_ranges_[{id, var}].end_pos_;
    std::vector<std::string> temp_vec;
    for (size_t j = 0; j < size; ++j) {
      if (i == j) continue;
      std::string temp_var = vars_[id][j];
      size_t temp_beg = live_ranges_[{id, temp_var}].start_pos_;
      size_t temp_end = live_ranges_[{id, temp_var}].end_pos_;
      if (end_pos >= temp_beg && beg_pos <= temp_end) {
        temp_vec.push_back(temp_var);
      }
    }
    (*graph)[var] = temp_vec;
  }
  return graph;
}

void GenCFG::analyse_live() {
  vars_.reserve(blocks_.size());
  // store vars in each blocks into vars_
  for (size_t i = 0; i < blocks_.size(); ++i) {
    size_t beg_pos = blocks_[i].start_pos_;
    size_t end_pos = blocks_[i].end_pos_;
    std::vector<std::string> tokens;
    std::set<std::string> vars;
    for (size_t j = beg_pos; j < end_pos; ++j) {
      tokens = cvt2tokens(j);
      size_t i = 0;
      for (auto& token : tokens) {
        if (i++ == 0) continue;
        if (tokens[0] == "call" && i == 2) continue;
        if (tokens[0] == "callr" && i == 3) continue;
        if (token.find(":") != std::string::npos) continue;
        if (token.find("label") != std::string::npos) continue;
        // if it's a variable
        if (!is_num(token) && arrays_.find(token) == arrays_.end()) {
          vars.insert(token);
        }
      }
    }

    std::vector<std::string> temp_vars;
    temp_vars.reserve(vars.size());
    for (auto& var : vars) {
      temp_vars.push_back(var);
    }
    vars_.push_back(std::move(temp_vars));
  }
  // lamda function: analyse live range
  size_t live_beg, live_end;
  auto detect_live_range = [&](size_t id, std::string var_name) {
    size_t beg_pos = blocks_[id].start_pos_;
    size_t end_pos = blocks_[id].end_pos_;
    size_t times = 0;
    for (size_t j = beg_pos; j < end_pos; ++j) {
      auto& line = ir_[j];
      if (line.find(var_name) != std::string::npos) {
        if (times++ == 0) {
          live_beg = j;
          live_end = j;
        } else {
          live_end = j;
        }
      }
    }
  };
  // set live range for each variable in each block
  for (size_t i = 0; i < vars_.size(); ++i) {
    for (size_t j = 0; j < vars_[i].size(); ++j) {
      detect_live_range(i, vars_[i][j]);
      live_ranges_[{i, vars_[i][j]}] = {live_beg, live_end};
    }
  }
  // build graph for each block and graph coloring
  for (size_t i = 0; i < vars_.size(); ++i) {
    graph_coloring(i, build_graph(i));
  }
}

std::string GenCFG::alloc_reg(std::string token) {
  std::string funcname;
  if (!func_stack_.empty()) {
    funcname = func_stack_.top();
    auto& params = func_map_[funcname];
    for (auto& param : params) {
      if (param.first == token) {
        if (param.second == "int") {
          param.second = "$a" + std::to_string(a_idx_);
          return "$a" + std::to_string(a_idx_++);
        } else if (param.second == "float") {
          param.second = "$f" + std::to_string(fp_idx_);
          return "$f" + std::to_string(fp_idx_++);
        } else {
          return param.second;
        }
      }
    }
  }

  if (is_inside_block_ == true) {
    if (regs_.find({block_id_, token}) != regs_.end() &&
        regs_[{block_id_, token}] != -1) {
      if (data_map_[token].second == FLOAT) {
        return "$f" + std::to_string(regs_[{block_id_, token}]);
      } else {
        return "$t" + std::to_string(regs_[{block_id_, token}]);
      }
    }
  }

  return "";
}

std::string GenCFG::load(std::string token) {
  std::string reg = alloc_reg(token);
  if (reg.empty()) {
    asm_.push_back("    la $t9, " + data_map_[token].first);
    if (check_type(token) == FLOAT) {
      reg = "$f16";
      asm_.push_back("    lwc1 $f16, 0($t9)");
    } else {
      reg = "$t8";
      asm_.push_back("    lw $t8, 0($t9)");
    }
  }
  return reg;
}

std::string GenCFG::load(std::string token, std::string res) {
  std::string reg = alloc_reg(token);
  if (reg.empty()) {
    asm_.push_back("    la $t9, " + data_map_[token].first);
    reg = res;
    if (check_type(token) == FLOAT) {
      asm_.push_back("    lwc1 " + reg + ", 0($t9)");
    } else {
      asm_.push_back("    lw " + reg + ", 0($t9)");
    }
  }
  return reg;
}

void GenCFG::store(std::string token, std::string reg) {
  std::string new_reg = alloc_reg(token);
  if (new_reg.empty()) {
    asm_.push_back("    la $t9, " + data_map_[token].first);
    if (check_type(token) == FLOAT) {
      asm_.push_back("    swc1 " + reg + ", 0($t9)");
    } else {
      asm_.push_back("    sw " + reg + ", 0($t9)");
    }
  } else {
    if (check_type(token) == FLOAT) {
      asm_.push_back("    mov.s " + new_reg + ", " + reg);
    } else {
      asm_.push_back("    move " + new_reg + ", " + reg);
    }
  }
}

void GenCFG::assign_asm(std::vector<std::string>& tokens) {
  reset_reg();
  if (tokens.size() == 4) {  // array
    size_t size = atoi(tokens[2].c_str());
    if (check_type(tokens[3]) == FLOAT) {
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
        asm_.push_back("    sw $t1, " + std::to_string(4 * (i + 1)) + "($t0)");
      }
    }
  } else if (tokens.size() == 3) {
    store(tokens[1], load(tokens[2]));
  }
  reset_reg();
}

void GenCFG::operator_asm(std::vector<std::string>& tokens) {
  reset_reg();
  std::string reg1, reg2;
  if (check_type(tokens[3]) == INT) {
    reg1 = load(tokens[1], "$t8");
    reg2 = load(tokens[2], "$t9");
    if (tokens[0] != "mult" && tokens[0] != "div") {
      asm_.push_back("    " + tokens[0] + " $t8" + ", " + reg1 + ", " +
                     reg2);
      reg1 = "$t8";
    } else {
      asm_.push_back("    " + tokens[0] + " " + reg1 + ", " + reg2);
      asm_.push_back("    mflo " + reg1);
    }
  } else {
    reg1 = load(tokens[1], "$f16");
    reg2 = load(tokens[2], "$f17");
    if (tokens[0] != "mult" && tokens[0] != "div") {
      asm_.push_back("    " + tokens[0] + ".s " + " $f16" + ", " + reg1 +
                     ", " + reg2);
      reg1 = "$f16";               
    } else {
      asm_.push_back("    " + tokens[0] + " " + reg1 + ", " + reg2);
      asm_.push_back("    mflo " + reg1);
    }
  }
  store(tokens[3], reg1);
  reset_reg();
}

void GenCFG::return_asm(std::vector<std::string>& tokens) {
  if (tokens.size() == 2) {
    asm_.push_back("    move $v0, " + load(tokens[1]));
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

void GenCFG::call_asm(std::vector<std::string>& tokens) {
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
    if (check_type(tokens[param_idx]) == INT) {
      asm_.push_back("    move $a" + std::to_string(i) + ", " +
                     load(tokens[param_idx + i]));
    } else {
      asm_.push_back("    mov.s $f" + std::to_string(i + 12) + ", " +
                     load(tokens[param_idx + i]));
    }
  }

  std::string funcname =
      ret_func_name(tokens[0] == "callr" ? tokens[2] : tokens[1]);

  asm_.push_back("    jal " + funcname);
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
    if (check_type(tokens[1]) == INT) {
      asm_.push_back("    move " + load(tokens[1]) + ", $v0");
    } else {
      asm_.push_back("    mov.s " + load(tokens[1]) + ", $v0");
    }
  }
}

void GenCFG::array_load_asm(std::vector<std::string>& tokens) {
  std::string reg = load(tokens[3], "$t9");
  asm_.push_back("    sll " + reg + ", " + reg + ", 2");
  asm_.push_back("    la $t8, " + data_map_[tokens[2]].first);
  asm_.push_back("    add $t8, $t8, " + reg);
  asm_.push_back("    srl " + reg + ", " + reg + ", 2");
  if (check_type(tokens[2]) == FLOAT) {
    asm_.push_back("    lwc1 $f16, 0($t8)");
    store(tokens[1], "$f16");
  } else {
    asm_.push_back("    lw  $t8, 0($t8)");
    store(tokens[1], "$t8");
  }
}

void GenCFG::array_store_asm(std::vector<std::string>& tokens) {
  std::string reg = load(tokens[2], "$t9");
  asm_.push_back("    sll " + reg + ", " + reg + ", 2");
  asm_.push_back("    la $t8, " + data_map_[tokens[1]].first);
  asm_.push_back("    add $t8, $t8, " + reg);
  asm_.push_back("    srl " + reg + ", " + reg + ", 2");
  if (check_type(tokens[1]) == FLOAT) {
    asm_.push_back("    swc1 " + load(tokens[3], "$f16") + ", 0($t8)");
  } else {
    asm_.push_back("    sw " + load(tokens[3], "$t9") + ", 0($t8)");
  }
}

void GenCFG::condition_asm(std::vector<std::string>& tokens) {
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

  std::string reg1 = load(tokens[1], "$t8");
  std::string reg2 = load(tokens[2], "$t9");

  asm_.push_back("    " + cond_asm + ", " + reg1 + ", " + reg2 + ", " +
                 tokens[3]);
}

void GenCFG::func_asm(std::vector<std::string>& code) {
  func_stack_.push(code[0].substr(0, code[0].size() - 1));
  reset_reg();
  asm_.push_back("\n" + code[0] + "\n");
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

  for (size_t i = 1; i < code.size(); ++i) {
    auto& line = code[i];
    std::vector<std::string> tokens;
    tokens = cvt2tokens(line);
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
      // function procedure
      asm_.push_back(tokens[0]);
    }
  }

  reset_reg();
  func_stack_.pop();
}

void GenCFG::text_seg() {
  asm_.push_back("\n# Beginning of the code section\n");
  asm_.push_back(".text");

  built_in_printi();
  built_in_exit();
  built_in_not();

  // main entrance
  asm_.push_back("main:");

  size_t times = 0;
  for (size_t i = 0; i < ir_.size(); ++i) {
    auto& line = ir_[i];
    std::vector<std::string> tokens;
    tokens = cvt2tokens(line);
    // init block variables or release block registers
    block_init(i);
    if (tokens[0] == "assign") {
      asm_.push_back("\n    # IR:" + line);
      assign_asm(tokens);
    } else if (tokens[0] == "add" || tokens[0] == "sub" || tokens[0] == "and" ||
               tokens[0] == "or" || tokens[0] == "mult" || tokens[0] == "div") {
      asm_.push_back("\n    # IR:" + line);
      operator_asm(tokens);
    } else if (tokens[0] == "goto") {
      block_release(i);
      asm_.push_back("    j " + tokens[1]);
      continue;
    } else if (tokens[0] == "breq" || tokens[0] == "brneq" ||
               tokens[0] == "brlt" || tokens[0] == "brgt" ||
               tokens[0] == "brgeq" || tokens[0] == "brleq") {
      asm_.push_back("\n    # IR:" + line);
      condition_asm(tokens);
    } else if (tokens[0] == "return") {
      block_release(i);
      asm_.push_back("\n    # IR:" + line);
      return_asm(tokens);
      continue;
    } else if (tokens[0] == "call" || tokens[0] == "callr") {
      asm_.push_back("\n    # IR:" + line);
      call_asm(tokens);
    } else if (tokens[0] == "array_store") {
      asm_.push_back("\n    # IR:" + line);
      array_store_asm(tokens);
    } else if (tokens[0] == "array_load") {
      asm_.push_back("\n    # IR:" + line);
      array_load_asm(tokens);
    } else if (tokens[0] == "main:") {
      asm_.push_back("\nmain0:\n");
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
    } else {
      if (tokens[0].find("label") == std::string::npos) {
        if (times == 0) {
          asm_.push_back("\n    # IR: goto, main0");
          asm_.push_back("    j main0");
          times++;
        }
        // function procedure
        std::vector<std::string> func_ir;
        while (line.find("return") == std::string::npos) {
          func_ir.push_back(line);
          ++i;
          line = ir_[i];
        }
        func_ir.push_back(line);
        func_asm(func_ir);
      } else {
        asm_.push_back(tokens[0]);
      }
    }
    block_release(i);
  }
}

void GenCFG::block_init(size_t line_id) {
  if (blocks_[block_id_].start_pos_ == line_id) {
    asm_.push_back("\n    # Enter block and load vars into registers ... \n");
    // load a set of variable you expected to use
    auto& vars = vars_[block_id_];
    for (size_t i = 0; i < vars.size(); ++i) {
      if (regs_[{block_id_, vars[i]}] != -1) {
        if (data_map_[vars[i]].second == INT) {
          load(vars[i], "$t" + std::to_string(regs_[{block_id_, vars[i]}]));
        } else {
          load(vars[i], "$f" + std::to_string(regs_[{block_id_, vars[i]}]));
        }
      }
    }
    // enter into block
    is_inside_block_ = true;
  }
}

void GenCFG::block_release(size_t line_id) {
  if (blocks_[block_id_].end_pos_ - 1 == line_id) {
    // leave this block
    is_inside_block_ = false;
    asm_.push_back("\n    # Leave block and save registers into vars ... \n");
    // save a set of variable in registers
    auto& vars = vars_[block_id_];
    for (size_t i = 0; i < vars.size(); ++i) {
      if (regs_[{block_id_, vars[i]}] != -1) {
        if (data_map_[vars[i]].second == INT) {
          store(vars[i], "$t" + std::to_string(regs_[{block_id_, vars[i]}]));
        } else {
          store(vars[i], "$f" + std::to_string(regs_[{block_id_, vars[i]}]));
        }
      }
    }

    // detect the next block
    block_id_++;
  }
}

void GenCFG::generate() {
  data_seg();
  find_blocks();   // detect blocks
  analyse_live();  // live range analysis
  text_seg();
}
