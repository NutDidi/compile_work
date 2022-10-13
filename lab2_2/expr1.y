%{
/****************************************************************************
expr.y
ParserWizard generated YACC file.

Date: 2016年10月18日
****************************************************************************/
#include <iostream>
#include <cctype>
using namespace std;
%}


%include {
#ifndef YYSTYPE
#define YYSTYPE double
#endif
}


// parser name
%name expr

// class definition
{
	// place any extra class members here
	virtual int yygettoken();
}
// constructor
{
	// place any extra initialisation code here
}

// destructor
{
	// place any extra cleanup code here
}

// place any declarations here
//%token NUMBER
%left '+' '-'
%left '*' '/'
%right UMINUS
lines	:	lines expr '\n'	{ cout << $2 << endl; }
	|	lines '\n'
	|
	;

expr	:	expr '+' expr	{ $$ = $1 + $3; }
	|	expr '-' expr	{ $$ = $1 - $3; }
	|	expr '*' expr	{ $$ = $1 * $3; }
	|	expr '/' expr	{ $$ = $1 / $3; }
	|	'(' expr ')'	{ $$ = $2; }
	|	'-' expr %prec UMINUS	{ $$ = -$2; }
	|	NUMBER
	;
NUMBER	:	'0'				{ $$ = 0.0; }
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

int YYPARSERNAME::yygettoken()
{
	// place your token retrieving code here
	return getchar();
}

int main(void)
{
	int n = 1;
	expr parser;
	if (parser.yycreate()) {
		n = parser.yyparse();
	}
	return n;
}

