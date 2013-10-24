# include "stdio.h"
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;

/*****************************************************************************/
/*                                                                           */
/*  CROBOTS                                                                  */
/*                                                                           */
/*  (C) Copyright Tom Poindexter, 1985, all rights reserved.                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/* lexical analyzer for crobots */

#include "tokens.h"
#include "crobots.h"
#include "compiler.h"

/* redefine i/o */
#undef 	input
#define	input()  	lexgetc()
#undef 	unput
#define	unput(c) 	lexungetc(c)
#undef 	output   
#define output(c)	lexputc(c)
#undef ECHO
#define ECHO fprintf(f_out,"%s",yytext)

long atol();
int count();

# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
		{ comment(); }
break;
case 2:
		{ count(); return(AUTO); }
break;
case 3:
		{ count(); return(BREAK); }
break;
case 4:
		{ count(); return(ELSE); }
break;
case 5:
	{ count(); return(EXTERN); }
break;
case 6:
		{ count(); return(FOR); }
break;
case 7:
		{ count(); return(IF); }
break;
case 8:
		{ count(); return(INT); }
break;
case 9:
		{ count(); return(LONG); }
break;
case 10:
	{ count(); return(REGISTER); }
break;
case 11:
	{ count(); return(RETURN); }
break;
case 12:
		{ count(); return(WHILE); }
break;
case 13:
	{ count(); 
				strncpy(last_ident,yytext,ILEN-1);
				last_ident[ILEN-1] = '\0';
				return(IDENTIFIER); }
break;
case 14:
    		{ count(); 
				kk = atol(yytext);
				return(CONSTANT); }
break;
case 15:
		{ count(); return(RIGHT_ASSIGN); }
break;
case 16:
		{ count(); return(LEFT_ASSIGN); }
break;
case 17:
		{ count(); return(ADD_ASSIGN); }
break;
case 18:
		{ count(); return(SUB_ASSIGN); }
break;
case 19:
		{ count(); return(MUL_ASSIGN); }
break;
case 20:
		{ count(); return(DIV_ASSIGN); }
break;
case 21:
		{ count(); return(MOD_ASSIGN); }
break;
case 22:
		{ count(); return(AND_ASSIGN); }
break;
case 23:
		{ count(); return(XOR_ASSIGN); }
break;
case 24:
		{ count(); return(OR_ASSIGN); }
break;
case 25:
		{ count(); return(RIGHT_OP); }
break;
case 26:
		{ count(); return(LEFT_OP); }
break;
case 27:
		{ count(); return(INC_OP); }
break;
case 28:
		{ count(); return(DEC_OP); }
break;
case 29:
		{ count(); return(AND_OP); }
break;
case 30:
		{ count(); return(OR_OP); }
break;
case 31:
		{ count(); return(LE_OP); }
break;
case 32:
		{ count(); return(GE_OP); }
break;
case 33:
		{ count(); return(EQ_OP); }
break;
case 34:
		{ count(); return(NE_OP); }
break;
case 35:
		{ count(); return(';'); }
break;
case 36:
		{ count(); return('{'); }
break;
case 37:
		{ count(); return('}'); }
break;
case 38:
		{ count(); return(','); }
break;
case 39:
		{ count(); return('='); }
break;
case 40:
		{ count(); return('('); }
break;
case 41:
		{ count(); return(')'); }
break;
case 42:
		{ count(); return('.'); }
break;
case 43:
		{ count(); return('&'); }
break;
case 44:
		{ count(); return('!'); }
break;
case 45:
		{ count(); return('~'); }
break;
case 46:
		{ count(); return('-'); }
break;
case 47:
		{ count(); return('+'); }
break;
case 48:
		{ count(); return('*'); }
break;
case 49:
		{ count(); return('/'); }
break;
case 50:
		{ count(); return('%'); }
break;
case 51:
		{ count(); return('<'); }
break;
case 52:
		{ count(); return('>'); }
break;
case 53:
		{ count(); return('^'); }
break;
case 54:
		{ count(); return('|'); }
break;
case 55:
	{ count(); }
break;
case 56:
		{ /* ignore bad characters */ }
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

yywrap()
{
	return(1);
}

comment()
{
	char c, c1;

	output('/');
	output('*');

loop:
	while ((c = input()) != '*' && c != 0)
		output(c);

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0) {
		output('*');
		output(c1);
	}
}


count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}


/* the i/o routines depend on f_in and f_out to be opened elsewhere */

int lexgetc()
{
  int c;
  c = fgetc(f_in);
  return ((c == EOF) ? 0 : c);
}

lexungetc(c)
int c;
{
  ungetc(c,f_in);
}

lexputc(c)
int c;
{
  fputc(c,f_out);
}

/* these two dummy routines for lex's debug options */
allprint() {}

sprint() {} 

