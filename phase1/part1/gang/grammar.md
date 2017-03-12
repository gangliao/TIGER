## Hand-modified Tiger grammer in appropriate LL(k) grammar form

Approach:

```
naive grammar
-> operator precedences and left associate
-> remove left recursion and perform left factoring
-> parse table
-> write parse code
```

### Grammar

```bash
# tiger-program
1: <tiger-program> -> let <declaration-segment> in <stat-seq> end

# declaration-segment
2: <declaration-segment> -> <type-declaration-list> <var-declaration-list> <funct-declaration-list>

3: <type-declaration-list> -> <type-declaration> <type-declaration-list>
4: <type-declaration-list> -> NULL
5: <var-declaration-list> -> <var-declaration> <var-declaration-list>
6: <var-declaration-list> -> NULL
7: <funct-declaration-list> -> <funct-declaration> <funct-declaration-list>
8: <funct-declaration-list> -> NULL

# type-declaration
9: <type-declaration> -> type id = <type>;
10: <type> -> <type-id>
11: <type> -> array [INTLIT] of <type-id>
12: <type> -> id

13: <type-id> -> int
14: <type-id> -> float

# var-declaration
15: <var-declaration> -> var <id-list> : <type> <optional-init>;
16: <id-list> -> id <id-list-tail>
17: <id-list-tail> -> , id <id-list-tail>
18: <id-list-tail> -> NULL
19: <optional-init> -> := <const>
20: <optional-init> -> NULL

# funct-declaration
21: <funct-declaration> -> function id (<param-list>) <ret-type> begin <stat-seq> end;
22: <param-list> -> <param> <param-list-tail>
23: <param-list> -> NULL
24: <param-list-tail> -> , <param> <param-list-tail>
25: <param-list-tail> -> NULL
26: <ret-type> -> : <type>
27: <ret-type> -> NULL
28: <param> -> id : <type>

# stat-seq
29: <stat-seq> -> <stat> <stat-seq-tail>
30: <stat-seq-tail> -> <stat> <stat-seq-tail>
31: <stat-seq-tail> -> NULL

# stat
32: <stat> -> if <expr> then <stat-seq> <stat-if-tail>
33: <stat-if-tail> -> else <stat-seq> endif;
34: <stat-if-tail> -> endif;

35: <stat> -> id <stat-funct-or-assign>
36: <stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
37: <stat-funct-or-assign> -> (<expr-list>);

38: <stat-assign> -> id <stat-assign-stuff>
39: <stat-assign> -> (<expr>) <stat-assign-tail>
40: <stat-assign> -> <const> <stat-assign-tail>

41: <stat-assign-stuff> -> (<expr-list>)
42: <stat-assign-stuff> -> <lvalue-tail> <stat-assign-tail>

43: <stat-assign-tail> -> <expr-tail>
44: <stat-assign-tail> -> <OR-expr-tail>
45: <stat-assign-tail> -> <AND-expr-tail>
46: <stat-assign-tail> -> <compare-tail>
47: <stat-assign-tail> -> <term-tail>
48: <stat-assign-tail> -> NULL

49: <stat> -> while <expr> do <stat-seq> enddo;
50: <stat> -> for id := <expr> to <expr> do <stat-seq> enddo;


51: <stat> -> break;
52: <stat> -> return <expr>;

53: <stat> -> let <declaration-segment> in <stat-seq> end

# expr
54: <expr> -> <OR-expr> <expr-tail>
55: <expr-tail> -> <OR-op> <OR-expr> <stat-assign-tail>

56: <OR-expr> -> <AND-expr> <OR-expr-tail>
57: <OR-expr-tail> -> <AND-op> <AND-expr> <stat-assign-tail>

58: <AND-expr> -> <compare> <AND-expr-tail>
69: <AND-expr-tail> -> <compare-op> <compare> <stat-assign-tail>

60: <compare> -> <term> <compare-tail>
61: <compare-tail> -> <add-op> <term> <stat-assign-tail>

62: <term> -> <factor> <term-tail>
63: <term-tail> -> <mul-op> <factor> <stat-assign-tail>

64: <factor> -> (<expr>)
65: <factor> -> <const>
66: <factor> -> <lvalue>

# const
67: <const> -> INTLIT
68: <const> -> FLOATLIT

# binary-operator
69: <OR-op> -> |
70: <AND-op> -> &
71: <compare-op> -> <=
72: <compare-op> -> >=
73: <compare-op> -> <
74: <compare-op> -> >
75: <compare-op> -> <>
76: <compare-op> -> =
77: <add-op> -> -
78: <add-op> -> +
79: <mul-op> -> /
80: <mul-op> -> *

81: <expr-list> -> NULL
82: <expr-list> -> <expr> <expr-list-tail>
83: <expr-list-tail> -> , <expr> <expr-list-tail>
84: <expr-list-tail> -> NULL

85: <lvalue> -> id <lvalue-tail>
86: <lvalue-tail> -> [<expr>]
87: <lvalue-tail> -> NULL
```

## First Sets

