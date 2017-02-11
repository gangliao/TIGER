## Parse Table

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
9: <type-declaration> -> type id := <type>;
10: <type> -> <type-id>
11: <type> -> array [INTLIT] of <type-id>
12: <type> -> id

13: <type-id> -> int | float

# var-declaration
14: <var-declaration> -> var <id-list> : <type> <optional-init>;
15: <id-list> -> id <id-list-tail>
16: <id-list-tail> -> , id <id-list-tail>
17: <id-list-tail> -> NULL
18: <optional-init> -> := <const>
19: <optional-init> -> NULL

# funct-declaration
20: <funct-declaration> -> function id (<param-list>) <ret-type> begin <stat-seq> end;
21: <param-list> -> <param> <param-list-tail>
22: <param-list> -> NULL
23: <param-list-tail> -> , <param> <param-list-tail>
24: <param-list-tail> -> NULL
25: <ret-type> -> : <type>
26: <ret-type> -> : NULL
27: <param> -> id : <type>

# stat-seq
28: <stat-seq> -> <stat> <stat-seq-tail>
29: <stat-seq-tail> -> <stat> <stat-seq-tail>
30: <stat-seq-tail> -> NULL

# stat
31: <stat> -> if <expr> then <stat-seq> <stat-if-tail>
32: <stat-if-tail> -> else <stat-seq> endif;
33: <stat-if-tail> -> endif;

34: <stat> -> id <stat-funct-or-assign>
35: <stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
36: <stat-funct-or-assign> -> (<expr-list>);

37: <stat-assign> -> id <stat-assign-stuff>
38: <stat-assign> -> (<expr>) <stat-assign-tail>
39: <stat-assign> -> <const> <stat-assign-tail>

40: <stat-assign-stuff> -> (<expr-list>)
41: <stat-assign-stuff> -> <lvalue-tail> <stat-assign-tail>

42: <stat-assign-tail> -> <expr-tail>
43: <stat-assign-tail> -> <OR-tail>
44: <stat-assign-tail> -> <AND-tail>
45: <stat-assign-tail> -> <compare-tail>
46: <stat-assign-tail> -> <term-tail>

47: <stat> -> while <expr> do <stat-seq> enddo;
48: <stat> -> for id := <expr> to <expr> do <stat-seq> enddo;


49: <stat> -> break;
50: <stat> -> return <expr>;

51: <stat> -> let <declaration-segment> in <stat-seq> end

# expr
52: <expr> -> <OR-expr> <expr-tail>
53: <expr-tail> -> <OR-op> <OR-expr> <expr-tail>
54: <expr-tail> -> NULL

55: <OR-expr> -> <AND-expr> <OR-expr-tail>
56: <OR-expr-tail> -> <AND-op> <AND-expr> <OR-expr-tail>
57: <OR-expr-tail> -> NULL

58: <AND-expr> -> <compare> <AND-expr-tail>
59: <AND-expr-tail> -> <compare-op> <compare> <AND-expr-tail>
60: <AND-expr-tail> -> NULL

61: <compare> -> <term> <compare-tail>
62: <compare-tail> -> <add-op> <term> <compare-tail>
63: <compare-tail> -> NULL

64: <term> -> <factor> <term-tail>
65: <term-tail> -> <mul-op> <factor> <term-tail>
66: <term-tail> -> NULL

67: <factor> -> (<expr>)
68: <factor> -> <const>
69: <factor> -> <lvalue>

# const
70: <const> -> INTLIT
71: <const> -> FLOATLIT

# binary-operator
72: <OR-op> -> |
73: <AND-op> -> &
74: <compare-op> -> <=
75: <compare-op> -> >=
76: <compare-op> -> <
77: <compare-op> -> >
78: <compare-op> -> <>
79: <compare-op> -> =
80: <add-op> -> -
81: <add-op> -> +
82: <mul-op> -> /
83: <mul-op> -> *

84: <expr-list> -> NULL
85: <expr-list> -> <expr> <expr-list-tail>
86: <expr-list-tail> -> , <expr> <expr-list-tail>
87: <expr-list-tail> -> NULL

88: <lvalue> -> id <lvalue-tail>
89: <lvalue-tail> -> [<expr>]
90: <lvalue-tail> -> NULL
```


```bash

|   M[N][T]             |array|break| do | else | end | for |function| if | in | let | nil | of | then | to | type | int |float| var |while|endif|begin| end | enddo |return| , | : | ; | ( | ) | [ | ] | { | } | + | - | * | / | = | < | > | < | > | <= | >= | & | | | := | 
|-----------------------|-----|-----|----|------|-----|-----|--------|----|----|-----|-----|----|------|----|------|-----|-----|-----|-----|-----|-----|-----|-------|------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|----|----|---|---|----| 
| <tiger-program>       |     |

```