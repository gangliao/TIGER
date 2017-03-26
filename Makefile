GCC=g++
CFLAGS=-std=c++11 -g
BIN=bin/
SRC=src/

#All: scanner parser
All: scanner parser nas cfg

scanner: dir
	$(GCC) $(CFLAGS) -o $(BIN)$@  $(SRC)Scanner/Scanner.cpp

parser: dir
	$(GCC) $(CFLAGS) -o $(BIN)$@ $(SRC)Parser/Parser.cpp

nas: dir
	$(GCC) $(CFLAGS) -o $(BIN)$@ $(SRC)Parser/NaiveAllocationScheme.cpp
	
cfg: dir
	$(GCC) $(CFLAGS) -o $(BIN)$@ $(SRC)Parser/cfg.cpp
	
dir:
	mkdir -p $(BIN)$

clean:
	#rm $(BIN)parser $(BIN)scanner
	rm $(BIN)parser $(BIN)scanner rm $(BIN)nas rm $(BIN)cfg
