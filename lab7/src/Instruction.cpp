#include "Instruction.h"
#include "BasicBlock.h"
#include <iostream>
#include "Function.h"
#include "Type.h"
#include <vector>
extern FILE* yyout;

Instruction::Instruction(unsigned instType, BasicBlock *insert_bb)
{
    prev = next = this;
    opcode = -1;
    this->instType = instType;
    if (insert_bb != nullptr)
    {
        insert_bb->insertBack(this);
        parent = insert_bb;
    }
}

Instruction::~Instruction()
{
    parent->remove(this);
}

BasicBlock *Instruction::getParent()
{
    return parent;
}

void Instruction::setParent(BasicBlock *bb)
{
    parent = bb;
}

void Instruction::setNext(Instruction *inst)
{
    next = inst;
}

void Instruction::setPrev(Instruction *inst)
{
    prev = inst;
}

Instruction *Instruction::getNext()
{
    return next;
}

Instruction *Instruction::getPrev()
{
    return prev;
}

BinaryInstruction::BinaryInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb) : Instruction(BINARY, insert_bb)
{
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

BinaryInstruction::~BinaryInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void BinaryInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[0]->getType()->toStr();
    switch (opcode)
    {
    case ADD:
        op = "add";
        break;
    case SUB:
        op = "sub";
        break;
    case MUL:
        op = "mul";
        break;
    case DIV:
        op = "sdiv";
        break;
    case MOD:
        op = "srem";
        break;
    default:
        break;
    }
    fprintf(yyout, "  %s = %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

CmpInstruction::CmpInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb): Instruction(CMP, insert_bb){
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

CmpInstruction::~CmpInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void CmpInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[1]->getType()->toStr();
    switch (opcode)
    {
    case E:
        op = "eq";
        break;
    case NE:
        op = "ne";
        break;
    case L:
        op = "slt";
        break;
    case LE:
        op = "sle";
        break;
    case G:
        op = "sgt";
        break;
    case GE:
        op = "sge";
        break;
    default:
        op = "";
        break;
    }

    fprintf(yyout, "  %s = icmp %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

UncondBrInstruction::UncondBrInstruction(BasicBlock *to, BasicBlock *insert_bb) : Instruction(UNCOND, insert_bb)
{
    branch = to;
}

void UncondBrInstruction::output() const
{
    fprintf(yyout, "  br label %%B%d\n", branch->getNo());
}

void UncondBrInstruction::setBranch(BasicBlock *bb)
{
    branch = bb;
}

BasicBlock *UncondBrInstruction::getBranch()
{
    return branch;
}

CondBrInstruction::CondBrInstruction(BasicBlock*true_branch, BasicBlock*false_branch, Operand *cond, BasicBlock *insert_bb) : Instruction(COND, insert_bb){
    this->true_branch = true_branch;
    this->false_branch = false_branch;
    cond->addUse(this);
    operands.push_back(cond);
}

CondBrInstruction::~CondBrInstruction()
{
    operands[0]->removeUse(this);
}

void CondBrInstruction::output() const
{
    std::string cond, type;
    cond = operands[0]->toStr();
    type = operands[0]->getType()->toStr();
    int true_label = true_branch->getNo();
    int false_label = false_branch->getNo();
    fprintf(yyout, "  br %s %s, label %%B%d, label %%B%d\n", type.c_str(), cond.c_str(), true_label, false_label);
}

void CondBrInstruction::setFalseBranch(BasicBlock *bb)
{
    false_branch = bb;
}

BasicBlock *CondBrInstruction::getFalseBranch()
{
    return false_branch;
}

void CondBrInstruction::setTrueBranch(BasicBlock *bb)
{
    true_branch = bb;
}

BasicBlock *CondBrInstruction::getTrueBranch()
{
    return true_branch;
}

RetInstruction::RetInstruction(Operand *src, BasicBlock *insert_bb) : Instruction(RET, insert_bb)
{
    if(src != nullptr)
    {
        operands.push_back(src);
        src->addUse(this);
    }
}

RetInstruction::~RetInstruction()
{
    if(!operands.empty())
        operands[0]->removeUse(this);
}

void RetInstruction::output() const
{
    if(operands.empty())
    {
        fprintf(yyout, "  ret void\n");
    }
    else
    {
        std::string ret, type;
        ret = operands[0]->toStr();
        type = operands[0]->getType()->toStr();
        fprintf(yyout, "  ret %s %s\n", type.c_str(), ret.c_str());
    }
}

AllocaInstruction::AllocaInstruction(Operand *dst, SymbolEntry *se, BasicBlock *insert_bb) : Instruction(ALLOCA, insert_bb)
{
    operands.push_back(dst);
    dst->setDef(this);
    this->se = se;
}

AllocaInstruction::~AllocaInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
}

void AllocaInstruction::output() const
{
    std::string dst, type;
    dst = operands[0]->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "  %s = alloca %s, align 4\n", dst.c_str(), type.c_str());
}

LoadInstruction::LoadInstruction(Operand *dst, Operand *src_addr, BasicBlock *insert_bb) : Instruction(LOAD, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src_addr);
    dst->setDef(this);
    src_addr->addUse(this);
}

