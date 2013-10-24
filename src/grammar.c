


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

# define IDENTIFIER 257
# define CONSTANT 258
# define INC_OP 259
# define DEC_OP 260
# define LEFT_OP 261
# define RIGHT_OP 262
# define LE_OP 263
# define GE_OP 264
# define EQ_OP 265
# define NE_OP 266
# define AND_OP 267
# define OR_OP 268
# define MUL_ASSIGN 269
# define DIV_ASSIGN 270
# define MOD_ASSIGN 271
# define ADD_ASSIGN 272
# define SUB_ASSIGN 273
# define LEFT_ASSIGN 274
# define RIGHT_ASSIGN 275
# define AND_ASSIGN 276
# define XOR_ASSIGN 277
# define OR_ASSIGN 278
# define EXTERN 279
# define AUTO 280
# define REGISTER 281
# define INT 282
# define LONG 283
# define IF 284
# define ELSE 285
# define WHILE 286
# define FOR 287
# define RETURN 288
# define BREAK 289
# define U_NEGATIVE 290
# define U_NOT 291
# define U_ONES 292
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 144
# define YYLAST 536
short yyact[]={

  72, 107, 158,  22,  15,  16, 105,  77,  17, 137,
 138,  72,  71, 118, 143, 144,  32,  72,  77, 139,
 121, 140,  25,  71,  77,  17,  47,  54, 170,  71,
  12,  13,  14,  15,  16, 197, 172,  47,  12,  13,
  14,  15,  16,  47,   5, 199,  80,  78, 151,  79,
  72,  27, 172,  83, 172, 196,  72,  77, 127,  82,
  74,  68,  71,  77, 203,  72,  88,  63,  71,  85,
 131,  88,  77,  48,  34, 204,  47,  71, 200,  28,
  87, 156,  47,  98,  96, 164, 103, 201, 165, 145,
  22, 146,  94,  73,  90,  86, 208,  91, 130,  72,
 212,  22, 207,  38,  73,  72,  77,  22, 206, 155,
  73,  71,  77,  72, 149, 191, 152,  71, 194, 147,
  77, 129, 190, 168, 148,  71,  72, 100,  22, 167,
  39, 104,  85,  77, 102,  29, 136,   2,  71,  18,
  22,  72,  92,  73,  19, 202,  22,   3,  77,  73,
  56, 132,  99,  71,  51, 157, 134,  55,  73,  12,
  13,  14,  15,  16,  21, 133, 154, 150,  42,  50,
  72,  95,  89, 161,  20,  49, 159, 123, 160, 166,
 162, 152,  71,  46, 174, 175, 173,  40, 163, 153,
  37, 210,  73, 192,  45, 153,  20, 132,  73, 182,
 183, 195, 188, 180, 181,  44,  73,  43, 187, 108,
 109, 110, 111, 112, 113, 114, 115, 116, 117,  73,
  17, 171, 141, 142,  17,  76,  65,  66, 198,  33,
 205, 152,  17,  11,  73,  17,  76,  65,  66, 119,
 120,  17,  76,  65,  66, 209,  12,  13,  14,  15,
  16,  59, 211,  60, 213,  53,  52,  12,  13,  14,
  15,  16,  59,  73,  60,  84,  53,  52,  59,  17,
  60,   9,  53,  52,  17,  76,  65,  66,  15,  16,
  17,  76,  65,  66,  12,  13,  14,  15,  16,  17,
  76,  65,  66,   7,  15,  16,  15,  16,  81,   8,
  26,  59, 106,  60, 126,  53,  52,  59,  58,  60,
  57,  53,  52,  61,  15,  16,  15,  16,  67, 128,
  70,  41,  64,  17,  76,  65,  66,  62,  69,  17,
  76,  65,  66, 125,  75,  10,  10,  17,  76,  65,
  66,  10,   1,   0,  62,  62,  35,   0,   0, 135,
  17,  76,  65,  66,   0,   0,  62,   0,  62,  10,
  62,  93,   0,  62,   0,  17,  76,  65,  66,   0,
 122, 124,   0,  97,   0,  62,   0, 101,   0,  23,
   4,   4,  62,   0,   0,   0,   0,   0,   0,  24,
   6,   6,   0,  62,  17,  76,  65,  66,  30,  31,
  62,  36,   0,   0,   0,   0, 169,  62,   0,  62,
   0,   0,   0, 184, 185, 186,   0,  93,   0,   0,
  36,   0,  10,   0,   0,   0,  35,   0,  62,   0,
   0,   0,   0,   0, 193,   0,   0,   0, 176, 177,
 178, 179,   0,   0, 135,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,
   0,   0,  62,   0,   0,   0, 135,   0,   0,   0,
  62,   0, 169,   0,   0,   0,   0,   0,   0, 189,
 135,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  10 };
