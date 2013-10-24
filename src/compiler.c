

/*****************************************************************************/
/*                                                                           */
/*  CROBOTS                                                                  */
/*                                                                           */
/*  (C) Copyright Tom Poindexter, 1985, all rights reserved.                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/* compiler.c - compiler routines in support of grammar.c */

#include <stdio.h>

#include "crobots.h"
/* EXT causes externals to be declared without extern keyword in compiler.h */
/* that is, to give them actual storage (some compilers require this method) */
#define EXT 1
#include "compiler.h"
#include "tokens.h"

/* yyerror - simple error message on parser failure */

yyerror(s)
char *s;
{
  int i;
  r_flag = 1;
  fprintf(f_out,"\n");
  for (i = 1; i < column; i++)
    fprintf(f_out," ");
  fprintf(f_out,"^\n");
  fprintf(f_out,"** Error ** %s",s);
}


char *malloc();


/* init_comp - initializes the compiler for one file */
/* assumes robot structure allocated and pointed to by cur->robot */

init_comp() 
{
  register int i;

  /* these tables freed after the entire file is compiled */
  ifs = (struct fix_if *) malloc(sizeof (struct fix_if) * NESTLEVEL);
  if_nest = 0;
  whiles = (struct fix_while *) malloc(sizeof (struct fix_while) * NESTLEVEL);
  while_nest = 0;

  /* compiler flags */
  column = 0;
  num_parm = 0;
  num_instr = 0;
  in_func = 0;
  r_flag = 0;  /* global error flag */
  undeclared = 0;
  postfix = 0;

  strncpy(last_ident,"",ILEN);
  strncpy(func_ident,"",ILEN);

  ext_tab = malloc(MAXSYM * ILEN);   /* freed after file */
  var_tab = malloc(MAXSYM * ILEN);   /* freed after function */

  func_tab = malloc(MAXSYM * ILEN);  /* should not be freed, part of robot */

  var_stack = malloc(MAXSYM * ILEN);  /* freed after file */
  var_off = 0;

  func_stack = malloc(MAXSYM * ILEN);  /* freed after file */
  func_off = 0;

  op_stack = (int *) malloc(MAXSYM * sizeof (int)); /* freed after file */
  op_off = 0;

  /* allocate code space in robot, code should not be freed */
  cur_robot->code_list = (struct func *) 0;
  cur_robot->code = (struct instr *) malloc(CODESPACE * sizeof(struct instr));
  instruct = (struct instr *) cur_robot->code;

  /* initialize all tables */
  for (i = 0; i < MAXSYM; i++) {
    *(ext_tab + (i * ILEN)) = '\0';
    *(var_tab + (i * ILEN)) = '\0';
    *(func_tab+ (i * ILEN)) = '\0';
    *(var_stack + (i * ILEN)) = '\0';
    *(func_stack + (i * ILEN)) = '\0';
    *(op_stack + i) = 0;
  }

}


/* reset_comp - resets the compiler for another file */
/* completes the robot structure */

