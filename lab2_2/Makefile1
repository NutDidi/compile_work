all: expr1

expr1.tab.c expr1.tab.h:	expr1.y
	bison -d expr1.y

expr1: expr1.tab.c expr1.tab.h
	gcc -o expr1 expr1.tab.c

clean:
	rm expr1 expr1.tab.c expr1.tab.h
	
