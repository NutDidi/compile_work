all: expr2

expr1.tab.c expr2.tab.h:	expr2.y
	bison -d expr2.y

expr1: expr2.tab.c expr2.tab.h
	gcc -o expr2 expr2.tab.c

clean:
	rm expr2 expr2.tab.c expr2.tab.h