reset_comp() 
{
  int i, j;
  int found = 0;
  int mainfunc = 0;
  struct func *chain;
  int good = 1;
  int ext_size;

  /* check for too many intructions */
  if (num_instr == CODESPACE) {
    fprintf(f_out,"\n** Error ** instruction space exceeded\n");
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**reset_comp**\n\n");
    good = 0;
  }

  /* check func_tab to code_list for missing functions (accept intrinsics) */
  /* this ensures no functions are referenced that are not coded or intrinsic */
  for (i = 0; *(func_tab + (i * ILEN)) != '\0'; i++) {
    found = 0;
    for (chain = cur_robot->code_list; chain != (struct func *) 0; 
	 chain = chain->nextfunc) {
      if (strcmp((func_tab + (i *ILEN)),chain->func_name) == 0) {
	found = 1;
	break;
      }
    }

    /* if not found as a coded function, check the intrinsic table */
    for (j = 0; *intrinsics[j].n != '\0'; j++) {
      if (strcmp((func_tab + (i * ILEN)),intrinsics[j].n) == 0) {
	found = 1;
	break;
      }
    }

    /* make sure that a main was declared */
    if (strcmp("main",(func_tab + (i * ILEN))) == 0)
      mainfunc = 1;

    if (!found) {
      fprintf(f_out,
   "\n** Error ** '%s (%d)' function referenced, but not defined or intrinsic\n",
	(func_tab + (i * ILEN)),i);
      good =0;
      r_flag = 1;
      if (r_debug)
        fprintf(f_out,"\n\n**reset_comp**\n\n");
    }
  }

  if (!mainfunc) {
    fprintf(f_out,"\n** Error ** 'main' not defined\n");
    good = 0;
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**reset_comp**\n\n");
  }

  if (undeclared > 0)
    fprintf(f_out,"\n** Warning ** %d undeclared variables\n",undeclared);

  if (postfix > 0)
    fprintf(f_out,"\n** Warning ** %d postfix operators\n",postfix);

  fprintf(f_out,"\nCode utilization: %d%%   (%d / %d) \n",
	  (int) (((long) num_instr) * 100L / CODESPACE) ,num_instr,CODESPACE);
  fflush(f_out);

  /* clean up compiler tables and flags */
  num_parm = 0;
  num_instr = 0;
  in_func = 0;
  undeclared = 0;
  postfix = 0;
  ext_size = poolsize(ext_tab);
  free(ifs);
  free(whiles);
  free(ext_tab);
  free(var_tab);
  free(var_stack);
  free(func_stack);
  free(op_stack);

  /* if compile was ok, then allocate external pool, stack, and robot flags */
  if (good) {
    cur_robot->ext_count = ext_size;
    cur_robot->external = (long *) malloc(cur_robot->ext_count * sizeof(long));
    cur_robot->stackbase = (long *) malloc(DATASPACE * sizeof(long));
    cur_robot->stackend = cur_robot->stackbase + DATASPACE;
    cur_robot->funcs = func_tab;
    cur_robot->status = ACTIVE;
    instruct->ins_type = NOP;
  }
  return (good);
}


/* new_func - reset the compiler for a new function within the same file */

new_func()
{
  register int i;

  /* make sure name is not an intrinsic */
  for (i = 0; *(intrinsics[i].n) != '\0'; i++) {
    if (strcmp(intrinsics[i].n,func_ident) == 0) {
     fprintf(f_out,"\n** Error ** '%s' function definition same as intrinsic\n",
	      func_ident);
      r_flag = 1;
      if (r_debug)
        fprintf(f_out,"\n\n**new_func**\n\n");
      return (0);
    }
  }

  /* func name ok, insert a new function header */
  new = (struct func *) malloc(sizeof (struct func)); /* never freed */
  new->nextfunc = cur_robot->code_list;		/* link in */
  cur_robot->code_list = new;			/*  "    " */
  strcpy(new->func_name,func_ident);		/* copy name */
  new->first = instruct;			/* current instruct is start */
  new->var_count = 0; 				/* filled-in later */
  new->par_count = num_parm;			/* number of parms */
  in_func = 1;
  if (findvar(func_ident,func_tab) == -1)	/* add name to function table */
    allocvar(func_ident,func_tab);
  return (1);

}


/* end_func - cleanup the end of a function */

end_func() 
{
  register int i;

  /* fill in the space required by local variables into function header */
  cur_robot->code_list->var_count = poolsize(var_tab);
  num_parm = 0;
  in_func = 0;
  func_off = 0;
  op_off = 0;
  var_off = 0;

  if (r_debug) {
    fprintf(f_out,"\n\nFunction: %s\n\n  Local symbol table:\n",new->func_name);
    dumpoff(var_tab);
    fprintf(f_out,"\n\n\nExternal symbol table:\n");
    dumpoff(ext_tab);
    fprintf(f_out,"\n\nFunction symbol table:\n");
    dumpoff(func_tab);
    fprintf(f_out,"\n\n Generated code:\n");
    decompile(cur_robot->code_list->first);
  }


  /* initialize local variable table again */
  for (i = 0; i < NESTLEVEL; i++) {
    *(var_tab + (i * ILEN)) = '\0';
  }

}


