#ifndef __UNIT_H__
#define __UNIT_H__

#include <vector>
#include "Function.h"
#include "AsmBuilder.h"

class Unit
{
    typedef std::vector<Function *>::iterator iterator;
    typedef std::vector<Function *>::reverse_iterator reverse_iterator;

private:
    std::vector<Function *> func_list;
    //lab6
    std::vector<IdentifierSymbolEntry *> global_list;  //全局变量
    std::vector<IdentifierSymbolEntry *> libfunc_list; //库函数
public:
    Unit() = default;
    ~Unit() ;
    void insertFunc(Function *);
    void removeFunc(Function *);
    void output() const;
    iterator begin() { return func_list.begin(); };
    iterator end() { return func_list.end(); };
    reverse_iterator rbegin() { return func_list.rbegin(); };
    reverse_iterator rend() { return func_list.rend(); };
    void genMachineCode(MachineUnit* munit);
    //lab6
    void insertGlobal(IdentifierSymbolEntry *);  //全局变量
    void insertLibfunc(IdentifierSymbolEntry *); //库函数
};

#endif