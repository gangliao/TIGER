## TIGER - A Tiny Full Compiler

[![Build Status](https://travis-ci.com/gangliao/TIGER.svg?token=bQUUVqcsZpghUieTvsMM&branch=master)](https://travis-ci.com/gangliao/TIGER/builds)

This compiler includes both front end and back end.

> Front end: Grammar Rules, LL(1) Parse Table, Syntax and Semantic Check and Intermediate Code.


> Back end: IR Optimization (Intra-block CFG optimization), MIPS Register Allocation, Instruction Selection and Code Generation.


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

	In [testCases2](testCases2) directory, it includes a test script `test.sh` to execute all test cases and
	generate the corresponding asm files `*.naive.s` and `*.cfg.s`.

	After souce code is compiled, you can simply issue the commands:

	```bash
	cd testCases2
	sh ./test.sh
	```

### Demo

**NOTE:** This demo is `gif` graph format. If you have problem to view it in the markdown file,
you can directly open it which located at `img/demo.gif`.

This Demo shows that

	(1) [Compile Source Code] How to compile and generate parser binary ?

	(2) [Compiler Front End] How to transfrom raw tiger program into IR code ?

	(3) [Compiler Back End] How to generate optimized MIPS asm code via IR code ?

<a href="img/demo.gif" target="_blank"><img src="img/demo.gif" width="1000" /></a>

### Desgin Internals

[Tiger Compiler Front End - Design Internals](design_doc/front_end.md)

[Tiger Compiler Back End - Design Internals](design_doc/back_end.md)

### Test Cases

We passed all tests cases which provided by TA.

Please check out the details in report [Phase2_Testing_and_Output.pdf](Phase2_Testing_and_Output.pdf) from current directory, which includes test cases and their quality comparisons for naive and CFG intra-block register allocation.

### Accomplishment

- [x] Register allocation code
	- [x] Naive
	- [x] CFG and intra block allocation
	- [ ] EBB and intra-EBB allocation
	- [x] Whole function register allocation
	- [x] Live Range Analysis and Graph Coloring
- [x] Instruction selection and generation code
- [x] Passes tests using generated code executing on simulator.
- [x] Report (desgin Internals, how to build, run, code quality comparisions, etc.)