/* allocvar - allocates a variable in a pool, returns offset */

allocvar(s,pool) 

char s[];
char *pool;
{
  register int i;

  for (i = 0; i < MAXSYM; i++) {
    if (*(pool + (i * ILEN)) == '\0') {		/* pool is treated as two- */
      strcpy(pool + (i * ILEN),s);		/* dim array, by adding ints */
      return (i);				/* to pointers; see K&R */
    }
  }
  r_flag = 1;
  if (r_debug)
    fprintf(f_out,"\n\n**alloc_var**\n\n");
  fprintf(f_out,"\n\n** Error ** symbol pool exceeded\n");
  return (-1);
}



/* findvar - returns offset of variable in a pool */

findvar(s,pool)

char s[];
char *pool;
{
  register int i;

  for (i = 0; i < MAXSYM; i++) {
    if (strcmp(pool + (i * ILEN),s) == 0)
      return (i);
  }  
   
  return (-1);
}


/* stackid - stacks an identifier, note pointer to stack offset */

stackid(id,stack,ptr)

char id[];
char *stack;
int *ptr;
{
  if (*ptr < MAXSYM - 1) {
    (*ptr)++;				/* the ptr itself is incremented */
    strcpy(stack + (*ptr * ILEN),id);
    return (1);
  } else {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**stackid**\n\n");
    return (-1);
  }
}


/* popid - unstacks an identifier, note pointer to stack offset */

popid(id,stack,ptr)

char id[];
char *stack;
int *ptr;
{
  if (*ptr > 0) {
    strcpy(id,stack + (*ptr * ILEN));
    (*ptr)--;
    return (1);
  } else {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**popid**\n\n");
    return (-1);
  }
}




/* poolsize - returns the size of a pool */

poolsize(pool)

char *pool;
{
  register int i;

  /* count the number of items */
  for (i = 0; i < MAXSYM; i++) {
    if (*(pool + (i * ILEN)) == '\0')
      return (i);
  }  
   
  r_flag = 1;
  if (r_debug)
    fprintf(f_out,"\n\n**poolsize**\n\n");
  return (-1);
}


/* dumpoff - print a table of names and offsets in a symbol pool */

dumpoff(pool)

char *pool;
{
  register int i;
  int count = 0;

  for (i = 0; i < MAXSYM; i++) {
    if (*(pool + (i * ILEN)) == '\0')
      return;
    fprintf(f_out,"%4d : %-8s  ",i,pool + (i * ILEN));
    if (++count == 4) {
      fprintf(f_out,"\n");
      count = 0;
    }
  }
}

/* all code emit functions check for code space availability, and increments */
/* the current instruction pointer within the code space */

/* efetch - emit a fetch instruction */

efetch(offset)

int offset;
{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**efetch**\n\n");
    return (0);
  }
  instruct->ins_type = FETCH;
  instruct->u.var1 = offset;
  last_ins = instruct++;
  return (1);
}


/* estore - emit a store instruction */

estore(offset, operator)

int offset;
int operator;
{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**estore*\n\n");
    return (0);
  }
  instruct->ins_type = STORE;
  instruct->u.a.var2 = offset;
  instruct->u.a.a_op = operator;
  last_ins = instruct++;
  return (1);
}


/* econst - emit a constant instruction */

econst(c)

long c;
{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
printf("\n\n**econst*\n\n");
    return (0);
  }
  instruct->ins_type = CONST;
  instruct->u.k = c;
  last_ins = instruct++;
  return (1);
}


/* ebinop - emit a binop instruction */

ebinop(c)

int c;
{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**ebinop**\n\n");
    return (0);
  }
  instruct->ins_type = BINOP;
  instruct->u.var1 = c;
  last_ins = instruct++;
  return (1);
}


