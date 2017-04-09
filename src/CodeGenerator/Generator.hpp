#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

/**
 * Convert IR to MIPS code.
 */
class Generator {
  public:
    Generator(std::string filename);
    Generator(std::vector<std::string>& ir);
    virtual void generate() = 0;
    void dump();
    std::string new_general_reg();
    std::string new_float_reg();
  private:
    virtual void data_seg() = 0;
    virtual void text_seg() = 0;
    inline void load_built_in(std::string filename) {
      std::ifstream file;
      file.open(filename);
      std::string line;
      if (file.is_open()) {
        while (std::getline(file, line)) {
            func_map_[filename].push_back(line);
        }
      } else {
        std::cerr << "Open file " << filename << " failed!\n";
        exit(EXIT_FAILURE);
      }
      file.close();
    }

  protected:
    enum Type {
      INT = 0,
      FLOAT = 1
    };
    /// intermediate code
    std::vector<std::string> ir_;
    /// MIPS ASM code
    std::vector<std::string> asm_;
    /// built in funcs hash map
    std::unordered_map<std::string, std::vector<std::string> > func_map_;
    /// data segment map
    std::unordered_map<std::string, std::pair<std::string, int> > data_map_;

    size_t float_num_ = 0;
    size_t genetal_num_ = 0;
};

class GenNaive final : public Generator {
  public:
    GenNaive(std::string filename) : Generator(filename) {}
    GenNaive(std::vector<std::string>& ir) : Generator(ir) {}
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
