%{


/*****************************************************************************/
/*                                                                           */
/*  CROBOTS                                                                  */
/*                                                                           */
/*  (C) Copyright Tom Poindexter, 1985, all rights reserved.                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include "crobots.h"
#include "compiler.h"

%}

%token IDENTIFIER 
%token CONSTANT 
%token INC_OP 
%token DEC_OP 
%token LEFT_OP 
%token RIGHT_OP 
%token LE_OP 
%token GE_OP 
%token EQ_OP 
%token NE_OP
%token AND_OP 
%token OR_OP 
%token MUL_ASSIGN 
%token DIV_ASSIGN 
%token MOD_ASSIGN 
%token ADD_ASSIGN
%token SUB_ASSIGN 
%token LEFT_ASSIGN 
%token RIGHT_ASSIGN 
%token AND_ASSIGN
%token XOR_ASSIGN 
%token OR_ASSIGN 
%token EXTERN 
%token AUTO 
%token REGISTER
%token INT 
%token LONG 
%token IF 
%token ELSE 
%token WHILE 
%token FOR 
%token RETURN
%token BREAK

%token U_NEGATIVE
%token U_NOT
%token U_ONES

%start file

%right '=' SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%left OR_OP 
%left AND_OP
%left '|'
%left '^'
%left '&'
%left EQ_OP NE_OP
%left '<' '>' LE_OP GE_OP 
%left LEFT_OP RIGHT_OP
%left '+' '-'
%left '*' '/' '%'
%right '!' '~' INC_OP DEC_OP 

%%

primary_expr
	: identifier
		{ /* printf("IDENTIFIER\n"); */
		if ((work = findvar(last_ident,var_tab)) == -1)
		  if ((work = findvar(last_ident,ext_tab)) == -1) {
		    if (findvar(last_ident,func_tab) == -1) {
		      /* printf("\n***undeclared %s***\n",last_ident); */
		      undeclared++;
		    }
		    work = allocvar(last_ident,var_tab);
		  }
		  else
		    work |= EXTERNAL;
		if (!efetch(work))
		  return(1);
		}
	| CONSTANT
		{ /*printf("CONSTANT\n"); */
		 if (!econst(kk))
		   return(1);
		}
	| '(' expr ')'
	;

postfix_expr
	: primary_expr
	| fcall_start argument_expr_list ')'
		{ /* printf("FCALL\n"); */
		popid(func_ident,func_stack,&func_off); /* note ptr to off */
		if ((work = findvar(func_ident,func_tab)) == -1) {
		  /* printf("\n***declared %s***\n",func_ident); */
		  undeclared--; /*function name mistakenly undeclared*/
		  work = allocvar(func_ident,func_tab);
		}
		if(!efcall(work))
		  return(1);
		}
	| fcall_start ')'
		{ /* printf("FCALL\n"); */
		popid(func_ident,func_stack,&func_off); /* note ptr to off */
		if ((work = findvar(func_ident,func_tab)) == -1) {
		  /* printf("\n***declared %s***\n",func_ident); */
		  undeclared--; /*function name mistakenly undeclared*/
		  work = allocvar(func_ident,func_tab);
		}
		if (!efcall(work))
		  return(1);
		}
	| postfix_expr INC_OP
		{ /* printf("POSTFIX-INC\n"); */
		/* this is wrong!  same as infix increment */
		postfix++;
		if (!econst(1L))
		  return(1);
		if ((work = findvar(last_ident,var_tab)) == -1)
		  if ((work = findvar(last_ident,ext_tab)) == -1) {
		    work = allocvar(last_ident,var_tab);
		  }
		  else
		    work |= EXTERNAL;
		if(!estore(work,ADD_ASSIGN))
		  return(1);
		}
	| postfix_expr DEC_OP
		{ /* printf("POSTFIX-DEC\n"); */
		/* this is wrong!  same as infix decrement */
		postfix++;
		if (!econst(1L))
		  return(1);
		if ((work = findvar(last_ident,var_tab)) == -1)
		  if ((work = findvar(last_ident,ext_tab)) == -1) {
		    work = allocvar(last_ident,var_tab);
		  }
		  else
		    work |= EXTERNAL;
		if (!estore(work,SUB_ASSIGN))
		  return(1);
		}
	;

