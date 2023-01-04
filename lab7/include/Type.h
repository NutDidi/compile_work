#ifndef __TYPE_H__
#define __TYPE_H__
#include <vector>
#include <string>

class Type
{
private:
    int kind;
protected:
    enum {INT, VOID, FUNC, PTR, BOOL, ConstInt}; //lab6
    int size; //bd lab7
public:
    Type(int kind , int size) : kind(kind), size(size) {};
    Type(int kind ) : kind(kind), size(0) {};
    virtual ~Type() {};
    virtual std::string toStr() = 0;
    bool isInt() const {return kind == INT||kind == ConstInt;};  //lab6
    bool isVoid() const {return kind == VOID;};
    bool isFunc() const {return kind == FUNC;};
    //lab6
    bool isBool() const {return kind == BOOL;};
    bool isConstInt() const {return kind == ConstInt;};
    bool isConst() const {return kind == ConstInt;};
    int  getKind() const {return kind;};
    //bd lab7
    int getSize() const {return size;};
};
//bd lab7
class IntType : public Type
{
public:
    IntType(int size) : Type(Type::INT, size){};
    std::string toStr();
};

class VoidType : public Type
{
public:
    VoidType() : Type(Type::VOID){};
    std::string toStr();
};

class FunctionType : public Type
{
private:
    Type *returnType;
    std::vector<Type*> paramsType;
public:
    FunctionType(Type* returnType, std::vector<Type*> paramsType) : 
    Type(Type::FUNC), returnType(returnType), paramsType(paramsType){};
    Type* getRetType() {return returnType;};
    std::string toStr();
    //lab6
    void setparamsType(std::vector<Type*>);
    std::vector<Type*> getParamsType() {return this->paramsType;}
};

class PointerType : public Type
{
private:
    Type *valueType;
public:
    PointerType(Type* valueType) : Type(Type::PTR) {this->valueType = valueType;};
    std::string toStr();
    //lab6
    Type* getValueType() {return this->valueType;};
};
//bd lab7
//lab6
class BoolType : public Type
{
public:
    BoolType(int size) : Type(Type::BOOL , size){};
    std::string toStr();
};

//lab6
class ConstIntType : public Type
{
public:
    ConstIntType() : Type(Type::INT){};//bd
    std::string toStr();
};

class TypeSystem
{
private:
    static IntType commonInt;
    static VoidType commonVoid;
    //lab6
    static BoolType commonBool;
    static ConstIntType commonConstInt;    
public:
    static Type *intType;
    static Type *voidType;
    static Type *boolType;
    static Type *constIntType; //lab6
    //lab6
    static Type* getMaxType(Type* type1, Type* type2);
    static bool needCast(std::string src, std::string target);
};

#endif
