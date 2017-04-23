## TIGER - A Tiny Full Compiler

This compiler includes both front end and back end. Front end: Grammar Rules, LL(1) Parse Table, Syntax and Semantic Check and Intermediate Code. Back end: IR Optimization (Intra-block CFG optimization), MIPS Register Allocation, Instruction Selection and Code Generation.


### How to Build

0. development environment

	Currently, this project repository is maintained on github privately and also been deployed on Travis CI.
	It supports both Ubuntu and Mac OS X.

1. build:

	```bash
		# cd project dir
		cd Tiger-Compiler
		# build scanner, parser, generator
		mkdir build && cd build
		# cmake building tool
		cmake ..
		make -j4
	```

2. run:

	You can parsing test cases named `*.tiger` under `/testCases2` to generate IR code.
	Default it will utilize **CFG optimized technique** to generate MIPS asm code.

	```bash
		# verbose mode: "-d to implement a verbose mode"
		./src/parser <filename> -d
	```

	If you want to use the **naive** mode to generate asm code, simply issue:

	```bash
		./src/parser <filename> -d -naive
	```

3. test:

	In `/testCases2` directory, it includes a test script `test.sh` to execute all test cases and
	generate the corresponding asm files `*.naive.s` and `*.cfg.s`.

	You can simply issue the commands:

	```bash
	cd testCases2
	sh ./test.sh
	```

### Demo

This Demo shows that

(1) [Compile Source Code] How to compile and generate parser binary ?

(2) [Compiler Front End] How to transfrom raw tiger program into IR code ?

(3) [Compiler Back End] How to generate optimized MIPS asm code via IR code ?

<a href="https://asciinema.org/a/89pja97rl0kb6s49iejc47lmn" target="_blank"><img src="https://asciinema.org/a/89pja97rl0kb6s49iejc47lmn.png" width="600" /></a>

### Desgin Internals

[Tiger Compiler Front End - Design Internals](design_doc/front_end.md)

[Tiger Compiler Back End - Design Internals](design_doc/back_end.md)

### Reference

1. [MIPS syscall](design_doc/syscall_mips.md)