fcall_start
	: postfix_expr '('
		{ /* printf("FCALL-START\n"); */
		stackid(last_ident,func_stack,&func_off); /* note ptr to off */
		if (!eframe())
		  return(1);
		}
	;

argument_expr_list
	: assignment_expr
	| argument_expr_list ',' assignment_expr
	;

unary_expr
	: postfix_expr
	| INC_OP unary_expr
		{ /* printf("INFIX-INC\n"); */
		if (!econst(1L))
		  return(1);
		if ((work = findvar(last_ident,var_tab)) == -1)
		  if ((work = findvar(last_ident,ext_tab)) == -1) {
		    work = allocvar(last_ident,var_tab);
		  }
		  else
		    work |= EXTERNAL;
		if (!estore(work,ADD_ASSIGN))
		  return(1);
		}
	| DEC_OP unary_expr
		{ /* printf("INFIX-DEC\n"); */
		if (!econst(1L))
		  return(1);
		if ((work = findvar(last_ident,var_tab)) == -1)
		  if ((work = findvar(last_ident,ext_tab)) == -1) {
		    work = allocvar(last_ident,var_tab);
		  }
		  else
		    work |= EXTERNAL;
		if (!estore(work,SUB_ASSIGN))
		  return(1);
		}
	| unary_operator cast_expr
		{ /* printf("UNARY-OP\n"); */
		/* note special tokens defined only to pass to interpreter */
		un_op = *(op_stack + op_off);
		op_off--;
		if (un_op == '-') {
		  if (!econst(0L))
		    return(1);
		  if (!ebinop(U_NEGATIVE))
		    return(1);
		} else if (un_op == '!') {
		  if (!econst(0L))
		    return(1);
		  if (!ebinop(U_NOT))
		    return(1);
		} else if (un_op == '~') {
		  if (!econst(0L))
		    return(1);
		  if (!ebinop(U_ONES))
		    return(1);
		}
		}
	;

unary_operator
	: '-'
		{ /* printf("UNARY-OP\n"); */
		op_off++;
		*(op_stack + op_off) = '-';
		}
	| '!'
		{ 
		op_off++;
		*(op_stack + op_off) = '!';
		}
	| '~'
		{ 
		op_off++;
		*(op_stack + op_off) = '~';
		}
	;

cast_expr
	: unary_expr
	| '(' type_name ')' cast_expr
	;

multiplicative_expr
	: cast_expr
	| multiplicative_expr '*' cast_expr
		{ /* printf("MULTIPLY\n"); */
		if (!ebinop('*'))
		  return(1);
		}
	| multiplicative_expr '/' cast_expr
		{ /*printf("DIVIDE\n"); */
		if (!ebinop('/'))
		  return(1);
		}
	| multiplicative_expr '%' cast_expr
		{ /* printf("MOD\n"); */
		if (!ebinop('%'))
		  return(1);
		}
	;

additive_expr
	: multiplicative_expr
	| additive_expr '+' multiplicative_expr
		{ /* printf("ADD\n"); */
		if (!ebinop('+'))
		  return(1);
		}
	| additive_expr '-' multiplicative_expr
		{ /* printf("SUBTRACT\n"); */
		if (!ebinop('-'))
		  return(1);
		}
	;

shift_expr
	: additive_expr
	| shift_expr LEFT_OP additive_expr
		{ /* printf("SHIFT-LEFT\n"); */
		if (!ebinop(LEFT_OP))
		  return(1);
		}
	| shift_expr RIGHT_OP additive_expr
		{ /* printf("SHIFT-RIGHT\n"); */
		if (!ebinop(RIGHT_OP))
		  return(1);
		}
	;

relational_expr
	: shift_expr
	| relational_expr '<' shift_expr
		{ /* printf("LESS-THAN\n"); */
		if (!ebinop('<'))
		  return(1);
		}
	| relational_expr '>' shift_expr
		{ /* printf("GREATER-THAN\n"); */
		if (!ebinop('>'))
		  return(1);
		}
	| relational_expr LE_OP shift_expr
		{ /*printf("LESS-EQUAL\n"); */
		if (!ebinop(LE_OP))
		  return(1);
		}
	| relational_expr GE_OP shift_expr
		{ /* printf("GREATER-EQUAL\n"); */
		if (!ebinop(GE_OP))
		  return(1);
		}
	;