short yypact[]={

-249,-249,-1000,-1000,-1000,-120, -37,  95,-241,-241,
-1000, -25,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-120, -22,-1000, -37,-1000,  36,   5,-1000,-1000,
-1000,-1000,-1000,  53,-1000,-1000,-1000,-1000,-1000,  17,
 -33,-1000,-1000,-1000,-1000,-1000,-1000,-1000,  25,-1000,
-1000,  23,  24,  93,-1000,  23,  94,-1000, 108,  91,
-1000,-262, -60,-254, -20, 137, 137, 108, -66,-1000,
  80,-1000,-1000,-1000, -24,-1000,-1000,  32,  98,-256,
 -41,-247,  46,  77,-1000,  10,-1000,-232,  72,-1000,
-1000,-232,-1000,-1000,-1000, -16,-1000,-1000,-1000,-1000,
  22,-283, 108,-1000, 108, 108,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 108,-1000,
-1000,-1000,-1000, 108,-1000,-1000,-1000, 108,  44,-1000,
-1000, 108,  88,  82,  -4,-1000, 108, 108, 108, 108,
 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
-1000,-1000,-1000,  72,-1000,-1000,-1000,  23,-1000,  81,
  74,-254, -66, -24,-1000, 108,  98,-1000, 108,-1000,
-1000,  78,  14,-256, -41, -41,-247,-247,-247,-247,
  46,  46,  77,  77,-1000,-1000,-1000,  20,-1000,-1000,
-1000,-1000,-1000,-1000,  34,  67,-1000,  61,  52,-1000,
  12,-1000,-1000,  66,-1000,  59,-1000,-1000,-278,-1000,
-1000,-1000,-1000,-1000 };
short yypgo[]={

   0, 342, 328, 334,  73, 322, 320, 319,  27, 304,
 318, 265,  87,  53,  59, 298,  46,  49,  47,  60,
  61,  67, 313, 310, 308, 302, 379, 389, 300, 299,
 271,  79,  44,  48, 293, 233, 229,  78,  74,  35,
 228,  45,  28, 221, 208, 321, 168, 207, 205, 194,
 183, 130, 164, 175, 169, 157, 155, 154, 150, 137,
 147, 144 };
short yyr1[]={

   0,   2,   2,   2,   5,   5,   5,   5,   5,   6,
   7,   7,   9,   9,   9,   9,  10,  10,  10,  11,
  11,  13,  13,  13,  13,  14,  14,  14,  15,  15,
  15,  16,  16,  16,  16,  16,  17,  17,  17,  18,
  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,
   8,   8,  24,  25,  25,  25,  25,  25,  25,  25,
  25,  25,  25,  25,   4,  26,  26,  27,  27,  27,
  27,  28,  28,  31,  31,  29,  29,  29,  30,  30,
  32,  34,  34,  34,  35,  37,  37,  38,  36,  36,
  39,  40,  40,  41,  41,  12,  12,  42,  43,  43,
  43,  43,  43,  33,  33,  33,  44,  44,  45,  45,
  45,  45,  45,  46,  46,  46,  46,  52,  52,  51,
  51,  47,  47,  48,  48,  53,  54,  55,  56,  49,
  58,  57,  50,  50,  50,   1,   1,  59,  59,  60,
  60,  61,  61,   3 };
