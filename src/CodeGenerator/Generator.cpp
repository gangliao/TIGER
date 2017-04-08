#include "Generator.hpp"
#include <iostream>
#include <fstream>

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

Generator::Generator(std::vector<std::string>& ir) {
  ir_ = std::move(ir);
}

void Generator::dump() {
  std::cout << "\n\n----------------------------------------" << std::endl;
  std::cout << "Generate ASM CODE ..." << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  for (auto& code : asm_) {
    std::cout << code << std::endl;
  }
  std::cout << "----------------------------------------\n" << std::endl;  
}