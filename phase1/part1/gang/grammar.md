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

<type-declaration-list> -> <type-declaration> <type-declaration-list> | NULL
<var-declaration-list> -> <var-declaration> <var-declaration-list> | NULL
<funct-declaration-list> -> <funct-declaration> <funct-declaration-list> | NULL

# type-declaration
<type-declaration> -> type <id> = <type>;
<type> -> <type-id>
<type> -> array [INTLIT] of <type-id>
<type> -> id

<type-id> -> int | float

# var-declaration
<var-declaration> -> var <id-list> : <type> <optional-init>;
<id-list> -> id, <id-list> | id
<optional-list> -> := <const> | NULL

# funct-declaration
<funct-declaration> -> function id (<param-list>) <ret-type> begin <stat-seq> end;
<param-list> -> <param> <param-list-tail> | NULL
<param-list-tail> -> , <param> <param-list-tail> | NULL
<ret-type> -> : <type> | NULL
<param> -> id : <type>
```

