#include "Generator.hpp"

Generator::Generator(std::string filename) {
  std::ifstream file;
  file.open(filename);
  std::string line;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      ir_.push_back(line);
    }
  } else {
    std::cerr << "Open file " << filename << " failed!";
    exit(EXIT_FAILURE);
  }
  file.close();
}

Generator::Generator(
    std::vector<std::string>& ir,
    std::unordered_map<std::string,
                       std::vector<std::pair<std::string, std::string>>>&
        func_info) {
  ir_ = std::move(ir);
  func_map_ = std::move(func_info);
}

void Generator::dump() {
  std::cout << "\n\n#----------------------------------------" << std::endl;
  std::cout << "# Generate ASM CODE ..." << std::endl;
  std::cout << "#----------------------------------------" << std::endl;
  for (auto& code : asm_) {
    std::cout << code << std::endl;
  }
  std::cout << "#----------------------------------------\n" << std::endl;
}

void Generator::built_in_printi() {
  asm_.push_back("lib_printi:");
  asm_.push_back("    # Callee Convention (entering printi):");
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
  asm_.push_back("    li $v0, 1");
  asm_.push_back("    syscall");
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
  asm_.push_back("    jr $ra\n");
}

void Generator::built_in_exit() {
  asm_.push_back("lib_exit:");
  asm_.push_back("    # Callee Convention (entering exit):");
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
  asm_.push_back("    li $v0, 17");
  asm_.push_back("    syscall");
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
  asm_.push_back("    jr $ra\n");
}

void Generator::built_in_not() {
  asm_.push_back("lib_not:");
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
  asm_.push_back("    addi $sp, $sp, -4\n");
  asm_.push_back("    # IR: assign, var0, 0,");
  asm_.push_back("    la $t0, num_0");
  asm_.push_back("    lw $t1, 0($t0)");
  asm_.push_back("    move $s8, $t1\n");
  asm_.push_back("    # IR: brneq, i1, 0, _if_label0");
  asm_.push_back("    li $t1, 0");
  asm_.push_back("    bne $a0, $t1, _if_label0\n");
  asm_.push_back("    # IR: assign, var0, 1,");
  asm_.push_back("    li $t1, 1");
  asm_.push_back("    move $s8, $t1");
  asm_.push_back("_if_label0:\n");
  asm_.push_back("    # IR: breq, var0, 0, _if_label1");
  asm_.push_back("    li $t1, 0");
  asm_.push_back("    beq $s8, $t1, _if_label1\n");
  asm_.push_back("    # IR: assign, i1, 1,");
  asm_.push_back("    li $t1, 1");
  asm_.push_back("    move $a0, $t1\n");
  asm_.push_back("    # IR: goto, _if_label2, ,");
  asm_.push_back("    j _if_label2");
  asm_.push_back("_if_label1:");
  asm_.push_back("    # IR: assign, i1, 0,");
  asm_.push_back("    li $t1, 0");
  asm_.push_back("    move $a0, $t1");
  asm_.push_back("_if_label2:");
  asm_.push_back("lib_not_end:\n");
  asm_.push_back("    # IR: return, i1, ,");
  asm_.push_back("    # Callee Convention (exiting not):");
  asm_.push_back("    move $v0, $a0");
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
  asm_.push_back("    jr $ra\n");
}

bool Generator::is_num(std::string s) {
  std::stringstream sin(s);
  double t;
  char p;
  if (!(sin >> t)) return false;
  if (sin >> p)
    return false;
  else
    return true;
}

std::string Generator::remove_white_space(std::string token) {
  int length = token.size();
  int i = 0, j = length - 1;
  while (i < length && isspace(token[i] & 0xFF)) {
    i++;
  }
  while (j >= 0 && isspace(token[j] & 0xFF)) {
    j--;
  }
  if (j < i) return std::string();
  return token.substr(i, j - i + 1);
}

std::vector<std::string> Generator::cvt2tokens(size_t id) {
  auto& line = ir_[id];
  std::istringstream ss(line);
  std::vector<std::string> tokens;
  std::string token;
  while (std::getline(ss, token, ',')) {
    token = remove_white_space(token);
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

std::vector<std::string> Generator::cvt2tokens(std::string line) {
  std::istringstream ss(line);
  std::vector<std::string> tokens;
  std::string token;
  while (std::getline(ss, token, ',')) {
    token = remove_white_space(token);
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

std::string Generator::ret_func_name(std::string name) {
  if (name == "printi") {
    return "lib_printi";
  } else if (name == "not") {
    return "lib_not";
  } else if (name == "exit") {
    return "lib_exit";
  }
  return name;
}

Generator::Type Generator::check_type(std::string name) {
  if (!func_stack_.empty()) {
    std::string funcname = func_stack_.top();
    auto& params = func_map_[funcname];
    for (auto& param : params) {
      if (param.first == name) {
        if (param.second == "int") {
          return INT;
        } else {
          return FLOAT;
        }
      }
    }
  }
  if (data_map_[name].second == INT) {
    return INT;
  } else {
    return FLOAT;
  }
}