equality_expr
	: relational_expr
	| equality_expr EQ_OP relational_expr
		{ /* printf("EQUAL\n"); */
		if (!ebinop(EQ_OP))
		  return(1);
		}
	| equality_expr NE_OP relational_expr
		{ /* printf("NOT-EQUAL\n"); */
		if (!ebinop(NE_OP))
		  return(1);
		}
	;

and_expr
	: equality_expr
	| and_expr '&' equality_expr
		{ /* printf("AND\n"); */
		if (!ebinop('&'))
		  return(1);
		}
	;

exclusive_or_expr
	: and_expr
	| exclusive_or_expr '^' and_expr
		{ /* printf("EXCLUSIVE-OR\n"); */
		if (!ebinop('^'))
		  return(1);
		}
	;

inclusive_or_expr
	: exclusive_or_expr
	| inclusive_or_expr '|' exclusive_or_expr
		{ /* printf("INCLUSIVE-OR\n"); */
		if (!ebinop('|'))
		  return(1);
		}
	;

logical_and_expr
	: inclusive_or_expr
	| logical_and_expr AND_OP inclusive_or_expr
		{ /* printf("LOGICAL-AND\n"); */
		if (!ebinop(AND_OP))
		  return(1);
		}
	;

logical_or_expr
	: logical_and_expr
	| logical_or_expr OR_OP logical_and_expr
		{ /* printf("LOGICAL-OR\n"); */
		if (!ebinop(OR_OP))
		  return(1);
		}
	;

conditional_expr
	: logical_or_expr
	;

assignment_expr
	: conditional_expr
	| assignment_lval assignment_expr
		{ /* printf("ASSIGNMENT\n"); */
		/* func_ident used as temp storage */
		popid(func_ident,var_stack,&var_off); /* note ptr to off */
		if ((work = findvar(func_ident,var_tab)) == -1)
		  if ((work = findvar(func_ident,ext_tab)) == -1) {
		    work = allocvar(func_ident,var_tab);
		  }
		  else
		    work |= EXTERNAL;
		if (!estore(work,*(op_stack + op_off)))
		  return(1);
		op_off--;
		}
	;

assignment_lval
	: unary_expr assignment_operator
		{ /* printf("ASSIGNMENT-LVAL\n"); */
		stackid(last_ident,var_stack,&var_off); /* note ptr to off */
		op_off++;
		*(op_stack + op_off) = work;
		}
	;

assignment_operator
	: '='
		{ work =  '=';}
	| MUL_ASSIGN
		{ work = MUL_ASSIGN;}
	| DIV_ASSIGN
		{ work = DIV_ASSIGN;}
	| MOD_ASSIGN
		{ work = MOD_ASSIGN;}
	| ADD_ASSIGN
		{ work = ADD_ASSIGN;}
	| SUB_ASSIGN
		{ work = SUB_ASSIGN;}
	| LEFT_ASSIGN
		{ work = LEFT_ASSIGN;}
	| RIGHT_ASSIGN
		{ work = RIGHT_ASSIGN;}
	| AND_ASSIGN
		{ work = AND_ASSIGN;}
	| XOR_ASSIGN
		{ work = XOR_ASSIGN;}
	| OR_ASSIGN
		{ work = OR_ASSIGN;}
	;

expr
	: assignment_expr
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator
	| declarator '=' initializer
		{ /* printf("INITIALIZER\n"); */
		fprintf(f_out,"\n**Warning** unsupported initializer\n");
		/* get rid of constant placed on stack */
		if (!echop())
		  return(1);
		}
	;

storage_class_specifier
	: EXTERN
	| AUTO
	| REGISTER
	;

type_specifier
	: INT
	| LONG
	;

declarator
	: declarator2
	;

