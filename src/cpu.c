

/*****************************************************************************/
/*                                                                           */
/*  CROBOTS                                                                  */
/*                                                                           */
/*  (C) Copyright Tom Poindexter, 1985, all rights reserved.                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/* cpu.c - the routines to execute crobot instructions */

#include <stdio.h>
#include "crobots.h"
#include "tokens.h"

/* push - basic stack push mechanism */
/*         depends on cur_robot, set r_flag on overflow */

long push(k)

long k;
{
  /* increment stack and check for collistion into return ptrs */
  if (++cur_robot->stackptr == cur_robot->retptr) {
    r_flag = 1;  /* signal a stack overflow, i.e., collision into returns */
    return(0L);
  }
  *cur_robot->stackptr = k;
  return (k);
}


/* pop - basic stack pop mechanism */
/*         depends on cur_robot, set r_flag on overflow */

long pop()
{
  long v;
  if (cur_robot->stackptr == cur_robot->stackbase) {
    r_flag = 1;  /* signal a stack underflow */
    return (0L);
  }
  v = *cur_robot->stackptr;
  cur_robot->stackptr--;
  return (v);
}


/* cycle - interpret one instruction for current robot */
/*         depends on cur_robot and cur_instr */

/* any errors (stack collision, missing functions, etc) cause the 'main' */
/* function to be restarted, with a clean stack; signal by r_flag = 1 */