```bash
First(<tiger-program>) = {let}
First(<declaration-segment>) = {type var function NULL}
First(<type-declaration-list>) = {type NULL}
First(<var-declaration-list>) = {var NULL}
First(<funct-declaration-list>) = {function NULL}
First(<type-declaration>) = {type}
First(<type>) = {int float array id}
First(<type-id>) = {int float}
First(<var-declaration>) = {var}
First(<id-list>) = {id}
First(<id-list-tail>) = {, NULL}
First(<optional-init>) = {:= NULL}
First(<funct-declaration>) = {function}
First(<param-list>) = {id NULL}
First(<param-list-tail>) = {, NULL}
First(<ret-type>) = {: NULL}
First(<param>) = {id}
First(<stat-seq>) = {if id while for break return let}
First(<stat-seq-tail>) = {if id while for break return let NULL}
First(<stat>) = {if id while for break return let}
First(<stat-if-tail>) = {else endif}
First(<stat-funct-or-assign>) = {[ ( :=}
First(<stat-assign>) = {id ( INTLIT FLOATLIT}
First(<stat-assign-stuff>) = {( [ | & <= >= < > <> = - + / * NULL}
First(<stat-assign-tail>) = {| & <= >= < > <> = - + / * NULL}
First(<expr>) = {( INTLIT FLOATLIT id}
First(<expr-tail>) = {| NULL}
First(<OR-expr>) = {( INTLIT FLOATLIT id}
First(<OR-expr-tail>) = {& NULL}
First(<AND-expr>) = {( INTLIT FLOATLIT id}
First(<AND-expr-tail>) = {<= >= < > <> = NULL}
First(<compare>) = {( INTLIT FLOATLIT id}
First(<compare-tail>) = {- + NULL}
First(<term>) = {( INTLIT FLOATLIT id}
First(<term-tail>) = {/ * NULL}
First(<factor>) = {( INTLIT FLOATLIT id}
First(<const>) = {INTLIT FLOATLIT}
First(<OR-op>) = {|}
First(<AND-op>) = {&}
First(<compare-op>) = {<= >= < > <> =}
First(<add-op>) = {- +}
First(<mul-op>) = {/ *}
First(<expr-list>) = {( INTLIT FLOATLIT id NULL}
First(<expr-list-tail>) = {, NULL}
First(<lvalue>) = {id}
First(<lvalue-tail>) = {[ NULL}
```

## Follow Sets

```bash
Follow(<tiger-program>) = {$}
Follow(<declaration-segment>) = {in}
Follow(<type-declaration-list>) = {var function in}
Follow(<var-declaration-list>) = {function in}
Follow(<funct-declaration-list>) = {in}
Follow(<type-declaration>) = {type var function in}
Follow(<type>) = {; := begin , )}
Follow(<type-id>) = {; := begin , )}
Follow(<var-declaration>) = {var function in}
Follow(<id-list>) = {:}
Follow(<id-list-tail>) = {:}
Follow(<optional-init>) = {;}
Follow(<funct-declaration>) = {function in}
Follow(<param-list>) = {)}
Follow(<param-list-tail>) = {)}
Follow(<ret-type>) = {begin}
Follow(<param>) = {, )}
Follow(<stat-seq>) = {end endif else enddo}
Follow(<stat-seq-tail>) = {end endif else enddo}
Follow(<stat>) = {if id while for break return let end endif else enddo}
Follow(<stat-if-tail>) = {if id while for break return let end endif else enddo}
Follow(<stat-funct-or-assign>) = {if id while for break return let end endif else enddo}
Follow(<stat-assign>) = {;}
Follow(<stat-assign-stuff>) = {;}
Follow(<stat-assign-tail>) = {;}
Follow(<expr>) = {then ) do to ; ] ,}
Follow(<expr-tail>) = {then ) do to ; ] ,}
Follow(<OR-expr>) = {| then ) do to ; ] ,}
Follow(<OR-expr-tail>) = {| then ) do to ; ] ,}
Follow(<AND-expr>) = {& | then ) do to ; ] ,}
Follow(<AND-expr-tail>) = {& | then ) do to ; ] ,}
Follow(<compare>) = {<= >= < > <> = & | then ) do to ; ] ,}
Follow(<compare-tail>) = {<= >= < > <> = & | then ) do to ; ] ,}
Follow(<term>) = {- + <= >= < > <> = & | then ) do to ; ] ,}
Follow(<term-tail>) = {- + <= >= < > <> = & | then ) do to ; ] ,}
Follow(<factor>) = {/ * - + <= >= < > <> = & | then ) do to ; ] ,}
Follow(<const>) = {; / * - + <= >= < > <> = & | then ) do to ; ] ,}
Follow(<OR-op>) = {( INTLIT FLOATLIT id}
Follow(<AND-op>) = {( INTLIT FLOATLIT id}
Follow(<compare-op>) = {( INTLIT FLOATLIT id}
Follow(<add-op>) = {( INTLIT FLOATLIT id}
Follow(<mul-op>) = {( INTLIT FLOATLIT id}
Follow(<expr-list>) = {)}
Follow(<expr-list-tail>) = {)}
Follow(<lvalue>) = {/ * - + <= >= < > <> = & | then ) do to ; ] ,}
Follow(<lvalue-tail>) = {:= / * - + <= >= < > <> = & | then ) do to ; ] ,}
```