short yyr2[]={

   0,   1,   1,   3,   1,   3,   2,   2,   2,   2,
   1,   3,   1,   2,   2,   2,   1,   1,   1,   1,
   4,   1,   3,   3,   3,   1,   3,   3,   1,   3,
   3,   1,   3,   3,   3,   3,   1,   3,   3,   1,
   3,   1,   3,   1,   3,   1,   3,   1,   3,   1,
   1,   2,   2,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   2,   3,   1,   2,   1,
   2,   1,   3,   1,   3,   1,   1,   1,   1,   1,
   1,   1,   2,   3,   2,   1,   2,   1,   1,   3,
   1,   1,   3,   2,   1,   1,   2,   1,   3,   2,
   3,   3,   4,   1,   3,   4,   1,   3,   1,   1,
   1,   1,   1,   2,   3,   3,   4,   1,   2,   1,
   2,   1,   2,   1,   1,   2,   4,   4,   1,   2,
   1,   4,   2,   2,   3,   1,   2,   1,   1,   2,
   3,   1,   2,   1 };
short yychk[]={

-1000,  -1, -59, -60, -26, -32, -27, -34, -29, -30,
  -3, -35, 279, 280, 281, 282, 283, 257, -59, -61,
 -46, -52, 123, -26, -27,  59, -28, -32, -31,  40,
 -27, -27,  41, -36, -38,  -3, -26, -46, 125, -51,
 -52, -45, -46, -47, -48, -49, -50,  59,  -4, -53,
 -54, -57, 289, 288,  -8, -55, -58, -23, -24, 284,
 286, -22,  -9, -21,  -5, 259, 260, -10, -20,  -2,
  -6,  45,  33, 126, -19,  -3, 258,  40, -18, -17,
 -16, -15, -14, -13, -11, -32,  59,  44,  61, -61,
  41,  44, 125, -45, 125, -51,  59, -45,  59,  59,
  -4, -45,  40,  -8,  40, 268, -25,  61, 269, 270,
 271, 272, 273, 274, 275, 276, 277, 278, 267, 259,
 260,  40,  -9,  40,  -9, -11,  -9, 124,  -7,  41,
  -8,  94,  -4, -12, -37, -30,  38, 265, 266,  60,
  62, 263, 264, 261, 262,  43,  45,  42,  47,  37,
 -31, -33,  -8, 123, -38, 125,  59, -56, 285,  -4,
  -4, -21, -20, -19,  41,  44, -18,  41,  41, -30,
 -42, -43,  40, -17, -16, -16, -15, -15, -15, -15,
 -14, -14, -13, -13, -11, -11, -11, -44, -33, -45,
  41,  41,  -8, -11,  40, -42,  41, -39, -40, -41,
 -37, -12, 125,  44,  41, -39,  41,  41,  44, -32,
 125, -33,  41, -41 };
short yydef[]={

   0,  -2, 135, 137, 138,   0,   0,  80,  67,  69,
  81,   0,  75,  76,  77,  78,  79, 143, 136, 139,
 141,   0,   0, 117,   0,  65,   0,  73,  71,  84,
  68,  70,  82,   0,  88,  87, 118, 142, 113,   0,
   0, 119, 108, 109, 110, 111, 112, 121,   0, 123,
 124,   0,   0,   0,  64,   0,   0,  50,   0,   0,
 130,  49,  19,  47,  12,   0,   0,   0,  45,   4,
   0,  16,  17,  18,  43,   1,   2,   0,  41,  39,
  36,  31,  28,  25,  21,  73,  66,   0,   0, 140,
  83,   0, 114, 120, 115,   0, 122, 129, 132, 133,
   0, 125,   0,  51,   0,   0,  52,  53,  54,  55,
  56,  57,  58,  59,  60,  61,  62,  63,   0,   7,
   8,   9,  13,   0,  14,  15,  19,   0,   0,   6,
  10,   0,   0,   0,  95,  85,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  72,  74, 103,   0,  89, 116, 134,   0, 128,   0,
   0,  48,  46,  44,   5,   0,  42,   3,   0,  86,
  96,  97,   0,  40,  37,  38,  32,  33,  34,  35,
  29,  30,  26,  27,  22,  23,  24,   0, 106, 126,
 131, 127,  11,  20,   0,   0,  99,   0,  90,  91,
  95,  94, 104,   0, 101,   0,  98, 100,   0,  93,
 105, 107, 102,  92 };
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

