
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

  /// get type
  std::string getType() const { return type; }
  /// get dimentsion
  int getDimension() const { return dimension; }
  /// get return type
  std::string getReturnType() const { return returnType; }
  /// get parameter types
  const std::vector<std::string>& getParameterTypes() const {
    return parameterTypes;
  }
  /// get parameter dimensions
  const std::vector<int>& getParameterDimensions() const {
    return parameterDimensions;
  }  
};