cycle()
{
  int j;
  int c;
  int called = 0;
  long value;
  register struct instr *cur_instr;
  struct func *f;
  register char *n;
  struct instr **i;
  long **l;
  long push();
  long pop();


  cur_instr = cur_robot->ip;

  if (r_debug) 
    decinstr(cur_instr);

  switch(cur_instr->ins_type) {

    case FETCH:		/* push a value from a variable pool */

      if (cur_instr->u.var1 & EXTERNAL) 
	push(*(cur_robot->external + (cur_instr->u.var1 & ~EXTERNAL)));
      else
	push(*(cur_robot->local + cur_instr->u.var1));
      cur_robot->ip++;
      break;


    case STORE:		/* store tos in a variable pool */

      binaryop(cur_instr->u.a.a_op);	/* perform assignment operation */
      if (cur_instr->u.a.var2 & EXTERNAL) 
	*(cur_robot->external +(cur_instr->u.a.var2 & ~EXTERNAL)) = push(pop());
      else
	*(cur_robot->local + cur_instr->u.var1) = push(pop());
      cur_robot->ip++;
      break;

      
    case CONST:		/* push a constant */

      push(cur_instr->u.k);
      cur_robot->ip++;
      break;


    case BINOP:		/* do a binary operation */

      binaryop(cur_instr->u.var1);
      cur_robot->ip++;
      break;


    case FCALL:

      n = cur_robot->funcs + (cur_instr->u.var1 * ILEN); /* get function name*/

      /* first, check for an intrinsic call */
      for (j = 0; *intrinsics[j].n != '\0'; j++) {
        if (r_debug)
          printf("\nfunc %s found %s\n",n,intrinsics[j].n);
	if (strcmp(intrinsics[j].n,n) == 0) {
	  (*intrinsics[j].f)();  	/* call the intrinsic function */
	  value = pop(); 		/* get return value */

          /* re-frame stack to ensure we discard all expressions */
          l = (long **) cur_robot->retptr++;
          cur_robot->stackptr = *l;

	  pop();  			/* get rid of bogus function value */
	  push(value);  		/* put return value on stack */
          cur_robot->ip++;
	  called = 1;
	  break;
	}
      }

      if (!called) {
	/* find coded function by name */
	/* search through function headers */
	for (f=cur_robot->code_list; f != (struct func *) 0; f=f->nextfunc) {
	  if (r_debug)
            printf("\nfunc %s found %s\n",n,f->func_name);

	  if (strcmp(f->func_name,n) == 0) {
	    /* save next instruction pointer */
	    if (--cur_robot->retptr == cur_robot->stackptr) {
	      r_flag = 1;
	    }
	    i = (struct instr **) cur_robot->retptr;
	    *i = (cur_robot->ip + 1);
	    if (r_debug)
              printf("\nsaving  return ip %ld\n",(long)(cur_robot->ip + 1));

	    /* save current local variable pointer */
	    if (--cur_robot->retptr == cur_robot->stackptr) {
	      r_flag = 1;
	    }
	    l = (long **) cur_robot->retptr;
	    *l = cur_robot->local;
	    if (r_debug)
              printf("\nsaving local pool %ld\n",(long)cur_robot->local);

	    /* setup new variable pool, if any */
	    /* variable pool starts at the first of the current agruments */
	    cur_robot->local = cur_robot->stackptr - f->par_count + 1;

	    /* initialize all other local variables to zero */
	    for (j = f->par_count; j <= f->var_count; j++)
	      *(cur_robot->local + j) = 0L;

	    /* set new stackptr just beyond the local variables */
	    cur_robot->stackptr = cur_robot->local + f->var_count;

	    /* check for collision into return stack */
	    if (cur_robot->stackptr >= cur_robot->retptr) {
	      r_flag = 1;
	    }

	    /* set new ip at start of module for next cycle */
	    cur_robot->ip = f->first;
	    called = 1;

	    break;

	  }
	}

	if (!called) {
	/* big trouble -- missing function */
	cur_robot->ip++;
	}
      }

      break;


    case RETSUB:	/* restore variable pool, ip, stackframe, push retval*/

      /* check for end of main */
      if (cur_robot->retptr == cur_robot->stackend) {
        if (r_debug)
          printf("\nend of main\n");
        r_flag = 1;
	break;
      }

      value = pop();     /* save return value */
      if (r_debug) {
        printf("\n\nreturn pointers\n");
        dumpvar(cur_robot->retptr,3);
      }

      /* restore previous local variable pool */
      l = (long **) cur_robot->retptr++;
      cur_robot->local = *l;
      if (r_debug)
        printf("\nrestore local pool %ld\n",(long) cur_robot->local);

      /* restore next instruction pointer */
      i = (struct instr **) cur_robot->retptr++;
      cur_robot->ip = *i;
      if (r_debug)
        printf("\nrestore ip %ld\n",(long) cur_robot->ip);

      /* re-frame stack to ensure we discard all expressions */
      l = (long **) cur_robot->retptr++;
      cur_robot->stackptr = *l;
      if (r_debug)
        printf("\nrestore stack %ld\n",(long) cur_robot->stackptr);

      pop();		/* get rid of bogus function value */
      push(value);	/* place return value on stack */

      break;


    case BRANCH:	/* branch if tos == zero */

      if (pop() == 0L)
	cur_robot->ip = cur_instr->u.br;
      else
        cur_robot->ip++;
      break;

    case CHOP:		/* discard tos */
      
      pop();
      cur_robot->ip++;
      break;


    case FRAME:		/* store current stackptr on retptr stack */

      /* retptr grows downward toward stackptr */
      if (--cur_robot->retptr == cur_robot->stackptr) {
	r_flag = 1;
      }

      l = (long **) cur_robot->retptr;
      *l = cur_robot->stackptr;
      if (r_debug)
        printf("\nsave frame %ld\n",(long)cur_robot->stackptr);

      cur_robot->ip++;
      break;

    default:

      cur_robot->ip++;
      break;

  }

  /* check for execution failure: stack corruption, etc */
  if (r_flag) {
    robot_go(cur_robot);	/* restart the 'main' function */
    r_flag = 0;
  }

  if (r_debug) {
    printf("\t\t\t\ttos %ld: * %ld\n",
	   (long)cur_robot->stackptr,*cur_robot->stackptr);
    printf("d,h,q,<cr>: ");
    if ((c=getchar()) == 'd') {
      printf("\nexternals");
      dumpvar(cur_robot->external,cur_robot->ext_count);
      printf("\nlocal stack");
      dumpvar(cur_robot->local,cur_robot->stackptr - cur_robot->local + 1);
      printf("\n\nx...........%7ld",cur_robot->x);
      printf("\ty...........%7ld",cur_robot->y);
      printf("\norg_x.......%7ld",cur_robot->org_x);
      printf("\torg_y.......%7ld",cur_robot->org_y);
      printf("\nrange.......%7ld",cur_robot->range);
      printf("\tspeed.......%7d",cur_robot->speed);
      printf("\nd_speed.....%7d",cur_robot->d_speed);
      printf("\theading.....%7d",cur_robot->heading);
      printf("\nd_heading...%7d",cur_robot->d_heading);
      printf("\tdamage......%7d",cur_robot->damage);
      printf("\nmiss[0]stat.%7d",missiles[cur_robot-&robots[0]][0].stat);
      printf("\tmiss[1]stat.%7d",missiles[cur_robot-&robots[0]][1].stat);
      printf("\nmiss[0]head.%7d",missiles[cur_robot-&robots[0]][0].head);
      printf("\tmiss[1]head.%7d",missiles[cur_robot-&robots[0]][1].head);
      printf("\nmiss[0]x....%7ld",missiles[cur_robot-&robots[0]][0].cur_x);
      printf("\tmiss[1]y....%7ld",missiles[cur_robot-&robots[0]][1].cur_y);
      printf("\nmiss[0]dist.%7ld",missiles[cur_robot-&robots[0]][0].curr_dist);
      printf("\tmiss[1]dist.%7ld",missiles[cur_robot-&robots[0]][1].curr_dist);
      printf("\n\n");
      getchar();
    } else {
      if (c == 'q') {     /* quit debugging */
	r_flag = 1;
      } else {            /* induce damage */
	if (c == 'h') {
	  cur_robot->damage += 10;
	}
      }
    }
  }


}
     