int yydebug = 0; /* 1 for debugging */
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

			if( yydebug ) printf( "error recovery discards char %d\n", yychar );

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

		if( yydebug ) printf("reduce %d\n",yyn);
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 1:
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
		} break;
case 2:
{ /*printf("CONSTANT\n"); */
		 if (!econst(kk))
		   return(1);
		} break;
case 5:
{ /* printf("FCALL\n"); */
		popid(func_ident,func_stack,&func_off); /* note ptr to off */
		if ((work = findvar(func_ident,func_tab)) == -1) {
		  /* printf("\n***declared %s***\n",func_ident); */
		  undeclared--; /*function name mistakenly undeclared*/
		  work = allocvar(func_ident,func_tab);
		}
		if(!efcall(work))
		  return(1);
		} break;
case 6:
{ /* printf("FCALL\n"); */
		popid(func_ident,func_stack,&func_off); /* note ptr to off */
		if ((work = findvar(func_ident,func_tab)) == -1) {
		  /* printf("\n***declared %s***\n",func_ident); */
		  undeclared--; /*function name mistakenly undeclared*/
		  work = allocvar(func_ident,func_tab);
		}
		if (!efcall(work))
		  return(1);
		} break;
case 7:
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
		} break;
case 8:
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
		} break;
case 9:
{ /* printf("FCALL-START\n"); */
		stackid(last_ident,func_stack,&func_off); /* note ptr to off */
		if (!eframe())
		  return(1);
		} break;
case 13:
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
		} break;
case 14:
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
		} break;
case 15:
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
		} break;
case 16:
{ /* printf("UNARY-OP\n"); */
		op_off++;
		*(op_stack + op_off) = '-';
		} break;
case 17:
{ 
		op_off++;
		*(op_stack + op_off) = '!';
		} break;
case 18:
{ 
		op_off++;
		*(op_stack + op_off) = '~';
		} break;
case 22:
{ /* printf("MULTIPLY\n"); */
		if (!ebinop('*'))
		  return(1);
		} break;
case 23:
{ /*printf("DIVIDE\n"); */
		if (!ebinop('/'))
		  return(1);
		} break;
case 24:
{ /* printf("MOD\n"); */
		if (!ebinop('%'))
		  return(1);
		} break;
case 26:
{ /* printf("ADD\n"); */
		if (!ebinop('+'))
		  return(1);
		} break;
case 27:
{ /* printf("SUBTRACT\n"); */
		if (!ebinop('-'))
		  return(1);
		} break;
case 29:
{ /* printf("SHIFT-LEFT\n"); */
		if (!ebinop(LEFT_OP))
		  return(1);
		} break;
case 30:
{ /* printf("SHIFT-RIGHT\n"); */
		if (!ebinop(RIGHT_OP))
		  return(1);
		} break;
case 32:
{ /* printf("LESS-THAN\n"); */
		if (!ebinop('<'))
		  return(1);
		} break;
case 33:
{ /* printf("GREATER-THAN\n"); */
		if (!ebinop('>'))
		  return(1);
		} break;
case 34:
{ /*printf("LESS-EQUAL\n"); */
		if (!ebinop(LE_OP))
		  return(1);
		} break;
case 35:
{ /* printf("GREATER-EQUAL\n"); */
		if (!ebinop(GE_OP))
		  return(1);
		} break;
case 37:
{ /* printf("EQUAL\n"); */
		if (!ebinop(EQ_OP))
		  return(1);
		} break;
case 38:
{ /* printf("NOT-EQUAL\n"); */
		if (!ebinop(NE_OP))
		  return(1);
		} break;
case 40:
{ /* printf("AND\n"); */
		if (!ebinop('&'))
		  return(1);
		} break;
case 42:
{ /* printf("EXCLUSIVE-OR\n"); */
		if (!ebinop('^'))
		  return(1);
		} break;
case 44:
{ /* printf("INCLUSIVE-OR\n"); */
		if (!ebinop('|'))
		  return(1);
		} break;
