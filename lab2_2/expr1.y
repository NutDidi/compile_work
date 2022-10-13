%{
/****************************************************************************
expr1.y
ParserWizard generated YACC file.

****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef YYSTYPE
#define YYSTYPE double
#endif
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror( const char* s);
%}

//MODIFY IN STEP 1
%token NUMBER
%token ADD
%token SUBTRACT
%token MULTIPLY
%token DIVIDE
%left ADD SUBTRACT
%left MULTIPLY DIVIDE
%right UMINUS

%%

lines	        :	lines expr ';'	{ printf("%f\n", $2); }
		|	lines ';'
		|
		;
		
//MODIFY IN STEP 1
expr	        :	expr ADD expr	{ $$ = $1 + $3; }
		|	expr SUBTRACT expr	{ $$ = $1 - $3; }
		|	expr MULTIPLY expr	{ $$ = $1 * $3; }
		|	expr DIVIDE expr	{ $$ = $1 / $3; }
		|	'(' expr ')'	{ $$ = $2; }
		|	SUBTRACT expr %prec UMINUS	{ $$ = -$2; }
//MODIFY IN STEP 2	
		|	NUMBER	{ $$ = $1; }
		;
/*
NUMBER	        :	'0'				{ $$ = 0.0; }
		|	'1'				{ $$ = 1.0; }
		|	'2'				{ $$ = 2.0; }
		|	'3'				{ $$ = 3.0; }
		|	'4'				{ $$ = 4.0; }
		|	'5'				{ $$ = 5.0; }
		|	'6'				{ $$ = 6.0; }
		|	'7'				{ $$ = 7.0; }
		|	'8'				{ $$ = 8.0; }
		|	'9'				{ $$ = 9.0; }
		;
*/		
		
%%

//program section

int yylex()
{
        //place your token retrieving code here
        		
//MODIFY IN STEP 1
        int t;
        while(1)
        {
             t = getchar();
             if(t == ' ' || t == '\t' || t == '\n')
             {
                   //do nothing
             }
             else if (isdigit(t))
                  {
                        yylval = 0;
                        while (isdigit(t))
                        {
                               yylval = yylval *10 +t -'0';
                               t = getchar();
                        }
                        ungetc(t,stdin);
                        return NUMBER;
                   }      
             else if(t=='+') return ADD;
             else if(t=='-') return SUBTRACT;
             else if(t=='*') return MULTIPLY;
             else if(t=='/') return DIVIDE;
             
             else return t;
        }
}

int main(void)
{
        yyin = stdin;
        do {
                 yyparse();
           }while(!feof(yyin));  
	return 0;
}

void yyerror(const char* s){
       fprintf(stderr, "Parse error: %s\n", s);
       exit(1);
}    