LoadInstruction::~LoadInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}

void LoadInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string src_type;
    std::string dst_type;
    dst_type = operands[0]->getType()->toStr();
    src_type = operands[1]->getType()->toStr();
    fprintf(yyout, "  %s = load %s, %s %s, align 4\n", dst.c_str(), dst_type.c_str(), src_type.c_str(), src.c_str());
}

StoreInstruction::StoreInstruction(Operand *dst_addr, Operand *src, BasicBlock *insert_bb) : Instruction(STORE, insert_bb)
{
    operands.push_back(dst_addr);
    operands.push_back(src);
    dst_addr->addUse(this);
    src->addUse(this);
}

StoreInstruction::~StoreInstruction()
{
    operands[0]->removeUse(this);
    operands[1]->removeUse(this);
}

void StoreInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string dst_type = operands[0]->getType()->toStr();
    std::string src_type = operands[1]->getType()->toStr();

    fprintf(yyout, "  store %s %s, %s %s, align 4\n", src_type.c_str(), src.c_str(), dst_type.c_str(), dst.c_str());
}

//lab6 begin
CallInstruction::CallInstruction(Operand *dst, IdentifierSymbolEntry* func, std::vector<Operand*> params, BasicBlock *insert_bb) : Instruction(CALL, insert_bb)
{
    operands.push_back(dst);//返回值，目标寄存器
    dst->setDef(this);
    for(auto param : params){//参数，源寄存器
        operands.push_back(param);
        param->addUse(this);
    }
    funcse = func;
}

CallInstruction::~CallInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    for(int i = 1;i<(int)operands.size(); i++){
        operands[i]->removeUse(this);
    }
}

void CallInstruction::output() const
{
    //只输出call语句前半部分：call void @putch(
    std::string dst = operands[0]->toStr();
    std::string dst_type;
    dst_type = operands[0]->getType()->toStr();
    Type* returnType = dynamic_cast<FunctionType*>(funcse->getType())->getRetType();
    if(!returnType->isVoid()){//仅当返回值为非void时，向临时寄存器赋值
        fprintf(yyout, "  %s =", dst.c_str());
    }
    fprintf(yyout, "  call %s %s(", dst_type.c_str(), funcse->toStr().c_str());
    for(int i = 1;i<(int)operands.size(); i++){
        std::string src = operands[i]->toStr();
        std::string src_type = operands[i]->getType()->toStr();
        if(i!=1){
            fprintf(yyout, ", ");
        }
        fprintf(yyout, "%s %s", src_type.c_str(), src.c_str());
    }
    fprintf(yyout, ")\n");
}

ZextInstruction::ZextInstruction ( Operand *src,Operand *dst,  BasicBlock *insert_bb) : Instruction(ZEXT,insert_bb)
{

    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

ZextInstruction::~ZextInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}

void ZextInstruction ::output() const
{    
    Operand* dst=operands[0];
    Operand* src=operands[1];
    fprintf(yyout,"  %s = zext i1 %s to i32\n",dst->toStr().c_str(),src->toStr().c_str() );
}
//lab6 end

