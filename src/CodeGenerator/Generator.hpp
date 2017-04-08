#pragma once

#include <vector>
#include <string>

/**
 * Convert IR to MIPS code.
 */
class Generator {
  public:
    Generator(std::string filename);
    Generator(std::vector<std::string>& ir);
    virtual void generate() = 0;
    void dump();
  private:
    /// intermediate code
    std::vector<std::string> ir_;
    /// MIPS ASM code
    std::vector<std::string> asm_;
};

class NaiveCons final : public Generator {
  public:
    void generate() override;
};

class CFGCons final : public Generator {
  public:
    void generate() override;
};

class EBBCons final : public Generator {
  public:
    void generate() override;
};