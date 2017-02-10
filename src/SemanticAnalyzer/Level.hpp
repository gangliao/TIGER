
#pragma once

#include <set>

// Complete the Level definition and other methods required if any
struct Level {
private:
  Level *upperLevel;
  std::set<Level *> lowerLevels;
  //...
};
