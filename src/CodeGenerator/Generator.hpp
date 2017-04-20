#pragma once

#include "Common.hpp"

#include <fstream>
#include <iostream>
#include <map>
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
  Generator(std::vector<std::string>& ir, func_info_t& func_info);
  virtual void generate() = 0;
  void dump();
  std::string new_general_reg();
  std::string new_float_reg();

 private:
  virtual void data_seg() = 0;
  virtual void text_seg() = 0;

 protected:
  void built_in_printi();
  void built_in_exit();
  void built_in_not();
  bool is_num(std::string s);
  std::string remove_white_space(std::string token);
  std::vector<std::string> cvt2tokens(size_t id);

  enum Type { INT = 0, FLOAT = 1 };
  /// intermediate code
  std::vector<std::string> ir_;
  /// MIPS ASM code
  std::vector<std::string> asm_;

  func_info_t func_map_;
  /// data segment map <data var name, <.space 20, FLOAT>>
  std::unordered_map<std::string, std::pair<std::string, int>> data_map_;

  std::stack<std::string> func_stack_;

  size_t temp_idx_ = 4;
  size_t fp_idx_ = 12;
  size_t a_idx_ = 0;
};

class GenNaive final : public Generator {
 public:
  GenNaive(std::string filename) : Generator(filename) {}
  GenNaive(std::vector<std::string>& ir, func_info_t& func_info)
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
};

class GenCFG final : public Generator {
 public:
  GenCFG(std::vector<std::string>& ir, func_info_t& func_info)
      : Generator(ir, func_info) {
    find_blocks(ir);
  }

  void generate() override;

 private:
  void data_seg() override;
  void text_seg() override;
  void find_blocks(std::vector<std::string>& ir);
  void analyse_live();
  graph_ptr build_graph(size_t id);

  std::vector<block_t> blocks_;
  std::map<variable_t, live_range_t> live_ranges_;
  std::map<variable_t, reg_t> regs_;
  std::vector<std::vector<std::string>> vars_;
};

class GenEBB final : public Generator {
 public:
  void generate() override;

 private:
  void data_seg() override;
  void text_seg() override;
};
