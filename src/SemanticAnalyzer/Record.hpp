class Record {
public:
  int scopeLevel = 0;
  std::string type = "";
  int dimension;
  std::vector<std::string> parameters;
  std::vector<std::string> parameterTypes;
  std::vector<int> parameterDimensions;
  std::string returnType = "";

  Record(int scopeLevel) { this->scopeLevel = scopeLevel; }
};
