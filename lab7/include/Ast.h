#ifndef __AST_H__
#define __AST_H__

#include <fstream>
#include "Operand.h"
#include<stack>//lab6
#include<vector>//lab6

class SymbolEntry;
class Unit;
class Function;
class BasicBlock;
class Instruction;
class IRBuilder;
class Type;//lab6

class Node
{
private:
    static int counter;
    int seq;
protected:
    std::vector<Instruction*> true_list;
    std::vector<Instruction*> false_list;
    static IRBuilder *builder;
    void backPatch(std::vector<Instruction*> &list, BasicBlock*bb);
    std::vector<Instruction*> merge(std::vector<Instruction*> &list1, std::vector<Instruction*> &list2);

    Operand* typeCast(Type* targetType, Operand* operand); //lab6 类型转换
public:
    Node();
    int getSeq() const {return seq;};
    static void setIRBuilder(IRBuilder*ib) {builder = ib;};
    virtual void output(int level) = 0;
    virtual void typeCheck() = 0;
    virtual void genCode() = 0;
    std::vector<Instruction*>& trueList() {return true_list;}
    std::vector<Instruction*>& falseList() {return false_list;}
};

class ExprNode : public Node
{
protected:
    SymbolEntry *symbolEntry;
    Operand *dst;   // The result of the subtree is stored into dst.
public:
    ExprNode(SymbolEntry *symbolEntry) : symbolEntry(symbolEntry){dst = new Operand(symbolEntry);};//lab6
    Operand* getOperand() {return dst;};
    SymbolEntry* getSymPtr() {return symbolEntry;};

    Type* getType(){return symbolEntry->getType();};//lab6
    void setType(Type* type){ symbolEntry->setType(type);};//lab6
};

class BinaryExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr1, *expr2;
public:
    enum {OR, AND, EQUAL, UNEQUAL, LESS, GREATER, LESSorEQUAL, GREATERorEQUAL, ADD, SUB, MULTI, DIVIDE, MOD}; //lab6
    BinaryExpr(SymbolEntry *se, int op, ExprNode*expr1, ExprNode*expr2) : ExprNode(se), op(op), expr1(expr1), expr2(expr2){dst = new Operand(se);};
    void output(int level);
    void typeCheck();
    void genCode();
};

//lab6
class SingleExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr1;
public:
    enum { SUB, NOT};
    SingleExpr(SymbolEntry *se, int op, ExprNode*expr1) : ExprNode(se), op(op), expr1(expr1){};
    void output(int level);
    void typeCheck();
    void genCode();    
};

class Constant : public ExprNode
{
public:
    Constant(SymbolEntry *se) : ExprNode(se){dst = new Operand(se);};
    void output(int level);
    void typeCheck();
    void genCode();
};

class Id : public ExprNode
{
public:
    // Id(SymbolEntry *se);//lab6
    Id(SymbolEntry *se) : ExprNode(se){};//bd
    void output(int level);
    SymbolEntry* getSymbolEntry() {return symbolEntry;}
    void typeCheck();
    void genCode();
};

class StmtNode : public Node
{};

//lab6
class ExprStmtNode : public StmtNode
{
private:
    std::vector<ExprNode*> exprList;
public:
    ExprStmtNode(){};
    void addNext(ExprNode* next);
    void output(int level);
    void typeCheck();
    void genCode();
};

//lab6
class EmptyStmt : public StmtNode
{
public:
    EmptyStmt(){};
    void genCode();
    void typeCheck();
    void output(int level);
};

//while break continue语句
//当前while入栈
//增加成员变量condBlock、endBlock 为break,continue做准备
//lab6
class WhileStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *loopStmt;
    BasicBlock* condBlock;
    BasicBlock* endBlock;
public:
    WhileStmt(ExprNode *Cond, StmtNode *LoopStmt):cond(Cond),loopStmt(LoopStmt),condBlock{nullptr},endBlock(nullptr){};
    BasicBlock* getCond(){return this->condBlock;}
    BasicBlock* getEnd(){return this->endBlock;}
    void output(int level);
    void genCode();
    void typeCheck();
};

//lab6
//记录当前while
class BreakStmt : public StmtNode
{
private:
    WhileStmt * whileStmt;   
public:
    BreakStmt():whileStmt(nullptr){};
    void genCode();
    void typeCheck();
    void output(int level);
};

//lab6
//记录当前while
class ContinueStmt : public StmtNode
{
private:
    WhileStmt * whileStmt;  
public:
    ContinueStmt():whileStmt(nullptr){};   
    void genCode();
    void typeCheck();
    void output(int level);
};

