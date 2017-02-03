#include "Scanner.hpp"

/**
 * For testing purposes
 */
int main(int argc, char **argv) {
  Scanner scanner(argv[1]);
  TokenPair *tokenPair = scanner.getToken();
  while (tokenPair->getTokenType().getValue() != Symbol::Terminal::EOFF) {
    std::cout << tokenPair->emit();
    tokenPair = scanner.getToken();
  }
  return 0;
}