/* efcall - emit a fcall instruction */

efcall (c)

int c;
{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**efcall**\n\n");
    return (0);
  }
  instruct->ins_type = FCALL;
  instruct->u.var1 = c;
  last_ins = instruct++;
  return (1);
}


/* eretsub - emit a retsub instruction */

eretsub()

{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**eretsub**\n\n");
    return (0);
  }
  instruct->ins_type = RETSUB;
  last_ins = instruct++;
  return (1);
}


/* ebranch - emit a  branch instruction */

ebranch()

{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**ebranch**\n\n");
    return (0);
  }
  instruct->ins_type = BRANCH;
  instruct->u.br = (struct instr *) 0;	/* must be fixed later */
  last_ins = instruct++;
  return (1);
}


/* echop - emit a chop instruction */

echop()

{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**echop**\n\n");
    return (0);
  }
  instruct->ins_type = CHOP;
  last_ins = instruct++;
  return (1);
}


/* eframe - emit a stack frame instruction */

eframe()

{
  if (++num_instr == CODESPACE) {
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**eframe**\n\n");
    return (0);
  }
  instruct->ins_type = FRAME;
  last_ins = instruct++;
  return (1);
}


/* new_if - start a nest for an if statement */

new_if()

{
  if (if_nest == NESTLEVEL) {
    fprintf(f_out,"\n** Error ** 'if' nest level exceeded\n");
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**new_if**\n\n");
    return (0);
  }

  if_nest++;
  if (!ebranch())
    return (0);

  /* save the not-true branch instruction address to be fixed later */
  /* this branch jumps to the else part, if any */
  (ifs + if_nest)->fix_false = last_ins;

  return (1);
}


/* else_part - the else part of an if-then-else */

else_part()

{	
  /* setup a unconditional branch around the else part */
  if (!econst(0L))
    return(0);  
  if (!ebranch())
    return (0);

  /* save the else branch instruction address */
  /* this branch jumps around the else part, if any */
  (ifs + if_nest)->fix_true = last_ins;

  /* fix the not-true branch */
  /* the branch instrunction address was saved in new_if() */
  (ifs + if_nest)->fix_false->u.br = instruct;

  return (1);
}


/* close_if - close out an if nest */

close_if()

{
  /* fix the not-else branch saved in else_part() */
  (ifs + if_nest)->fix_true->u.br = instruct;

  if_nest--;
}


/* new_while - start a nest for a new while statement */

new_while()

{
  if (while_nest == NESTLEVEL) {
    fprintf(f_out,"\n** Error ** 'while' nest level exceeded\n");
    r_flag = 1;
    if (r_debug)
      fprintf(f_out,"\n\n**new_while**\n\n");
    return (0);
  }
  while_nest++;

  /* save the target intruction for while-loop expression evaluation */
  (whiles + while_nest)->loop = instruct;

  return (1);
}


/* while_expr - while expression loop fix */

while_expr()

{
  if (!ebranch())
    return (0);

  /* save the branch out of while-loop address to fix later */
  /* this branch jumps around the while-body */
  (whiles + while_nest)->fix_br = last_ins;

  return (1);
}


/* close_while - close out the while nest */

close_while()

{
  /* emit an unconditional branch */
  if (!econst(0L))
    return (0);
  if (!ebranch())
    return (0);

  /* fix the jump back to expression evaluation */
  /* this was saved in new_while() */
  last_ins->u.br = (whiles + while_nest)->loop;

  /* fix the not while branch */
  /* this was saved in while_expr() */
  (whiles + while_nest)->fix_br->u.br = instruct;

  while_nest--;
  return (1);
}


/* decompile - print machine code */

decompile(code)

struct instr *code;
{

  while (code->ins_type != NOP) {
    decinstr(code);
    code++;
  }
}
       


/* decinstr - print one instruct; watch out for pointer to long conversion! */

decinstr(code)

struct instr *code;
{

