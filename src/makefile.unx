# makefile for crobots

OBJS =  main.o grammar.o lexanal.o compiler.o cpu.o intrins.o display.o screen.o motion.o

SOURCES = crobots.h compiler.h tokens.h main.c grammar.y grammar.c lexanal.l lexanal.c compiler.c cpu.c intrins.c display.c screen.c screend.c motion.c 

DOS = main.c grammar.c lexanal.c compiler.c cpu.c intrins.c display.c screend.c motion.c 

DOSA = main.c grammar.c lexanal.c compiler.c cpu.c intrins.c display.c screena.c motion.c 

# Xenix cc flags: optimize(O), omit stack checking(K), separate text and data(i)

CFLAGS = -O -K -i -DUNIX
LIBS = -lm -lcurses -ltermlib

# note - grammar.c and lexanal.c: both yacc (grammar.y) and lex (lexanal.l)
#  place '# line' and/or '# ifdef' that sometimes choke the PC-DOS compiler,
#  Lattice C, ver 2.14.  The egrep filters are designed to remove 
#  the troublesome spots.
#  Lattice does not accept '# line nn' or '#' (null) statements, and gets
#  confused when '#ifdef symbol .... #endif' are nested.

crobots: 	$(OBJS)
		cc -s $(CFLAGS) $(OBJS) -o crobots $(LIBS)

main.o: 	crobots.h main.c
		cc $(CFLAGS) -c main.c

grammar.o: 	crobots.h compiler.h grammar.y
		yacc -d grammar.y
		egrep -v '# line|^#$$' y.tab.c >grammar.c
		rm y.tab.c
		mv y.tab.h tokens.h
		cc $(CFLAGS) -c grammar.c

lexanal.o: 	crobots.h compiler.h lexanal.l grammar.y
		lex lexanal.l
		egrep -v '# ifdef|# endif' lex.yy.c >lexanal.c
		rm lex.yy.c
		cc $(CFLAGS) -c lexanal.c

compiler.o: 	crobots.h compiler.h tokens.h compiler.c
		cc $(CFLAGS) -c compiler.c
	
cpu.o: 		crobots.h grammar.y cpu.c
		cc $(CFLAGS) -c cpu.c
	
intrins.o:	crobots.h intrins.c
		cc $(CFLAGS) -c intrins.c
	
display.o:	crobots.h display.c
		cc $(CFLAGS) -c display.c
	
screen.o:	crobots.h screen.c
		cc $(CFLAGS) -c screen.c
	
motion.o:	crobots.h motion.c
		cc $(CFLAGS) -c motion.c
	
dos:
		cc -s -dos -K -i -O -DDOS $(DOS) -o crobotsx.exe -lm
		/bin/rm *.o

dosa:
		cc -s -dos -K -i -O -DDOS $(DOSA) -o crobotsa.exe -lm
		/bin/rm *.o
print:
		pr -t $(SOURCES) | lpr

lint:
		lint -abc *.c | tee crobots.lint
		@echo 'lint output kept in crobots.lint'

