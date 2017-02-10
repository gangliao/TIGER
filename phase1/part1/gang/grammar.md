## Hand-modified Tiger grammer in appropriate LL(k) grammar form

### Grammar

```bash
<tiger-program> -> let <declaration-segment> in <stat-seq> end
<declaration-segment> -> <type-declaration-list> <var-declaration-list> <funct-declaration-list>
<type-declaration-list> -> <type-declaration> <type-declaration-list> | NULL
<var-declaration-list> -> <var-declaration> <var-declaration-list> | NULL
<funct-declaration-list> -> <funct-declaration> <funct-declaration-list> | NULL
```

