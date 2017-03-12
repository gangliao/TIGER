## Tiger Compiler - Front End

### How to Build

1. build:

	```bash
		# cd project dir
		cd Tiger-Compiler
		# build scanner and parser
		make
	```
	
2. run:

	You can parsing test cases named `*.tiger` under `/testCases/test-phaseI` to generate IR code.

	```bash
		# naive mode
		bin/parser <filename>
		# verbose mode: "-d to implement a verbose mode" 
		bin/parser <filename> -d
	```	

3. test:

	In `/testCases/test-phaseI` directory, it includes a test script `test.sh` to execute all of 32 test cases and
	generate the corresponding ouput file `*.tiger.txt`. 

	You can simply issue the commands:

	```bash
	cd testCases/test-phaseI
	sh ./test.sh
	```

### Demo

<a href="https://asciinema.org/a/89pja97rl0kb6s49iejc47lmn" target="_blank"><img src="https://asciinema.org/a/89pja97rl0kb6s49iejc47lmn.png" width="600" /></a>

### Desgin Internals

#### Design LL(1) Parse Table

1. Hand-modified Tiger grammars
2. Hand-written parser table
3. Parser code

#### Symbol Table

#### Semantic Checking

#### Intermediate Code

