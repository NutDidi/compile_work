#include "SymbolTable.h"
#include "Type.h"
#include <sstream>
#include <string.h>
#include <iomanip>
#include <iostream>

extern FILE* yyout;

SymbolEntry::SymbolEntry(Type *type, int kind) 
{
    this->type = type;
    this->kind = kind;
}

ConstantSymbolEntry::ConstantSymbolEntry(Type *type, int value) : SymbolEntry(type, SymbolEntry::CONSTANT)
{
    this->value = value;
}
//Modify float处理
std::string ConstantSymbolEntry::toStr()
{
    std::ostringstream buffer;
    buffer << value;
    return buffer.str();
}

//Modify 标识符

IdentifierSymbolEntry::IdentifierSymbolEntry(Type *type, std::string name, int scope) : SymbolEntry(type, SymbolEntry::VARIABLE), name(name)
{
    this->scope = scope;
    addr = nullptr;//Modify
}

std::string IdentifierSymbolEntry::toStr()
{
    if(type==TypeSystem::constIntType){
        std::ostringstream buffer;
        buffer << (int)value;
        return buffer.str();
    }    
    if(isGlobal()){
        return "@" + name;
    }
    else if(isParam()){
        return "%" + name;
    }
}

//Modify 库函数isLibFunc
bool IdentifierSymbolEntry::isLib()
{
    return  name=="getint" || name=="getch" || name=="putint" || name=="putch" || name=="starttime" || name=="stoptime";
}
//Modify 声明output bd
void IdentifierSymbolEntry::outputgloabl() 
{
    fprintf(yyout, "@%s = dso_local global %s %d\n", this->name.c_str(), this->type->toStr().c_str(), (int)value);
}
void IdentifierSymbolEntry::outputlibfunc() 
{

    fprintf(yyout, "declare %s @%s(", dynamic_cast<FunctionType*>(type)->getRetType()->toStr().c_str(), (const char*)name.c_str());
    
    int param_index = 0;
    for(auto paramType : dynamic_cast<FunctionType*>(type)->getParamsType())
    {
        param_index ++;
        if(param_index == 1)
        {
            fprintf(yyout, "%s", paramType->toStr().c_str());
        }
        else fprintf(yyout, ", %s", paramType->toStr().c_str());
    }
    fprintf(yyout, ")\n");
}

TemporarySymbolEntry::TemporarySymbolEntry(Type *type, int label) : SymbolEntry(type, SymbolEntry::TEMPORARY)
{
    this->label = label;
}

std::string TemporarySymbolEntry::toStr()
{
    std::ostringstream buffer;
    buffer << "%t" << label;
    return buffer.str();
}

SymbolTable::SymbolTable()
{
    prev = nullptr;
    level = 0;
}

SymbolTable::SymbolTable(SymbolTable *prev)
{
    this->prev = prev;
    this->level = prev->level + 1;
}

/*
    Description: lookup the symbol entry of an identifier in the symbol table
    Parameters: 
        name: identifier name
    Return: pointer to the symbol entry of the identifier

    hint:
    1. The symbol table is a stack. The top of the stack contains symbol entries in the current scope.
    2. Search the entry in the current symbol table at first.
    3. If it's not in the current table, search it in previous ones(along the 'prev' link).
    4. If you find the entry, return it.
    5. If you can't find it in all symbol tables, return nullptr.
*/
SymbolEntry* SymbolTable::lookup(std::string name)
{
    // Todo
    //Modify finished bd
    if (symbolTable.count(name) != 0)
    {
        return symbolTable[name];
    }
    else if(prev!=nullptr)
    {
        return prev->lookup(name);
    }
    else return nullptr;    
    // return nullptr;
}

// install the entry into current symbol table.
void SymbolTable::install(std::string name, SymbolEntry* entry)
{
    symbolTable[name] = entry;
}

//Modify 重定义 finished bd
bool SymbolTable::isRedef(std::string name)
{
    if (symbolTable.count(name) == 1)
    {
        return true;
    }
    else return false;    
}

bool SymbolTable::isFuncRedef(std::string name, std::vector<Type*> paramlist)
{
    int num = paramlist.size();
    for(auto iter:symbolTable)
    {
        std::string NAME = iter.first;
        SymbolEntry* SE = iter.second;

        if(NAME == name)
        {
            std::vector<Type*> paramsTYPE = ((FunctionType*)SE->getType())->getParamsType();
            int NUM =  paramsTYPE.size();

            if(num == 0 && NUM == 0) return true;

            if(num == NUM)
            {
                bool isSame = true;
                for (int i=0; i<NUM; i++)
                {
                    if( paramsTYPE[i]->getKind() != paramlist[i]->getKind())
                    {
                        isSame = false;
                    }
                }
                if(isSame == true) return true;
            
            }
        }
    }
    return false;
}

int SymbolTable::counter = 0;
static SymbolTable t;
SymbolTable *identifiers = &t;
SymbolTable *globals = &t;
