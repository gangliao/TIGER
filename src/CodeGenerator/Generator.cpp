#include "Generator.hpp"

std::string Generator::new_general_reg() {
  return "$t" + std::to_string(genetal_num_++);
}
std::string Generator::new_float_reg() {
  return "$f" + std::to_string(float_num_++);
}

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

Generator::Generator(std::vector<std::string>& ir) { ir_ = std::move(ir); }

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