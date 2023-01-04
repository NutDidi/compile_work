%code top{
    #include <iostream>
    #include <assert.h>
    #include "parser.h"
    extern Ast ast;
    int yylex();
    int yyerror( char const * );
    Type* curType;
}

%code requires {
    #include "Ast.h"
    #include "SymbolTable.h"
    #include "Type.h"
}

%union {
    int itype;
    char* strtype;
    StmtNode* stmttype;
    ExprNode* exprtype;
    Type* type;
}

%start Program

%token <strtype> ID 
%token <itype> INTEGER
%token CONST
%token INT VOID 
%token SEMICOLON
%token RETURN 
%token IF ELSE WHILE
%token BREAK CONTINUE
%token COMMA 
%token ASSIGN
%token OR
%token AND
%token EQUAL UNEQUAL
%token LESS GREATER GREATERorEQUAL LESSorEQUAL
%token ADD SUB
%token MULTI DIVIDE MOD 
%token NOT
%token LPAREN RPAREN LBRACE RBRACE LSQUAREBRACKET RSQUAREBRACKET

%nterm <stmttype> Stmts Stmt BlockStmt IfStmt ReturnStmt DeclStmt FuncDef WhileStmt AssignStmt ExpStmt EmptyStmt BreakStmt ContinueStmt
%nterm <stmttype> var_idlist const_idlist const_id_decl var_id_decl FuncParams FuncParam func_argu
%nterm <exprtype> expr Exp Cond logical_or logical_and equal_expr rela_expr add_minus mult_div_mod one_expr bas_expr left_expr varExp constExp 
%nterm <type> Type

%precedence THEN
%precedence ELSE

%%
Program
    : Stmts {
        ast.setRoot($1);
    }
    ;
Stmts
    : Stmt {$$=$1;}
    | Stmts Stmt{
        $$ = new SeqNode($1, $2);
    }
    ;

//Modify finished
Stmt
    : AssignStmt {$$=$1;}
    | BlockStmt {$$=$1;}
    | IfStmt {$$=$1;}
    | ReturnStmt {$$=$1;}
    | DeclStmt {$$=$1;}
    | FuncDef {$$=$1;}
    | ExpStmt {$$=$1;}
    | WhileStmt {$$=$1;}
    | EmptyStmt {$$ = $1; }
    | BreakStmt {$$ = $1;}
    | ContinueStmt {$$ = $1;}
    ;
//break continue empty

BreakStmt
    :   
    BREAK SEMICOLON {
            $$ = new BreakStmt();
        }
    ;
ContinueStmt
    :   
    CONTINUE SEMICOLON{
            $$ = new ContinueStmt();
        }
    ;
EmptyStmt
    :
    SEMICOLON{
        $$ = new EmptyStmt();
    }
    ;
//赋值语句
AssignStmt
    :
    left_expr ASSIGN expr SEMICOLON {
        $$ = new AssignStmt($1, $3);
    }
    ;
//左值表达式
left_expr
    : ID 
    {
        SymbolEntry *se;
        se = identifiers->lookup($1);
        //变量未声明 类型检查1
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            exit(EXIT_FAILURE);
        }
        $$ = new Id(se);
        delete []$1;
    }
    //数组
    ;
//表达式 int+bool
expr
    :
    logical_or {$$ = $1;}
    ;
//表达式 int
Exp 
    :
    add_minus
    {
        $$ = $1;
    }
    ;
//逻辑或
logical_or
    :
    logical_and {$$ = $1;}
    |
    logical_or OR logical_and
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::OR, $1, $3);
    }
    ;
//逻辑与
logical_and
    : equal_expr 
    {
        $$ = $1;
    }
    | 
    logical_and AND equal_expr
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::AND, $1, $3);
    }
    ;
//相等性
equal_expr
    : rela_expr 
    {
        $$ = $1;
    }
    | equal_expr EQUAL rela_expr
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::EQUAL, $1, $3);
    }
    | 
    equal_expr UNEQUAL rela_expr 
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::UNEQUAL, $1, $3);
    }
    ;
//关系型
rela_expr
    : 
    add_minus 
    {
        $$ = $1;
    }
    | 
    rela_expr LESS add_minus
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LESS, $1, $3);
    }
    | 
    rela_expr GREATER add_minus
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GREATER, $1, $3);
    }
    | 
    rela_expr LESSorEQUAL add_minus
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LESSorEQUAL, $1, $3);
    }
    | 
    rela_expr GREATERorEQUAL add_minus
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GREATERorEQUAL, $1, $3);
    }
    ;
//加减 float
add_minus
    : mult_div_mod 
    {
        $$ = $1;
    }
    | 
    add_minus ADD mult_div_mod 
    {
        SymbolEntry *se;
        if ($1->getType()->isInt() && $3->getType()->isInt()) se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        //MOdify float else se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());            
        
        $$ = new BinaryExpr(se, BinaryExpr::ADD, $1, $3);
    }
    | 
    add_minus SUB mult_div_mod 
    {
        SymbolEntry *se;
        if ($1->getType()->isInt() && $3->getType()->isInt()) se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        // Modify float else se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());            
        $$ = new BinaryExpr(se, BinaryExpr::SUB, $1, $3);
    }
    ;
