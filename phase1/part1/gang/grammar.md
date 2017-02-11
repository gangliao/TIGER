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
<tiger-program> -> let <declaration-segment> in <stat-seq> end

# declaration-segment
<declaration-segment> -> <type-declaration-list> <var-declaration-list> <funct-declaration-list>

<type-declaration-list> -> <type-declaration> <type-declaration-list>
<type-declaration-list> -> NULL
<var-declaration-list> -> <var-declaration> <var-declaration-list>
<var-declaration-list> -> NULL
<funct-declaration-list> -> <funct-declaration> <funct-declaration-list>
<funct-declaration-list> -> NULL 

# type-declaration
<type-declaration> -> type id := <type>;
<type> -> <type-id>
<type> -> array [INTLIT] of <type-id>
<type> -> id

<type-id> -> int | float

# var-declaration
<var-declaration> -> var <id-list> : <type> <optional-init>;
<id-list> -> id <id-list-tail>
<id-list-tail> -> , id <id-list-tail>
<id-list-tail> -> NULL
<optional-init> -> := <const>
<optional-init> -> NULL

# funct-declaration
<funct-declaration> -> function id (<param-list>) <ret-type> begin <stat-seq> end;
<param-list> -> <param> <param-list-tail>
<param-list> -> NULL
<param-list-tail> -> , <param> <param-list-tail>
<param-list-tail> -> NULL
<ret-type> -> : <type>
<ret-type> -> : NULL
<param> -> id : <type>

# stat-seq
<stat-seq> -> <stat> <stat-seq-tail>
<stat-seq-tail> -> <stat> <stat-seq-tail>
<stat-seq-tail> -> NULL

# stat
<stat> -> if <expr> then <stat-seq> <stat-if-tail>
<stat-if-tail> -> else <stat-seq> endif;
<stat-if-tail> -> endif;

<stat> -> id <stat-funct-or-assign>
<stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
<stat-funct-or-assign> -> (<expr-list>);

<stat-assign> -> id <stat-assign-stuff>
<stat-assign> -> (<expr>) <stat-assign-tail>
<stat-assign> -> <const> <stat-assign-tail>

<stat-assign-stuff> -> (<expr-list>)
<stat-assign-stuff> -> <lvalue-tail> <stat-assign-tail>

<stat-assign-tail> -> <expr-tail>
<stat-assign-tail> -> <OR-tail>
<stat-assign-tail> -> <AND-tail>
<stat-assign-tail> -> <compare-tail>
<stat-assign-tail> -> <term-tail>

<stat> -> while <expr> do <stat-seq> enddo;
<stat> -> for id := <expr> to <expr> do <stat-seq> enddo;


<stat> -> break;
<stat> -> return <expr>;

<stat> -> let <declaration-segment> in <stat-seq> end

# expr
<expr> -> <OR-expr> <expr-tail>
<expr-tail> -> <OR-op> <OR-expr> <expr-tail>
<expr-tail> -> NULL

<OR-expr> -> <AND-expr> <OR-expr-tail>
<OR-expr-tail> -> <AND-op> <AND-expr> <OR-expr-tail>
<OR-expr-tail> -> NULL

<AND-expr> -> <compare> <AND-expr-tail>
<AND-expr-tail> -> <compare-op> <compare> <AND-expr-tail>
<AND-expr-tail> -> NULL

<compare> -> <term> <compare-tail>
<compare-tail> -> <add-op> <term> <compare-tail>
<compare-tail> -> NULL

<term> -> <factor> <term-tail>
<term-tail> -> <mul-op> <factor> <term-tail>
<term-tail> -> NULL

<factor> -> (<expr>)
<factor> -> <const>
<factor> -> <lvalue>

# const
<const> -> INTLIT
<const> -> FLOATLIT

# binary-operator
<OR-op> -> |
<AND-op> -> &
<compare-op> -> <=
<compare-op> -> >=
<compare-op> -> <
<compare-op> -> >
<compare-op> -> <>
<compare-op> -> =
<add-op> -> -
<add-op> -> +
<mul-op> -> /
<mul-op> -> *

<expr-list> -> NULL
<expr-list> -> <expr> <expr-list-tail>
<expr-list-tail> -> , <expr> <expr-list-tail>
<expr-list-tail> -> NULL

<lvalue> -> id <lvalue-tail>
<lvalue-tail> -> [<expr>]
<lvalue-tail> -> NULL
```

## First Sets

```bash
First(<tiger-program>) = {let}
First(<declaration-segment>) = {let type var function NULL}
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
First(<ret-type>) = {:}
First(<param>) = {id}
First(<stat-seq>) = {if id while for break return let}
First(<stat-seq-tail>) = {if id while for break return let NULL}
First(<stat>) = {if id while for break return let}
First(<stat-if-tail>) = {else endif}
First(<stat-funct-or-assign>) = {[ ( NULL}
First(<stat-assign>) = {id ( INTLIT FLOATLIT}
First(<stat-assign-stuff>) = {( [ | & <= >= < > <> = + / * NULL}
First(<stat-assign-tail>) = {| & <= >= < > <> = + / * NULL}
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
Follow(<type-declaration-list>) = {type var function in}
Follow(<var-declaration-list>) = {var function in}
Follow(<funct-declaration-list>) = {function in}
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
Follow(<param>) = {,}
Follow(<stat-seq>) = {end endif else enddo}
Follow(<stat-seq-tail>) = {end endif else enddo}
Follow(<stat>) = {if id while for break return let}
Follow(<stat-if-tail>) = {if id while for break return let}
Follow(<stat-funct-or-assign>) = {if id while for break return let}
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
Follow(<const>) = {; | & <= >= < > <> = + / *}
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
