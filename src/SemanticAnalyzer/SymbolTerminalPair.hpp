/**
 * SymbolTerminalPair
 *
 * @author:  gatech Buzz
 */

#pragma once

#include "../common/Symbol.hpp"

class SymbolTerminalPair final {
 private:
  Symbol a;
  Symbol w;

 public:
  explicit SymbolTerminalPair(int a, int w) {
    this->a = a;
    this->w = w;
  }

  friend bool operator<(const SymbolTerminalPair &lhs,
                        const SymbolTerminalPair &rhs) {
    if (lhs.a.getValue() < rhs.a.getValue()) {
      return true;
    } else {
      if (lhs.a.getValue() == rhs.a.getValue()) {
        if (lhs.w.getValue() < rhs.w.getValue()) {
          return true;
        }
      }
    }
    return false;
  }
};
