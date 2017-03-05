
#pragma once

#include <string>
#include <vector>

class Record {
 public:
  int scopeLevel = 0;
  std::string type = "";
  int dimension;
  std::vector<std::string> parameters;
  std::vector<std::string> parameterTypes;
  std::vector<int> parameterDimensions;
  std::string returnType = "";

  Record() {}
  Record(int scopeLevel) { this->scopeLevel = scopeLevel; }
  std::string getType() const { return type; }
  int getDimension() const { return dimension; }
};
