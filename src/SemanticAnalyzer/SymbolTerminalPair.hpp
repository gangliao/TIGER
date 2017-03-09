/**
 * SymbolTerminalPair
 *
 * @author:  gatech Buzz
 */

#pragma once

#include "../common/Symbol.hpp"

class SymbolTerminalPair {
 private:
  Symbol a;
  Symbol w;

 public:
  SymbolTerminalPair(int a, int w) {
    this->a = a;
    this->w = w;
  }
};