declarator2
	: identifier
		{ /* printf("VARIABLE-DECLARE\n"); */
		if (in_func) {
		  if (findvar(last_ident,var_tab) == -1)
		    allocvar(last_ident,var_tab);
		}
		else {
		  if (findvar(last_ident,ext_tab) == -1)
		    allocvar(last_ident,ext_tab);
		}
		}
	| func_start ')'
		{ /* printf("FUNCTION-DECLARE\n"); */
		if (new_func() == -1)
		  return (1); /* exit the parser */
		}
	| func_start parameter_declaration_list ')'
		{ /* printf("FUNCTION-DECLARE\n"); */
		if (new_func() == -1)
		  return (1); /* exit the parser */
		}
	;

func_start
	: declarator2 '('
		{ /* printf("FUNCTION-DEF-START\n"); */
		strcpy(func_ident,last_ident);
		}
	;

type_specifier_list
	: type_specifier
	| type_specifier_list type_specifier
	;

parameter_identifier
	: identifier
		{ /* printf("PARAMETER-DECLARE\n"); */
		allocvar(last_ident,var_tab);
		num_parm++;
		}
	;

parameter_declaration_list
	: parameter_identifier
	| parameter_declaration_list ',' parameter_identifier
	;

parameter_type_list
	: parameter_list
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: type_specifier_list declarator
	| type_name
	;

type_name
	: type_specifier_list
	| type_specifier_list abstract_declarator
	;

abstract_declarator
	: abstract_declarator2
	;

abstract_declarator2
	: '(' abstract_declarator ')'
	| '(' ')'
	| '(' parameter_type_list ')'
	| abstract_declarator2 '(' ')'
	| abstract_declarator2 '(' parameter_type_list ')'
	;

initializer
	: assignment_expr
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}'
	| '{' declaration_list '}'
	| '{' declaration_list statement_list '}'
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

statement_list
	: statement
	| statement_list statement
	;

expression_statement
	: ';'
	| expr ';'
		{ /* printf("CHOP\n"); */
		if (!echop())
		  return(1);
		}
	;

selection_statement
	: selection1_statement
	| selection2_statement
	;

selection1_statement
	: if_clause statement
		{ /* printf("IF-THEN\n"); */
		else_part();
		close_if();
		}
	;

selection2_statement
	: if_clause statement else_clause statement
		{ /* printf("IF-THEN-ELSE\n"); */
		close_if();
		}
	;

if_clause
	: IF '(' expr ')'
		{ /* printf("IF-CLAUSE\n"); */
		if (!new_if())
		  return (1); /* exit parser */
		}
	;

else_clause
	: ELSE
		{ /* printf("ELSE-CLAUSE\n"); */
		else_part();
		}
	;

iteration_statement
	: while_clause statement
		{ /* printf("WHILE\n"); */
		close_while();
		}
	;

while_token
	: WHILE
		{ /* printf("WHILE-TOKEN\n"); */
		if (!new_while())
		  return (1);  /* exit the parser */
		}
	;

while_clause
	: while_token '(' expr ')'
		{ /* printf("WHILE-CLAUSE\n"); */
		while_expr();
		}
	;

jump_statement
	: BREAK ';'
		{ /* printf("BREAK\n"); */
		/* breaks can be handled by building a instruct chain */
		/* as part of the while_nest structures and patching them */
		/* on while_close.  maybe later */
		fprintf(f_out,"\n**Warning** unsupported break\n");
		}
	| RETURN ';'
		{ /* printf("RETURN-NOEXPR\n"); */
		/* all functions must return a value */
		if (!econst(1L))
		  return(1);
		if (!eretsub())
		  return(1);
		}
	| RETURN expr ';'
		{ /* printf("RETURN\n"); */
		if (!eretsub())
		  return(1);
		}
	;

file
	: external_definition
	| file external_definition
	;

external_definition
	: function_definition
		{ /* printf("FUNCTION-DEFINITION\n"); */
		/* all functions must return a value */
		if (!econst(1L))
		  return(1);
		if (!eretsub())
		  return(1);
		end_func();
		}
	| declaration
		{ /* printf("EXTERNAL-DECLARE\n"); */
		}
	;

function_definition
	: declarator function_body
	| declaration_specifiers declarator function_body
	;

function_body
	: compound_statement
	| declaration_list compound_statement
	;

identifier
	: IDENTIFIER
	;

