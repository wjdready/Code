


all:
	flex test.l
	bison --yacc -dv test.y
	gcc y.tab.c lex.yy.c

clean:
	rm lex.yy.c y.output y.tab.c y.tab.h ./a
