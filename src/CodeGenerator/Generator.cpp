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

  /// init built in funcs
  // load_built_in("lib_printi");
  // load_built_in("lib_exit");
  // load_built_in("lib_not");
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