int yyvstop[] = {
0,

56,
0,

55,
56,
0,

55,
0,

44,
56,
0,

50,
56,
0,

43,
56,
0,

40,
56,
0,

41,
56,
0,

48,
56,
0,

47,
56,
0,

38,
56,
0,

46,
56,
0,

42,
56,
0,

49,
56,
0,

14,
56,
0,

35,
56,
0,

51,
56,
0,

39,
56,
0,

52,
56,
0,

13,
56,
0,

53,
56,
0,

13,
56,
0,

13,
56,
0,

13,
56,
0,

13,
56,
0,

13,
56,
0,

13,
56,
0,

13,
56,
0,

13,
55,
56,
0,

13,
56,
0,

36,
56,
0,

54,
56,
0,

37,
56,
0,

45,
56,
0,

34,
0,

21,
0,

29,
0,

22,
0,

19,
0,

27,
0,

17,
0,

28,
0,

18,
0,

1,
0,

20,
0,

14,
0,

26,
0,

31,
0,

33,
0,

32,
0,

25,
0,

13,
0,

23,
0,

13,
0,

13,
0,

13,
0,

13,
0,

13,
0,

7,
13,
0,

13,
0,

13,
0,

13,
0,

13,
0,

24,
0,

30,
0,

16,
0,

15,
0,

13,
0,

13,
0,

13,
0,

13,
0,

6,
13,
0,

8,
13,
0,

13,
0,

13,
0,

13,
0,

13,
0,

2,
13,
0,

13,
0,

4,
13,
0,

13,
0,

9,
13,
0,

13,
0,

13,
0,

13,
0,

3,
13,
0,

13,
0,

13,
0,

13,
0,

12,
13,
0,

5,
13,
0,

13,
0,

11,
13,
0,

13,
0,

10,
13,
0,
0};
# define YYTYPE char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	0,0,	
0,0,	0,0,	1,7,	1,8,	
0,0,	1,9,	1,10,	1,11,	
1,12,	1,13,	1,14,	1,15,	
1,16,	1,17,	8,39,	12,42,	
14,44,	16,46,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
1,18,	1,19,	1,20,	1,21,	
6,37,	7,38,	1,22,	11,41,	
14,45,	12,43,	19,49,	19,50,	
16,47,	8,40,	17,48,	17,48,	
17,48,	17,48,	17,48,	17,48,	
17,48,	17,48,	17,48,	17,48,	
20,51,	21,52,	21,53,	23,55,	
34,66,	49,68,	53,69,	0,0,	
0,0,	0,0,	0,0,	1,23,	
0,0,	0,0,	1,24,	1,25,	
0,0,	0,0,	1,26,	1,27,	
28,61,	30,64,	1,28,	32,65,	
57,71,	1,29,	26,58,	0,0,	
28,62,	27,60,	29,63,	1,30,	
25,57,	58,72,	56,70,	1,31,	
1,32,	24,56,	26,59,	59,73,	
1,33,	1,34,	1,35,	1,36,	
2,6,	60,74,	62,75,	63,76,	
2,7,	2,8,	64,77,	2,9,	
2,10,	2,11,	2,12,	2,13,	
2,14,	2,15,	2,16,	65,79,	
70,80,	71,81,	72,82,	64,78,	
73,83,	76,84,	77,85,	34,67,	
78,86,	79,87,	2,18,	2,19,	
2,20,	2,21,	81,88,	83,89,	
85,90,	86,91,	87,92,	89,93,	
90,94,	91,95,	94,96,	96,97,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	2,23,	0,0,	0,0,	
2,24,	2,25,	0,0,	0,0,	
2,26,	2,27,	0,0,	0,0,	
2,28,	0,0,	0,0,	2,29,	
0,0,	0,0,	0,0,	0,0,	
0,0,	2,30,	0,0,	0,0,	
0,0,	0,0,	2,32,	0,0,	
0,0,	0,0,	2,33,	2,34,	
2,35,	2,36,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	0,0,	0,0,	0,0,	
0,0,	22,54,	0,0,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	22,54,	22,54,	22,54,	
22,54,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-95,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+0,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+3,	0,		yyvstop+8,
yycrank+4,	0,		yyvstop+11,
yycrank+12,	0,		yyvstop+14,
yycrank+0,	0,		yyvstop+17,
yycrank+0,	0,		yyvstop+20,
yycrank+6,	0,		yyvstop+23,
yycrank+8,	0,		yyvstop+26,
yycrank+0,	0,		yyvstop+29,
yycrank+7,	0,		yyvstop+32,
yycrank+0,	0,		yyvstop+35,
yycrank+11,	0,		yyvstop+38,
yycrank+26,	0,		yyvstop+41,
yycrank+0,	0,		yyvstop+44,
yycrank+10,	0,		yyvstop+47,
yycrank+23,	0,		yyvstop+50,
yycrank+24,	0,		yyvstop+53,
yycrank+174,	0,		yyvstop+56,
yycrank+26,	0,		yyvstop+59,
yycrank+4,	yysvec+22,	yyvstop+62,
yycrank+2,	yysvec+22,	yyvstop+65,
yycrank+2,	yysvec+22,	yyvstop+68,
yycrank+2,	yysvec+22,	yyvstop+71,
yycrank+2,	yysvec+22,	yyvstop+74,
yycrank+3,	yysvec+22,	yyvstop+77,
yycrank+4,	yysvec+22,	yyvstop+80,
yycrank+0,	yysvec+22,	yyvstop+83,
yycrank+3,	yysvec+22,	yyvstop+87,
yycrank+0,	0,		yyvstop+90,
yycrank+27,	0,		yyvstop+93,
yycrank+0,	0,		yyvstop+96,
yycrank+0,	0,		yyvstop+99,
yycrank+0,	0,		yyvstop+102,
yycrank+0,	0,		yyvstop+104,
yycrank+0,	0,		yyvstop+106,
yycrank+0,	0,		yyvstop+108,
yycrank+0,	0,		yyvstop+110,
yycrank+0,	0,		yyvstop+112,
yycrank+0,	0,		yyvstop+114,
yycrank+0,	0,		yyvstop+116,
yycrank+0,	0,		yyvstop+118,
yycrank+0,	0,		yyvstop+120,
yycrank+0,	0,		yyvstop+122,
yycrank+0,	yysvec+17,	yyvstop+124,
yycrank+28,	0,		yyvstop+126,
yycrank+0,	0,		yyvstop+128,
yycrank+0,	0,		yyvstop+130,
yycrank+0,	0,		yyvstop+132,
yycrank+29,	0,		yyvstop+134,
yycrank+0,	yysvec+22,	yyvstop+136,
yycrank+0,	0,		yyvstop+138,
yycrank+2,	yysvec+22,	yyvstop+140,
yycrank+7,	yysvec+22,	yyvstop+142,
yycrank+2,	yysvec+22,	yyvstop+144,
yycrank+7,	yysvec+22,	yyvstop+146,
yycrank+15,	yysvec+22,	yyvstop+148,
yycrank+0,	yysvec+22,	yyvstop+150,
yycrank+14,	yysvec+22,	yyvstop+153,
yycrank+21,	yysvec+22,	yyvstop+155,
yycrank+31,	yysvec+22,	yyvstop+157,
yycrank+38,	yysvec+22,	yyvstop+159,
yycrank+0,	0,		yyvstop+161,
yycrank+0,	0,		yyvstop+163,
yycrank+0,	0,		yyvstop+165,
yycrank+0,	0,		yyvstop+167,
yycrank+33,	yysvec+22,	yyvstop+169,
yycrank+48,	yysvec+22,	yyvstop+171,
yycrank+45,	yysvec+22,	yyvstop+173,
yycrank+47,	yysvec+22,	yyvstop+175,
yycrank+0,	yysvec+22,	yyvstop+177,
yycrank+0,	yysvec+22,	yyvstop+180,
yycrank+46,	yysvec+22,	yyvstop+183,
yycrank+45,	yysvec+22,	yyvstop+185,
yycrank+35,	yysvec+22,	yyvstop+187,
yycrank+45,	yysvec+22,	yyvstop+189,
yycrank+0,	yysvec+22,	yyvstop+191,
yycrank+51,	yysvec+22,	yyvstop+194,
yycrank+0,	yysvec+22,	yyvstop+196,
yycrank+45,	yysvec+22,	yyvstop+199,
yycrank+0,	yysvec+22,	yyvstop+201,
yycrank+45,	yysvec+22,	yyvstop+204,
yycrank+47,	yysvec+22,	yyvstop+206,
yycrank+61,	yysvec+22,	yyvstop+208,
yycrank+0,	yysvec+22,	yyvstop+210,
yycrank+53,	yysvec+22,	yyvstop+213,
yycrank+48,	yysvec+22,	yyvstop+215,
yycrank+55,	yysvec+22,	yyvstop+217,
yycrank+0,	yysvec+22,	yyvstop+219,
yycrank+0,	yysvec+22,	yyvstop+222,
yycrank+65,	yysvec+22,	yyvstop+225,
yycrank+0,	yysvec+22,	yyvstop+227,
yycrank+53,	yysvec+22,	yyvstop+230,
yycrank+0,	yysvec+22,	yyvstop+232,
0,	0,	0};
struct yywork *yytop = yycrank+296;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,011 ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,'A' ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'v' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
	int debug;
	char *yylastch;
	/* start off machines */
	debug = 0;
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
			yyfirst=0;
		tryagain:
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
				if(debug)fprintf(yyout,"compressed state\n");
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
				goto tryagain;
				}
			else
				{unput(*--yylastch);break;}
		contin:
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
			;
			}
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
		if(debug)putchar('\n');
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