/* binaryop - pops 2 operands, performs operation, pushes result */
/*            divide by zero handled by returning 0 */

binaryop(op)

int op;
{
  long x,y;

  y = pop();  /* top of stack */
  x = pop();  /* next to top of stack */

  if (r_debug)
    printf("\nbinary operation %d, x = %ld y = %ld\n",op,x,y);

  switch (op) {

    case  '=':
      x = y;
      break;

    case  '|':
      x |= y;
      break;

    case  '^':
      x ^= y;
      break;

    case  '&':
      x &= y;
      break;

    case  '<':
      x = x < y;
      break;

    case  '>':
      x = x > y;
      break;

    case  '+':
      x += y;
      break;

    case  '-':
      x -= y;
      break;

    case  '*':
      x *= y;
      break;

    case  '/':
      if (y == 0L)
	x = 0L;
      else
        x /= y;
      break;

    case  '%':
      x %= y;
      break;

    case  LEFT_OP:
      x <<= y;
      break;

    case  RIGHT_OP:
      x >>= y;
      break;

    case  LE_OP:
      x = x <= y;
      break;

    case  GE_OP:
      x = x >= y;
      break;

    case  EQ_OP:
      x = x == y;
      break;

    case  NE_OP:
      x = x != y;
      break;

    case  AND_OP:
      x = x && y;
      break;

    case  OR_OP:
      x = x || y;
      break;

    case  MUL_ASSIGN:
      x *= y;
      break;

    case  DIV_ASSIGN:
      if (y == 0L)
	x = 0L;
      else
        x /= y;
      break;

    case  MOD_ASSIGN:
      x %= y;
      break;

    case  ADD_ASSIGN:
      x += y;
      break;

    case  SUB_ASSIGN:
      x -= y;
      break;

    case  LEFT_ASSIGN:
      x <<= y;
      break;

    case  RIGHT_ASSIGN:
      x >>= y;
      break;

    case  AND_ASSIGN:
      x &= y;
      break;

    case  XOR_ASSIGN:
      x ^= y;
      break;

    case  OR_ASSIGN:
      x |= y;
      break;

    case  U_NEGATIVE:
      x = -x;
      break;

    case  U_NOT:
      x = !x;
      break;

    case  U_ONES:
      x = ~x;
      break;

    default:
      break;

  }

  push(x);
}



/* robot_go - start the robot pointed to by r */

robot_go(r)

struct robot *r;
{
  register struct func *f;
  register int i;
  
  for (f = r->code_list; f != (struct func *) 0; f = f->nextfunc) {
    if (strcmp(f->func_name,"main") == 0) {
      r->ip = f->first;				/* start of code in main */
      for (i = 0; i < r->ext_count; i++)	/* zero externals */
	*(r->external + i) = 0L;
      r->local = r->stackbase;			/* setup local variables */
      for (i = 0; i <= f->var_count; i++)	/* zero locals */
	*(r->local + i) = 0L;
      r->stackptr = r->local + f->var_count;	/* set stack after locals */
      r->retptr = r->stackend;			/* return stack starts at end*/
      break;
    }
  }
}


/* dumpvar - dump a variable pool or stack for length size */

dumpvar(pool,size)

long *pool;
int size;
{
  register int i;

  for (i = 0; i < size; i++) {
    if (i % 3 == 0)
      printf("\n");
    printf("%8ld: %8ld\t",(long)(pool + i),*(pool + i));
  }
}