  fprintf(f_out,"%8ld : ",(long) code);	/* this could be flakey */
  switch (code->ins_type) {
    case FETCH:
      if (code->u.var1 & EXTERNAL) 
	fprintf(f_out,"fetch   %d external\n",code->u.var1 & ~EXTERNAL);
      else
	fprintf(f_out,"fetch   %d local\n",code->u.var1);
      break;
    case STORE:
      if (code->u.a.var2 & EXTERNAL)
	fprintf(f_out,"store   %d external, ",
		code->u.a.var2 & ~EXTERNAL);
      else
	fprintf(f_out,"store   %d local, ",code->u.a.var2);
      printop(code->u.a.a_op);
      fprintf(f_out,"\n");
      break;
    case CONST:
      fprintf(f_out,"const   %ld\n",code->u.k);
      break;
    case BINOP:
      fprintf(f_out,"binop   ");
      printop(code->u.var1);
      fprintf(f_out,"\n");
      break;
    case FCALL:
      fprintf(f_out,"fcall   %d\n",code->u.var1);
      break;
    case RETSUB:
      fprintf(f_out,"retsub\n");
      break;
    case BRANCH:
      fprintf(f_out,"branch  %ld\n",(long) code->u.br); /* more flakiness */
      break;
    case CHOP:
      fprintf(f_out,"chop\n");
      break;
    case FRAME:
      fprintf(f_out,"frame\n");
      break;
    default:
      fprintf(f_out,"ILLEGAL %d\n",code->ins_type);
      return;
  }
}


/* printop - print a binary operation code */

printop(op)

int op;
{

  switch (op) {

    case  '=':
      fprintf(f_out,"=");
      break;

    case  '|':
      fprintf(f_out,"|");
      break;

    case  '^':
      fprintf(f_out,"^");
      break;

    case  '&':
      fprintf(f_out,"&");
      break;

    case  '<':
      fprintf(f_out,"<");
      break;

    case  '>':
      fprintf(f_out,">");
      break;

    case  '+':
      fprintf(f_out,"+");
      break;

    case  '-':
      fprintf(f_out,"-");
      break;

    case  '*':
      fprintf(f_out,"*");
      break;

    case  '/':
      fprintf(f_out,"/");
      break;

    case  '%':
      fprintf(f_out,"%");
      break;

    case  LEFT_OP:
      fprintf(f_out,"<<");
      break;

    case  RIGHT_OP:
      fprintf(f_out,">>");
      break;

    case  LE_OP:
      fprintf(f_out,"<=");
      break;

    case  GE_OP:
      fprintf(f_out,">=");
      break;

    case  EQ_OP:
      fprintf(f_out,"==");
      break;

    case  NE_OP:
      fprintf(f_out,"!=");
      break;

    case  AND_OP:
      fprintf(f_out,"&&");
      break;

    case  OR_OP:
      fprintf(f_out,"||");
      break;

    case  MUL_ASSIGN:
      fprintf(f_out,"*=");
      break;

    case  DIV_ASSIGN:
      fprintf(f_out,"/=");
      break;

    case  MOD_ASSIGN:
      fprintf(f_out,"%=");
      break;

    case  ADD_ASSIGN:
      fprintf(f_out,"+=");
      break;

    case  SUB_ASSIGN:
      fprintf(f_out,"-=");
      break;

    case  LEFT_ASSIGN:
      fprintf(f_out,"<<=");
      break;

    case  RIGHT_ASSIGN:
      fprintf(f_out,">>=");
      break;

    case  AND_ASSIGN:
      fprintf(f_out,"&=");
      break;

    case  XOR_ASSIGN:
      fprintf(f_out,"^=");
      break;

    case  OR_ASSIGN:
      fprintf(f_out,"|=");
      break;

    case  U_NEGATIVE:
      fprintf(f_out,"(-)");
      break;

    case  U_NOT:
      fprintf(f_out,"(!)");
      break;

    case  U_ONES:
      fprintf(f_out,"(~)");
      break;

    default:
      fprintf(f_out,"ILLEGAL %d",op);
      break;

  }


}