//乘除 float
mult_div_mod
    : 
    one_expr 
    {
        $$ = $1;
    }
    |  
    mult_div_mod MULTI one_expr 
    {
        SymbolEntry *se;

        if ($1->getType()->isInt() && $3->getType()->isInt()) se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        //MOdify float else se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());  

        $$ = new BinaryExpr(se, BinaryExpr::MULTI, $1, $3);
    }
    | 
    mult_div_mod DIVIDE one_expr 
    {
        SymbolEntry *se;

        if ($1->getType()->isInt() && $3->getType()->isInt()) se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        //MOdify float else se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());  

        $$ = new BinaryExpr(se, BinaryExpr::DIVIDE, $1, $3);
    }
    | 
    mult_div_mod MOD one_expr 
    {
        SymbolEntry *se;

        if ($1->getType()->isInt() && $3->getType()->isInt()) se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        //MOdify float else se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());  
        
        $$ = new BinaryExpr(se, BinaryExpr::MOD, $1, $3);
    }
    ;
//单目
one_expr
    : 
    bas_expr 
    {
        $$ = $1;
    }
    | 
    ID LPAREN func_argu RPAREN 
    {
        SymbolEntry *se;
            //函数未声明 类型检查4
            se = identifiers->lookup($1);
            if(se == nullptr)
            {
                fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
                delete [](char*)$1;
                exit(EXIT_FAILURE);
            }
            SymbolEntry *tmp = new TemporarySymbolEntry(se->getType(), SymbolTable::getLabel());
            $$ = new FuncCallNode(tmp, new Id(se), (FuncCallParamsNode*)$3);
    }
    | 
    ADD one_expr 
    {
        $$ = $2;
    }
    | 
    SUB one_expr 
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        fprintf(stderr," SUB UnaryExp %s \n",$2->getType()->toStr().c_str());
        $$ = new SingleExpr(se, SingleExpr::SUB, $2);
    }
    | 
    NOT one_expr 
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new SingleExpr(se, SingleExpr::NOT, $2);
    } 
    ;
//函数调用参数
func_argu
    :   
    func_argu COMMA Exp 
    {
        FuncCallParamsNode* node = (FuncCallParamsNode*) $1;
        node->addNext($3);
        $$ = node;
    }
    |
    Exp 
    {
        FuncCallParamsNode* node = new FuncCallParamsNode();
        node->addNext($1);
        $$ = node;    
    }
    |   
    %empty 
    {
        $$ = nullptr;
    }
    ;
//基本(括号)
bas_expr
    : 
    LPAREN expr RPAREN 
    {
        $$ = $2;
    }
    | 
    left_expr 
    {
        $$ = $1;
    }
    | 
    INTEGER 
    {
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::constIntType, $1);
        $$ = new Constant(se);
    }
    //Modify Advanced-float
    ;
//块语句
BlockStmt
    :   LBRACE 
        {identifiers = new SymbolTable(identifiers);} 
        Stmts RBRACE 
        {
            $$ = new CompoundStmt($3);
            SymbolTable *top = identifiers;
            identifiers = identifiers->getPrev();
            delete top;
        }
    |
    LBRACE RBRACE
    {
        $$ = new CompoundStmt(nullptr);
    }
    ;
//if语句
IfStmt
    : IF LPAREN Cond RPAREN Stmt %prec THEN {
        $$ = new IfStmt($3, $5);
    }
    | IF LPAREN Cond RPAREN Stmt ELSE Stmt {
        $$ = new IfElseStmt($3, $5, $7);
    }
    ;
//返回语句
ReturnStmt
    :
    RETURN expr SEMICOLON
    {
        $$ = new ReturnStmt($2);
    }
    |
    RETURN SEMICOLON
    {
        $$ = new ReturnStmt(nullptr);
    }
    ;
//条件
Cond
    :
    logical_or {$$ = $1;}
    ;
//类型 float
Type
    : INT {
        $$ = TypeSystem::intType;
        curType = TypeSystem::intType;
    }
    | VOID {
        $$ = TypeSystem::voidType;
        curType = TypeSystem::voidType;
    }
    ;
//声明语句
DeclStmt
    :
    Type var_idlist SEMICOLON {
        $$ = $2 ;
    }
    |
    CONST Type const_idlist SEMICOLON
    {
        $$ = $3;
    }
    ;

var_idlist
    :
    var_id_decl
    {
            DeclStmt* node = new DeclStmt(false);
            node->addNext((DefNode*)$1);
            $$ = node;
    }
    |
    var_id_decl COMMA var_idlist
    {
            DeclStmt* node = (DeclStmt*) $3;
            node->addNext((DefNode*)$1);
            $$ = node;
    }
    ;

