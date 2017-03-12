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

### Desgin Docs