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
<stat> -> if <expr> then <stat-seq> <stat-seq-tail>
<stat-seq-tail> -> else <stat-seq> endif;
<stat-if-tail> -> endif;

<stat> -> id <stat-funct-or-assign>
<stat-funct-or-assign> -> <lvalue-tail> := <stat-assign>;
<stat-funct-or-assign> -> (<expr-list>);

<stat> -> while <expr> do <stat-seq> enddo;
<stat> -> for id := <expr> to <expr> do <stat-seq> enddo;


<stat> -> break;
<stat> -> return <expr>;

<stat> -> let <declaration-segment> in <stat-seq> end

# expr
<expr> -> <const>
<expr> -> <lvalue>
<expr> -> <expr> <binary-operator> <expr>
<expr> -> (<expr>)

# const
<const> -> INTLIT
<const> -> FLOATLIT

# binary-operator
<orop> -> |
<andop> -> &
<compareop> -> <=
<compareop> -> >=
<compareop> -> <
<compareop> -> >
<compareop> -> <>
<compareop> -> =
<addop> -> -
<addop> -> +
<mulop> -> /
<mulop> -> *

<expr-list> -> NULL
<expr-list> -> <expr> <expr-list-tail>
<expr-list-tail> -> , <expr><expr-list-tail>
<expr-list-tail> -> NULL

<lvalue> -> id <lvalue-tail>
<lvalue-tail> -> [<expr>]
<lvalue-tail> -> NULL
```

