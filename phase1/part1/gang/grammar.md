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
<type-declaration> -> type id = <type>;
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

<AND-expr> -> <compare-expr> <AND-expr-tail>
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
<expr-list-tail> -> , <expr><expr-list-tail>
<expr-list-tail> -> NULL

<lvalue> -> id <lvalue-tail>
<lvalue-tail> -> [<expr>]
<lvalue-tail> -> NULL
```

