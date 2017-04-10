ADD_EXECUTABLE(parser Parser/Parser.cpp)
ADD_EXECUTABLE(scanner Scanner/Scanner.cpp)

ADD_LIBRARY(generator
            CodeGenerator/Generator.cpp
            CodeGenerator/GenNaive.cpp)

ADD_EXECUTABLE(testGen CodeGenerator/test.cpp)
TARGET_LINK_LIBRARIES(testGen generator)