case 46:
{ /* printf("LOGICAL-AND\n"); */
		if (!ebinop(AND_OP))
		  return(1);
		} break;
case 48:
{ /* printf("LOGICAL-OR\n"); */
		if (!ebinop(OR_OP))
		  return(1);
		} break;
case 51:
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
		} break;
case 52:
{ /* printf("ASSIGNMENT-LVAL\n"); */
		stackid(last_ident,var_stack,&var_off); /* note ptr to off */
		op_off++;
		*(op_stack + op_off) = work;
		} break;
case 53:
{ work =  '=';} break;
case 54:
{ work = MUL_ASSIGN;} break;
case 55:
{ work = DIV_ASSIGN;} break;
case 56:
{ work = MOD_ASSIGN;} break;
case 57:
{ work = ADD_ASSIGN;} break;
case 58:
{ work = SUB_ASSIGN;} break;
case 59:
{ work = LEFT_ASSIGN;} break;
case 60:
{ work = RIGHT_ASSIGN;} break;
case 61:
{ work = AND_ASSIGN;} break;
case 62:
{ work = XOR_ASSIGN;} break;
case 63:
{ work = OR_ASSIGN;} break;
case 74:
{ /* printf("INITIALIZER\n"); */
		fprintf(f_out,"\n**Warning** unsupported initializer\n");
		/* get rid of constant placed on stack */
		if (!echop())
		  return(1);
		} break;
case 81:
{ /* printf("VARIABLE-DECLARE\n"); */
		if (in_func) {
		  if (findvar(last_ident,var_tab) == -1)
		    allocvar(last_ident,var_tab);
		}
		else {
		  if (findvar(last_ident,ext_tab) == -1)
		    allocvar(last_ident,ext_tab);
		}
		} break;
case 82:
{ /* printf("FUNCTION-DECLARE\n"); */
		if (new_func() == -1)
		  return (1); /* exit the parser */
		} break;
case 83:
{ /* printf("FUNCTION-DECLARE\n"); */
		if (new_func() == -1)
		  return (1); /* exit the parser */
		} break;
case 84:
{ /* printf("FUNCTION-DEF-START\n"); */
		strcpy(func_ident,last_ident);
		} break;
case 87:
{ /* printf("PARAMETER-DECLARE\n"); */
		allocvar(last_ident,var_tab);
		num_parm++;
		} break;
case 122:
{ /* printf("CHOP\n"); */
		if (!echop())
		  return(1);
		} break;
case 125:
{ /* printf("IF-THEN\n"); */
		else_part();
		close_if();
		} break;
case 126:
{ /* printf("IF-THEN-ELSE\n"); */
		close_if();
		} break;
case 127:
{ /* printf("IF-CLAUSE\n"); */
		if (!new_if())
		  return (1); /* exit parser */
		} break;
case 128:
{ /* printf("ELSE-CLAUSE\n"); */
		else_part();
		} break;
case 129:
{ /* printf("WHILE\n"); */
		close_while();
		} break;
case 130:
{ /* printf("WHILE-TOKEN\n"); */
		if (!new_while())
		  return (1);  /* exit the parser */
		} break;
case 131:
{ /* printf("WHILE-CLAUSE\n"); */
		while_expr();
		} break;
case 132:
{ /* printf("BREAK\n"); */
		/* breaks can be handled by building a instruct chain */
		/* as part of the while_nest structures and patching them */
		/* on while_close.  maybe later */
		fprintf(f_out,"\n**Warning** unsupported break\n");
		} break;
case 133:
{ /* printf("RETURN-NOEXPR\n"); */
		/* all functions must return a value */
		if (!econst(1L))
		  return(1);
		if (!eretsub())
		  return(1);
		} break;
case 134:
{ /* printf("RETURN\n"); */
		if (!eretsub())
		  return(1);
		} break;
case 137:
{ /* printf("FUNCTION-DEFINITION\n"); */
		/* all functions must return a value */
		if (!econst(1L))
		  return(1);
		if (!eretsub())
		  return(1);
		end_func();
		} break;
case 138:
{ /* printf("EXTERNAL-DECLARE\n"); */
		} break;
		}
		goto yystack;  /* stack new state and value */

	}
