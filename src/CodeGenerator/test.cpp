#include "Generator.hpp"

int main(int argc, char** argv) {
  Generator* gen = new GenNaive(argv[1]); 
  gen->generate();
  gen->dump();
  return 0;
}