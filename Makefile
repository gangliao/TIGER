GCC=g++
CFLAGS=-std=c++11 -g
BIN=bin/
SRC=src/

All: scanner parser

scanner: dir
	$(GCC) $(CFLAGS) -o $(BIN)$@  $(SRC)Scanner/Scanner.cpp

parser: dir
	$(GCC) $(CFLAGS) -o $(BIN)$@ $(SRC)Parser/Parser.cpp

dir:
	mkdir -p $(BIN)$

clean:
	rm $(BIN)parser $(BIN)scanner