var_id_decl
    :  
    ID 
    {
        //decl重定义 finished bd
        //重复声明 类型检查1 
        if( identifiers->isRedef($1) )
        {
            fprintf(stderr, "identifier \"%s\" redefine\n", $1);
            exit(EXIT_FAILURE);
        }
        // 首先将ID插入符号表中
        Type* type;
        if(curType->isInt()) type = TypeSystem::intType;
        //else type = TypeSystem::floatType;
        SymbolEntry *se;
        se = new IdentifierSymbolEntry(type, $1, identifiers->getLevel());
        identifiers->install($1, se);
        $$ = new DefNode(new Id(se), nullptr, false);
    }
    |   
    ID ASSIGN varExp 
    {
        //decl重定义 finished bd 
        //重复声明 类型检查1 
        if( identifiers->isRedef($1) )
        {
            fprintf(stderr, "identifier \"%s\" redefine\n", $1);
           exit(EXIT_FAILURE);
        }

        // 首先将ID插入符号表中
        Type* type;
        if(curType->isInt()) type = TypeSystem::intType;
        //else type = TypeSystem::floatType;
        SymbolEntry *se;
        se = new IdentifierSymbolEntry(type, $1, identifiers->getLevel());
        identifiers->install($1, se);
        $$ = new DefNode(new Id(se), (Node*)$3, false);
    }
    ;    

const_idlist
    :
    const_id_decl
    {
        DeclStmt* node = new DeclStmt(false);
        node->addNext((DefNode*)$1);
        $$ = node;        
    }
    |
    const_id_decl COMMA const_idlist
    {
        DeclStmt* node = (DeclStmt*) $3;
        node->addNext((DefNode*)$1);
        $$ = node;        
    }
    ;

const_id_decl
    :
    ID ASSIGN constExp
    {
        //decl重定义 finished bd 
        //重复声明 类型检查1 
        if( identifiers->isRedef($1) )
        {
            fprintf(stderr, "identifier \"%s\" redefine\n", $1);
            exit(EXIT_FAILURE);
        }
        Type* type;
        if(curType->isInt()) type = TypeSystem::constIntType;
        //else type = TypeSystem::constFloatType;
        SymbolEntry *se;
        se = new IdentifierSymbolEntry(type, $1, identifiers->getLevel());
        identifiers->install($1, se);
        $$ = new DefNode(new Id(se), (Node*)$3, true);//类型向上转换        
    }
    ;

varExp
    :
    add_minus{
        $$ = $1;
    }
    ;

constExp
    :
    add_minus{
        $$ = $1;
    }
    ;

FuncDef
    :
    Type ID 
    {
        Type *funcType;
        funcType = new FunctionType($1,{});
        SymbolEntry *se = new IdentifierSymbolEntry(funcType, $2, identifiers->getLevel());
        identifiers->install($2, se);
        identifiers = new SymbolTable(identifiers);
    }
    LPAREN
    FuncParams
    {
        SymbolEntry *se;
        se = identifiers->lookup($2);
        assert(se != nullptr);
        if($5!=nullptr)
        {
            ((FunctionType*)(se->getType()))->setparamsType(((FuncDefParamsNode*)$5)->getParamsType());
        }
    }
    RPAREN
    BlockStmt
    {
        SymbolEntry *se;
        se = identifiers->lookup($2);
        assert(se != nullptr);
        $$ = new FunctionDef(se,(FuncDefParamsNode*)$5,$8);
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        delete top;
        delete []$2;
    }
    ;
// 函数参数列表
FuncParams
    :   
    FuncParams COMMA FuncParam {
        FuncDefParamsNode* node = (FuncDefParamsNode*)$1;
        node->addNext(((DefNode*)$3)->getId());
        $$ = node;
    }
    |   
    FuncParam {
        FuncDefParamsNode* node = new FuncDefParamsNode();
        node->addNext(((DefNode*)$1)->getId());
        $$ = node;
    }
    |   
    %empty {
        $$ = nullptr;
    }
    ;

// 函数参数
FuncParam
    :   
    Type ID {
        SymbolEntry *se = new IdentifierSymbolEntry($1, $2, identifiers->getLevel());
        identifiers->install($2, se);
        $$ = new DefNode(new Id(se), nullptr, false);
    }
    ; 

//while语句
WhileStmt
    :
    WHILE LPAREN Cond RPAREN Stmt
    {
        $$ = new WhileStmt($3, $5);
    }
    ;
//Exp语句
ExpStmt
    :   
    ExpStmt COMMA Exp SEMICOLON{
        ExprStmtNode* node = (ExprStmtNode*)$1;
        node->addNext($3);
        $$ = node;
    }
    |   
    Exp SEMICOLON{
        ExprStmtNode* node = new ExprStmtNode();
        node->addNext($1);
        $$ = node;
    }
    ;
%%

int yyerror(char const* message)
{
    std::cerr<<message<<std::endl;
    return -1;
}