class CompoundStmt : public StmtNode
{
private:
    StmtNode *stmt;
public:
    CompoundStmt(StmtNode *stmt) : stmt(stmt) {};
    void output(int level);
    void typeCheck();
    void genCode();
};

class SeqNode : public StmtNode
{
private:
    StmtNode *stmt1, *stmt2;
public:
    SeqNode(StmtNode *stmt1, StmtNode *stmt2) : stmt1(stmt1), stmt2(stmt2){};
    void output(int level);
    void typeCheck();
    void genCode();
};

//声明节点
//lab6
class DefNode : public StmtNode
{
private:
    bool isConst;
    Id* id;
    Node* Value;
public:
    DefNode(Id* id, Node* initVal, bool isConst) : isConst(isConst), id(id), Value(initVal){};
    Id* getId() {return id;}
    void output(int level);
    void typeCheck();
    void genCode();
};

//lab6
class DeclStmt : public StmtNode
{
private:
    bool isConst; //lab6
    std::vector<DefNode*> defList; //lab6
public:
    DeclStmt(bool isConst) : isConst(isConst){}; //lab6
    void addNext(DefNode* next);
    void output(int level);
    void typeCheck();
    void genCode();
};

//lab6
// class DeclStmt : public StmtNode
// {
// private:
//     bool isConst;
//     std::vector<DefNode*> defList;
// public:
//     DeclStmt(bool isConst) : isConst(isConst){};
//     void addNext(DefNode* next);
//     void output(int level);
//     void typeCheck();
//     void genCode();
// };

class IfStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
public:
    IfStmt(ExprNode *cond, StmtNode *thenStmt) : cond(cond), thenStmt(thenStmt){};
    void output(int level);
    void typeCheck();
    void genCode();
};

class IfElseStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
    StmtNode *elseStmt;
public:
    IfElseStmt(ExprNode *cond, StmtNode *thenStmt, StmtNode *elseStmt) : cond(cond), thenStmt(thenStmt), elseStmt(elseStmt) {};
    void output(int level);
    void typeCheck();
    void genCode();
};

class ReturnStmt : public StmtNode
{
private:
    ExprNode *retValue;
    Type* retType; //lab6
public:
    ReturnStmt(ExprNode*retValue) : retValue(retValue) {};
    void output(int level);
    void typeCheck();
    void genCode();
};

class AssignStmt : public StmtNode
{
private:
    ExprNode *lval;
    ExprNode *expr;
public:
    AssignStmt(ExprNode *lval, ExprNode *expr) : lval(lval), expr(expr) {};
    void output(int level);
    void typeCheck();
    void genCode();
};

//lab6
//函数调用参数
class FuncCallParamsNode : public StmtNode
{
private:
    std::vector<ExprNode*> paramsList;
public:
    FuncCallParamsNode(){};
    void addNext(ExprNode* next);
    void output(int level);
    void typeCheck();
    void genCode();

    std::vector<Type*> getParamsType();
    std::vector<Operand*> getOperandList();
    std::vector<ExprNode*> getParamsList();

};

//lab6
//函数调用
class FuncCallNode : public ExprNode
{
private:
    Id* funcId;
    FuncCallParamsNode* params = nullptr;
public:
    FuncCallNode(SymbolEntry *se, Id* id, FuncCallParamsNode* params) : ExprNode(se), funcId(id), params(params){};
    void output(int level);
    void typeCheck();
    void genCode();    
};

//lab6
//函数定义参数
class FuncDefParamsNode : public StmtNode
{
private:
    std::vector<Id*> paramsList;
public:
    FuncDefParamsNode() {};
    void addNext(Id* next);
    std::vector<Type*> getParamsType();
    void output(int level);
    void typeCheck();
    void genCode();
};

class FunctionDef : public StmtNode
{
private:
    SymbolEntry *se;
    FuncDefParamsNode *params; //lab6
    StmtNode *stmt;
public:
    FunctionDef(SymbolEntry *se, FuncDefParamsNode *params, StmtNode *stmt) : se(se), params(params), stmt(stmt){};//lab6
    void output(int level);
    void typeCheck();
    void genCode();
};

class Ast
{
private:
    Node* root;
public:
    Ast() {root = nullptr;}
    void setRoot(Node*n) {root = n;}
    void output();
    void typeCheck();
    void genCode(Unit *unit);
};

#endif
