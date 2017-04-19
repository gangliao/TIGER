ADD_LIBRARY(generator
            CodeGenerator/Generator.cpp
            CodeGenerator/GenCFG.cpp
            CodeGenerator/GenNaive.cpp)

ADD_EXECUTABLE(parser Parser/Parser.cpp)
TARGET_LINK_LIBRARIES(parser generator)

ADD_EXECUTABLE(scanner Scanner/Scanner.cpp)
TARGET_LINK_LIBRARIES(scanner generator)

ADD_EXECUTABLE(testGen CodeGenerator/test.cpp)
TARGET_LINK_LIBRARIES(testGen generator)
