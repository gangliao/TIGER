## This is a markdown file

#Operator precedence

<expr1> -> <expr1> <boolop> <Term1> | <Term1>
boolop ->&||

<Term1>-><Term1> <compareop> <Term2> | <Term2>
compareop  -> <> | = | > |< |>=|<=
	
<Term2>-><Term2> <addsubop> <Term3> | <Term3>
Addsubop -> +|-

<Term3>-> <Term3> <muldivop> <factor>| <factor>
muldivop -> *|\

<factor-> <type> | <const>
	
<Expr> -> <const>|<lvalue> | (<expr1>) | <expr1>

#Remove ambiguity from if-else statement

<stat>-> <matched-stat> | <unmatched-stat>
matched-stat -> if <expr> then <matched-stat> else <matched-stat>endif;
Unmatched-stat-> if <expr> then <stat-seq> endif;| if<expr> then <matched-stmt> else <unmatched-stat> endif;

#Remove left recursion
<expr1> -> <term1> <t_tail1>
<t_tail1> -> <boolop><term1><t_tail1>|e

<term1>-><term2><t_tail2>
<t_tail2>-><compareop><term2><t_tail2>|e

<term2>-><term3><t_tail3>
<t_tail3>-><addsubop><term3><t_tail3>|e

<term3>-><factor><t_tail4>
<t_tail4>-><muldivop><factor><t_tail4>|e

<Expr> -> <expr> <binary-operator> <expr> ???
