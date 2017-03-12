## Tiger Compiler - Front End

### How to Build

0. development environment

	Currently, this project repository is maintained on github privately and also been deployed on Travis CI.
	It supports both Ubuntu and Mac OS X.

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

	First, we need to rewrite the grammar given in the Tiger language specification to remove the ambiguity by enforcing operator precedences and left associativity. This part is to be done by hand. You can check out our [modified grammar file](https://github.com/gangliao/Tiger-Compiler/blob/master/phase1/part1/gang/grammar.md) in this repo.

2. Hand-written parse table

	Modifying the grammar obtained in step 1 to support LL(1) parsing. This could include removing left recursion and performing left factoring on the grammar obtained in step 1 above. Creating the LL(1) parser table for Tiger. This will drive the decision-making process for the parser. This part is to be done by hand by using the theory of LL parsing by finding the first(), follow() sets that help you develop the parser table (please check out [parser table file](https://github.com/gangliao/Tiger-Compiler/blob/master/phase1/part1/gang/parser_table.md) in this repo.)

3. Parser code

	After hand-written parser table is created, it should be hand-coded into our program. we create a data structure - hash table:
	
	```c++
	std::map<SymbolTerminalPair, std::vector<int> > parseTable_;
	```

	Here, class `SymbolTerminalPair` includes a pair members `(Entry entry, std::string name)`, `std::vector<int>` in `parseTable_` is the actual expansion grammar rules.
	
	To build a parse table, we can simply insert all next terminals with their grammer rules **include action symbol** into hash table `parseTable_`.

	As a simple example, consider the following:

	```c++
	/// insert items into parse table
	void addToParseTable(const int nonterm, const std::vector<int> &terminals,
						 const std::vector<int> &expand_rule);
	// # tiger-program
	// 1: <tiger-program> -> let <declaration-segment> in <stat-seq> end
	addToParseTable(Symbol::Nonterminal::TIGER_PROGRAM,         // NOLINT
					{Symbol::Terminal::LET},                    // NOLINT
					{Symbol::Action::InitializeScope,           // NOLINT
					 Symbol::Terminal::LET,                     // NOLINT
					 Symbol::Nonterminal::DECLARATION_SEGMENT,  // NOLINT
					 Symbol::Terminal::IN,                      // NOLINT
					 Symbol::Action::MakeMainLabel,             // NOLINT
					 Symbol::Nonterminal::STAT_SEQ,             // NOLINT
					 Symbol::Terminal::END,                     // NOLINT
					 Symbol::Action::FinalizeScope});           // NOLINT
	...				
	```

	The next terminals are inside `std::vector<int> &terminals`, their grammar rules **include action symbol** are inside the third parameter `std::vector<int> &expand_rule` in `addToParseTable`.

	In general, combining `addToParseTable` and hand-written parse table, we can embed parse table into program before it starts parsing.

#### Symbol Table

<a href="https://github.com/gangliao/Tiger-Compiler/blob/master/img/symbol_table.png" target="_blank"><img src="https://github.com/gangliao/Tiger-Compiler/blob/master/img/symbol_table.png" width="500" /></a>

Since `let` statements can be nested as per the grammar, **Scoping-sensitive** data structure is required to
store the different levels' symbol table. For convenience and simplicity, we create an global data structure
`g_SymbolTable`: `int` is the current scoping level and `SymbolTablePtr` is a c++11 shared ptr to the corresponding symbol table.

```c++
/// global symbol table <level, symbol table>
std::unordered_map<int, SymbolTablePtr> g_SymbolTable;
```


#### Semantic Checking

#### Intermediate Code