//modify  bd const 参数 unfinfish
MachineOperand* Instruction::genMachineOperand(Operand* ope)
{
    auto se = ope->getEntry();
    MachineOperand* mope = nullptr;
    if(se->isConstant()) //常数
    {
        // fprintf(stderr,"MachineOperand se->isConstant()\n");
        mope = new MachineOperand(MachineOperand::IMM, dynamic_cast<ConstantSymbolEntry*>(se)->getValue());
    }
        
    else if(se->isTemporary())  //临时
    {
        // fprintf(stderr,"MachineOperand se->se->isTemporary()\n");
        // fprintf(stderr,"dynamic_cast<TemporarySymbolEntry*>(se)->getLabel() %d\n",dynamic_cast<TemporarySymbolEntry*>(se)->getLabel());
        mope = new MachineOperand(MachineOperand::VREG, dynamic_cast<TemporarySymbolEntry*>(se)->getLabel());
    }
       
    else if(se->isVariable()) //
    {
        // fprintf(stderr,"MachineOperand se->isVariable()");
        auto id_se = dynamic_cast<IdentifierSymbolEntry*>(se);
        if(id_se->isGlobal())  //全局变量
        {
            // fprintf(stderr,"  id_se->isGlobal()\n");
            mope = new MachineOperand(id_se->toStr().c_str());
        }
        else 
        if(id_se->isParam())  //函数参数 4个 0-3号物理寄存器
        {
            // fprintf(stderr,"  id_se->isParam()\n");
            int index = 0;
            for (auto param : this->parent->getParent()->params)
            {
                if(ope == param) break;
                index++;
            }

            if (index < this->parent->getParent()->params.size())
            {
                if(index < 4)
                {
                    mope = new MachineOperand(MachineOperand::REG, index);
                }
                else
                {
                    //quest 超过四个参数怎么办？
                    mope = new MachineOperand(MachineOperand::REG, 3);
                }
            }
        }
        else //quest?局部变量中有const吧应该，
        if(id_se->getType() == TypeSystem::constIntType)
        {
            // fprintf(stderr,"  id_se->getType()\n");
            mope = new MachineOperand(MachineOperand::IMM, id_se->value);
        }
        else
        {
            //quest 这是干啥呢
            // fprintf(stderr,"  else()\n");
            mope = new MachineOperand(id_se->toStr().c_str());
        }
    }
    return mope;
}

MachineOperand* Instruction::genMachineReg(int reg) 
{
    return new MachineOperand(MachineOperand::REG, reg);
}

MachineOperand* Instruction::genMachineVReg() 
{
    fprintf(stderr,"Instruction::genMachineVReg\n");
    return new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel());
}

MachineOperand* Instruction::genMachineImm(int val) 
{
    return new MachineOperand(MachineOperand::IMM, val);
}

MachineOperand* Instruction::genMachineLabel(int block_no)
{
    std::ostringstream buf;
    buf << ".L" << block_no;
    std::string label = buf.str();
    return new MachineOperand(label);
}

