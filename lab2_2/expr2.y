%{
/****************************************************************************
expr2.y
ParserWizard generated YACC file.

****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifndef YYSTYPE
#define YYSTYPE char*
#endif
//MODIFY IN STEP 3
char idstr[50];
char numstr[50];

int yylex();
extern int yyparse();
FILE* yyin;
void yyerror( const char* s);
%}

//MODIFY IN STEP 1
%token NUMBER
//MODIFY IN STEP 3
%token ID

%token ADD
%token SUBTRACT
%token MULTIPLY
%token DIVIDE
%left ADD SUBTRACT
%left MULTIPLY DIVIDE
%right UMINUS

%%

lines	        :	lines expr ';'	{ printf("%s\n", $2); }
		|	lines ';'
		|
		;

//MODIFY IN STEP 3
//MODIFY IN STEP 1
expr	        :	expr ADD expr	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$,$1); strcat($$,$3); strcat($$,"+ "); }
		|	expr SUBTRACT expr	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$,$1); strcat($$,$3); strcat($$,"- "); }
		|	expr MULTIPLY expr	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$,$1); strcat($$,$3); strcat($$,"* "); }
		|	expr DIVIDE expr	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$,$1); strcat($$,$3); strcat($$,"/ "); }
		|	'(' expr ')'	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$," "); strcat($$,$2); strcat($$," "); }
		|	SUBTRACT expr %prec UMINUS	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$," "); strcat($$,$2); strcat($$,"- "); }
//MODIFY IN STEP 2	
		|	NUMBER	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$,$1); strcat($$," ");  }
		|       ID	{ $$ = (char *)malloc(50*sizeof(char)); strcpy($$,$1); strcat($$," ");  }
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
        
//MODIFY IN STEP 3        		
//MODIFY IN STEP 12
        int t;
        while(1)
        {
             t = getchar();
             if(t == ' ' || t == '\t' || t == '\n')
             {
                   //do nothing
             }
             else if (t>='0' && t<='9')
                  {
                        int ti=0;
                        while (t>='0' && t<='9')
                        {
                               numstr[ti]=t;
                               t = getchar();
                               ti=ti+1;
                        }
                        numstr[ti]='\0';
                        yylval=numstr;
                        ungetc(t,stdin);
                        return NUMBER;
                   }
             else if ((t>='a' && t<='z') || (t>='A' && t<='Z') || (t =='_'))
                  {
                        int ti=0;
                        while ((t>='a' && t<='z') || (t>='A' && t<='Z') || (t =='_') || (t>='0' && t<='9'))
                        {
                               idstr[ti]=t;
                               t = getchar();
                               ti=ti+1;
                        }
                        idstr[ti]='\0';
                        yylval=idstr;
                        ungetc(t,stdin);
                        return ID;
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
