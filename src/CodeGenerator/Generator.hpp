#pragma once

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Convert IR to MIPS code.
 */
class Generator {
 public:
  Generator(std::string filename);
  Generator(std::vector<std::string>& ir,
            std::unordered_map<
                std::string, std::vector<std::pair<std::string, std::string>>>&
                func_info);
  virtual void generate() = 0;
  void dump();
  std::string new_general_reg();
  std::string new_float_reg();

 private:
  virtual void data_seg() = 0;
  virtual void text_seg() = 0;

 protected:
  enum Type { INT = 0, FLOAT = 1 };
  /// intermediate code
  std::vector<std::string> ir_;
  /// MIPS ASM code
  std::vector<std::string> asm_;
  /// function argument regs map <funcname, <(param0, reg0), (param1, reg1), ...
  /// >> use registers: t4 - t9, f12 - f14, a0 - a4, or stack
  std::unordered_map<std::string,
                     std::vector<std::pair<std::string, std::string>>>
      func_map_;
  /// data segment map <data var name, <.space 20, FLOAT>>
  std::unordered_map<std::string, std::pair<std::string, int>> data_map_;

  size_t float_num_ = 0;
  size_t genetal_num_ = 0;

  void built_in_printi();
  void built_in_exit();
  void built_in_not();
};

class GenNaive final : public Generator {
 public:
  GenNaive(std::string filename) : Generator(filename) {}
  GenNaive(std::vector<std::string>& ir,
           std::unordered_map<std::string,
                              std::vector<std::pair<std::string, std::string>>>&
               func_info)
      : Generator(ir, func_info) {}
  void generate() override;

 private:
  void data_seg() override;
  void text_seg() override;
  void assign_asm(std::vector<std::string>& tokens);
  void operator_asm(std::vector<std::string>& tokens);
  void return_asm(std::vector<std::string>& tokens);
  void call_asm(std::vector<std::string>& tokens);
  void array_load_asm(std::vector<std::string>& tokens);
  void array_store_asm(std::vector<std::string>& tokens);
  void condition_asm(std::vector<std::string>& tokens);
  void func_asm(std::vector<std::string>& tokens);
  std::string alloc_reg(std::string token);
  std::string load(std::string token);
  std::string load(std::string token, std::string res);
  void store(std::string token, std::string reg);
  inline void reset_reg() {
    temp_idx_ = 4;
    fp_idx_ = 12;
    a_idx_ = 0;
  }

  size_t temp_idx_ = 4;
  size_t fp_idx_ = 12;
  size_t a_idx_ = 0;
  std::stack<std::string> func_stack_;
};

class GenCFG final : public Generator {
 public:
  void generate() override;

 private:
  void data_seg() override;
  void text_seg() override;
};

class GenEBB final : public Generator {
 public:
  void generate() override;

 private:
  void data_seg() override;
  void text_seg() override;
};