void AllocaInstruction::genMachineCode(AsmBuilder* builder)
{
    /* HINT:
    * Allocate stack space for local variabel
    * Store frame offset in symbol entry */
   //为所有局部变量分配空间 并保存偏移量
    auto cur_func = builder->getFunction();
    int offset = cur_func->AllocSpace(4);
    dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->setOffset(-offset);
}
//modify bd 函数参数  MachineOperand LoadMInstruction
void LoadInstruction::genMachineCode(AsmBuilder* builder)
{
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    // Load global operand
    //全局变量或者常量
    if(operands[1]->getEntry()->isVariable() 
    && dynamic_cast<IdentifierSymbolEntry*>(operands[1]->getEntry())->isGlobal())
    {
        fprintf(stderr,"LoadInstruction case1\n");
        auto dst = genMachineOperand(operands[0]);
        fprintf(stderr,"genMachineVReg9\n");
        auto internal_reg1 = genMachineVReg(); 
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        auto src = genMachineOperand(operands[1]);
        // example: load r0, addr_a
        //在全局的地址标签中将其地址加载到寄存器中
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, src);
        cur_block->InsertInst(cur_inst);
        // example: load r1, [r0]
        //该地址中加载出其实际的值
        cur_inst = new LoadMInstruction(cur_block, dst, internal_reg2);
        cur_block->InsertInst(cur_inst);
    }
    // Load operand from temporary variable
    //load 临时变量 
    else if(operands[1]->getEntry()->isTemporary()
    && operands[1]->getDef() //有定义它的指令
    && operands[1]->getDef()->isAlloc())//分配了空间
    {
        // example: load r1, [r0, #4]
        fprintf(stderr,"LoadInstruction case2\n");
        auto dst = genMachineOperand(operands[0]);
        auto src1 = genMachineReg(11);//FP吧应该是
        auto src2 = genMachineImm(dynamic_cast<TemporarySymbolEntry*>(operands[1]->getEntry())->getOffset());//存偏移量的吧
        cur_inst = new LoadMInstruction(cur_block, dst, src1, src2);
        cur_block->InsertInst(cur_inst);
        //modify 保留函数参数偏移量
        //quest 没啥问题就是注意一下存这里了
        if (dynamic_cast<TemporarySymbolEntry*>(operands[1]->getEntry())->getOffset()>= 0) 
        {
            cur_block->getParent()->SaveParamOffset(src2);
        }
    }
    // Load local operand
    //load局部操作数
    else
    {
        // example: load r1, [r0]
        fprintf(stderr,"LoadInstruction case3\n");
        auto dst = genMachineOperand(operands[0]);
        auto src = genMachineOperand(operands[1]);
        cur_inst = new LoadMInstruction(cur_block, dst, src);
        cur_block->InsertInst(cur_inst);
    }
}
//modify bd LoadMInstruction StoreMInstruction
void StoreInstruction::genMachineCode(AsmBuilder* builder)
{
    //operands[0]是目的操作数

    // TODO
    //函数参数 （>=4的）
    if(operands[1]->getEntry()->isVariable())
    {
        auto id_se = dynamic_cast<IdentifierSymbolEntry*>(operands[1]->getEntry());
        if(id_se->isParam())
        {
            int index = 0;
            for (auto param : this->parent->getParent()->params)
            {
                if(operands[1] == param) break;
                index++;
            }

            if (index < this->parent->getParent()->params.size())
            {
                if(index >= 4)
                {
                    index = index-4;
                    dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->setOffset(4*index);
                    return;
                }
            }            
        }
        
    }
   
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;

    //quest operands[1]有可能为立即数吗？？
    auto src = genMachineOperand(operands[1]);
    if(operands[1]->getEntry()->isConstant())
    {
        fprintf(stderr,"genMachineVReg8\n");
        auto internal_reg1 = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, src);
        cur_block->InsertInst(cur_inst);
        src = new MachineOperand(*internal_reg1);
    }
   
    // store global operand
    //全局变量或者常量
    if(operands[0]->getEntry()->isVariable() 
    && dynamic_cast<IdentifierSymbolEntry*>(operands[0]->getEntry())->isGlobal())
    {
        fprintf(stderr,"StoreInstruction case1\n");
        auto dst = genMachineOperand(operands[0]);
        fprintf(stderr,"genMachineVReg7\n");
        auto internal_reg1 = genMachineVReg(); 
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        // example: load r0, addr_a
        //在全局的地址标签中将其地址加载到寄存器中
        //quest 不明白啊 要存的是operands[1] 它是一个全局变量 所以不应该先load它吗？？？？？ 但是这里应该是对的
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, dst); //dst load进 internal_reg
        cur_block->InsertInst(cur_inst);
        // example: store r1, [r0]
        cur_inst = new StoreMInstruction(cur_block, src, internal_reg2); //src 存入 internal_reg
        cur_block->InsertInst(cur_inst);
    }
    // store local operand
    else if(operands[0]->getEntry()->isTemporary()
    && operands[0]->getDef() 
    && operands[0]->getDef()->isAlloc())
    {
        // example: store r1, [r0, #4] 把r1放入r0+4
        // auto src = genMachineOperand(operands[1]);
        fprintf(stderr,"StoreInstruction case2\n");
        auto dst0 = genMachineReg(11); //FP
        auto dst = genMachineImm(dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->getOffset());
        //quest是否要考虑偏移量溢出？？
        if(abs(dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->getOffset()) > 255)
        {
            fprintf(stderr,"genMachineVReg6\n");
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, internal_reg, dst);
            cur_block->InsertInst(cur_inst);
            dst = new MachineOperand(*internal_reg);
        }
        cur_inst = new StoreMInstruction(cur_block, src, dst0, dst); 
        cur_block->InsertInst(cur_inst);
  
    }  
    // store operand from temporary variable
    //store 临时变量 
    else
    {
        fprintf(stderr,"StoreInstruction case3\n");
        // example: load r1, [r0]  r1 存入 r0
        auto dst = genMachineOperand(operands[0]);
        // auto src = genMachineOperand(operands[1]);
        cur_inst = new StoreMInstruction(cur_block, src, dst);
        cur_block->InsertInst(cur_inst);
    }

}
//modify bd LoadMInstruction MachineOperand BinaryMInstruction
void BinaryInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO:
    // complete other instructions
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src1 = genMachineOperand(operands[1]);
    auto src2 = genMachineOperand(operands[2]);
    /* HINT:
    * The source operands of ADD instruction in ir code both can be immediate num.
    * However, it's not allowed in assembly code.
    * So you need to insert LOAD/MOV instrucrion to load immediate num into register.
    * As to other instructions, such as MUL, CMP, you need to deal with this situation, too.*/
    MachineInstruction* cur_inst = nullptr;
    //是立即数的话要先load进来
    if(src1->isImm())
    {
        fprintf(stderr,"genMachineVReg5\n");
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    //mul and or可不可以第二个操作数是立即数
    if(opcode != ADD && opcode != SUB)
    {
        if (src2->isImm()) 
        {
            fprintf(stderr,"genMachineVReg4\n");
            auto temp_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(cur_block, temp_reg, src2);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*temp_reg);
        }
    }

    //quest立即数溢出需要吗？？？
    // if (src2->isImm() && abs(src2->getVal())>255)
    // {
    //     auto internal_reg = genMachineVReg();
    //     cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
    //     cur_block->InsertInst(cur_inst);
    //     src2 = new MachineOperand(*internal_reg);        
    // }

    MachineOperand* product;
    switch (opcode)
    {
    case ADD:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD, dst, src1, src2);
        break;
    case SUB:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, src1, src2);
        break;
    case MUL:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, dst, src1, src2);
        break;
    case DIV:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst, src1, src2);
        break;
    case MOD:
        //dst =src1/src2
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst, src1, src2);
        cur_block->InsertInst(cur_inst);
        //product =dst*src2
        product = new MachineOperand(*dst);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, product, dst, new MachineOperand(*src2));
        cur_block->InsertInst(cur_inst);
        //dst = src1-product
        dst = new MachineOperand(*product);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, new MachineOperand(*src1), product);
        break;
    case AND:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::AND, dst, src1, src2);
        break;
    case OR:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::OR, dst, src1, src2);
        break;
    default:
        break;
    }
    cur_block->InsertInst(cur_inst);
}
//modify bd LoadMInstruction MachineOperand MovMInstruction CmpMInstruction
void CmpInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    MachineBlock* cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;

    cur_block->Cond = opcode;

    MachineOperand* src1 = genMachineOperand(operands[1]);
    MachineOperand* src2 = genMachineOperand(operands[2]);
    //是立即数的话要先load进来
    if(src1->isImm())
    {
        fprintf(stderr,"genMachineVReg3\n");
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    if (src2->isImm()) 
    {
        fprintf(stderr,"genMachineVReg2\n");
        auto temp_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, temp_reg, src2);
        cur_block->InsertInst(cur_inst);
        src2 = new MachineOperand(*temp_reg);
    }

    cur_inst = new CmpMInstruction(cur_block, src1, src2, opcode);
    cur_block->InsertInst(cur_inst);
    //condType 0EQ==, 1NE!=, 2LT<, 3LE<= , 4GT>, 5GE>=, NONE };
    //opcode   E, NE, L, LE, G, GE
    //quest 这mov啥呢

        
        int true_op = opcode;
        int false_op; 
        if(opcode!=E && opcode!=NE) false_op = 7-opcode;
        else false_op = 1-opcode;
        fprintf(stderr,"in instruction opcode is %d\n",opcode);
        // fprintf(stderr,"opcode is %d\n",opcode);
        // fprintf(stderr,"opcode is %d\n",opcode);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, genMachineOperand(operands[0]), genMachineImm(1), opcode);
        cur_block->InsertInst(cur_inst);
        if(opcode!=E && opcode!=NE)
            cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, genMachineOperand(operands[0]), genMachineImm(0), 7-opcode);
        else
            cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, genMachineOperand(operands[0]), genMachineImm(0), 1-opcode);
        cur_block->InsertInst(cur_inst);

}
//modify bd BranchMInstruction MachineOperand
void UncondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    MachineBlock* cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    std::ostringstream label;
    label <<  ".L" << branch->getNo();
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, new MachineOperand(label.str()));
    cur_block->InsertInst(cur_inst);

}
//modify bd BranchMInstruction MachineOperand
void CondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    MachineBlock* cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;

    std::ostringstream Tlabel;
    Tlabel <<  ".L" << true_branch->getNo();
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, new MachineOperand(Tlabel.str()), cur_block->Cond);   
    cur_block->InsertInst(cur_inst);
    
    std::ostringstream Flabel;
    Flabel <<  ".L" << false_branch->getNo();
   cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, new MachineOperand(Flabel.str()));
    cur_block->InsertInst(cur_inst);
}
//modify yxy LoadMInstruction BranchMInstruction MachineOperand MovMInstruction
void RetInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    /* HINT:
    * 1. Generate mov instruction to save return value in r0
    * 2. Restore callee saved registers and sp, fp
    * 3. Generate bx instruction */

    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;

    //当函数有返回值时
    if(!operands.empty()){
        auto src = genMachineOperand(operands[0]);
        auto dst = new MachineOperand(MachineOperand::REG, 0);//r0
        //将返回值保存在 R0 寄存器中
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
        cur_block->InsertInst(cur_inst);
    }
    // 生成一条跳转到结尾函数栈帧处理的无条件跳转语句
    auto dst = new MachineOperand(".L" + this->getParent()->getParent()->getSymPtr()->toStr().erase(0,1) + "_END");
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, dst);
    cur_block->InsertInst(cur_inst);
}
//modify yxy MachineOperand MovMInstruction StackMInstruction BranchMInstruction
void CallInstruction::genMachineCode(AsmBuilder* builder)
{
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    for(unsigned int i = operands.size() - 1; i > 0; i--){
        //左起前4个参数通过r0-r3传递
        if(i<=4){
            fprintf(stderr,"i<=4 \n");
            auto dst = new MachineOperand(MachineOperand::REG, i-1);//r0-r3
            cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, genMachineOperand(operands[i]));
            cur_block->InsertInst(cur_inst);
        }
        else{
            MachineOperand* operand = genMachineOperand(operands[i]);
            if(operand->isImm()) { //如果是立即数先Load出来
                fprintf(stderr,"genMachineVReg1\n");
                MachineOperand* internal_reg = genMachineVReg();
                cur_inst = new LoadMInstruction(cur_block, internal_reg, operand);
                cur_block->InsertInst(cur_inst);
                operand = new MachineOperand(*internal_reg);
            }
            cur_inst = new StackMInstruction(cur_block, StackMInstruction::PUSH, operand); //yxy 
            cur_block->InsertInst(cur_inst);
        }
    }
    //生成跳转指令来进入 Callee 函数；
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::BL, new MachineOperand(funcse->getName(),true));
    cur_block->InsertInst(cur_inst);

    //现场恢复的工作
    // 恢复 SP 寄存器
    if(operands.size() > 5)
    {
        auto sp = new MachineOperand(MachineOperand::REG, 13);
        auto src = genMachineImm((operands.size() - 5) * 4);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD, sp, sp, src);
        cur_block->InsertInst(cur_inst);
    }

    // 保存 R0 寄存器中的返回值
    if(dynamic_cast<FunctionType*>(this->funcse->getType())->getRetType() != TypeSystem::voidType) {
        auto dst = genMachineOperand(operands[0]);
        auto src = new MachineOperand(MachineOperand::REG, 0);//r0
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
        cur_block->InsertInst(cur_inst);
    }
}
//modify yxy LoadMInstruction MachineOperand ZextMInstruction
void ZextInstruction::genMachineCode(AsmBuilder* builder)
{
    MachineBlock* cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;

    MachineOperand* src = genMachineOperand(operands[1]);
    MachineOperand* dst = genMachineOperand(operands[0]);
    cur_inst = new ZextMInstruction(cur_block, dst, src);
    cur_block->InsertInst(cur_inst);
}