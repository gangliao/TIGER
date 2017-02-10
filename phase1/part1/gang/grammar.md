## Hand-modified Tiger grammer in appropriate LL(k) grammar form

### Grammar

<tiger-program> -> let <declaration-segment> in <stat-seq> end
<declaration-segment> -> <type-declaration-list> <var-declaration-list> <funct-declaration-list>
<type-declaration-list> -> <type-declaration> <type-declaration-list> | NULL

