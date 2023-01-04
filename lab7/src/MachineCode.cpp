#include "MachineCode.h"
#include "Type.h"
extern FILE* yyout;

//
int MachineBlock::spilt_label = 0;

MachineOperand::MachineOperand(int tp, int val)
{
    this->type = tp;
    if(tp == MachineOperand::IMM)
        this->val = val;
    else 
        this->reg_no = val;
}
//modify 函数 
//quest 改了output这好像不用改了
MachineOperand::MachineOperand(std::string label, bool func)
{

    this->type = MachineOperand::LABEL;
    this->label = label;
    this->func = func;
}

bool MachineOperand::operator==(const MachineOperand&a) const
{
    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

bool MachineOperand::operator<(const MachineOperand&a) const
{
    if(this->type == a.type)
    {
        if(this->type == IMM)
            return this->val < a.val;
        return this->reg_no < a.reg_no;
    }
    return this->type < a.type;

    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

void MachineOperand::PrintReg()
{
    switch (reg_no)
    {
    case 11:
        fprintf(yyout, "fp");
        break;
    case 13:
        fprintf(yyout, "sp");
        break;
    case 14:
        fprintf(yyout, "lr");
        break;
    case 15:
        fprintf(yyout, "pc");
        break;
    default:
        fprintf(yyout, "r%d", reg_no);
        break;
    }
}

//bd 函数定义输出 finish
void MachineOperand::output() 
{
    /* HINT：print operand
    * Example:
    * immediate num 1 -> print #1;
    * register 1 -> print r1;
    * lable addr_a -> print addr_a; */
    switch (this->type)
    {
    case IMM:
        fprintf(yyout, "#%d", this->val);
        break;
    case VREG:
        fprintf(yyout, "v%d", this->reg_no);
        break;
    case REG:
        PrintReg();
        break;
    case LABEL:
        if (this->label.substr(0, 2) == ".L" )
            fprintf(yyout, "%s", this->label.c_str());
        // //bd 函数定义输出 quest string+1是啥，我不理解
        else if (func)
            fprintf(yyout, "%s", this->label.c_str());
        else
            fprintf(yyout, "addr_%s_%d", this->label.erase(0,1).c_str() ,parent->getParent()->getParent()->getParent()->getno());
    default:
        break;
    }
}

//modify 补全 bd finish
void MachineInstruction::PrintCond()
{
    // TODO
    //{ EQ, NE, LT, LE , GT, GE, NONE }
    fprintf(stderr,"in machine opcode is %d\n",cond);
    switch (cond)
    {
        case EQ:
            fprintf(yyout, "eq");
            break;
        case NE:
            fprintf(yyout, "ne");
            break;
        case LT:
            fprintf(yyout, "lt");
            break;
        case LE:
            fprintf(yyout, "le");
            break;
        case GT:
            fprintf(yyout, "gt");
            break;
        case GE:
            fprintf(yyout, "ge");
            break;
        default:
            break;
    }
}

BinaryMInstruction::BinaryMInstruction(MachineBlock* p, int op, MachineOperand* dst, MachineOperand* src1, MachineOperand* src2, int cond)
{
    this->parent = p;
    this->type = MachineInstruction::BINARY;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    src2->setParent(this);
}

//modify 补全 bd finish
void BinaryMInstruction::output() 
{
    // TODO: 
    // Complete other instructions
    switch (this->op)
    {
    case BinaryMInstruction::ADD:
        fprintf(yyout, "\tadd ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::SUB:
        fprintf(yyout, "\tsub ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::MUL:
        fprintf(yyout, "\tmul ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;    
    case BinaryMInstruction::DIV:
        fprintf(yyout, "\tsdiv ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::AND:
        fprintf(yyout, "\tand ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    case BinaryMInstruction::OR:
        fprintf(yyout, "\torr ");
        this->PrintCond();
        this->def_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[0]->output();
        fprintf(yyout, ", ");
        this->use_list[1]->output();
        fprintf(yyout, "\n");
        break;
    default:
        break;
    }
}

LoadMInstruction::LoadMInstruction(MachineBlock* p,MachineOperand* dst, MachineOperand* src1, MachineOperand* src2,int cond)
{
    this->parent = p;
    this->type = MachineInstruction::LOAD;
    this->op = -1;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    if (src2)
        this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    if (src2)
        src2->setParent(this);
}

void LoadMInstruction::output()
{
    fprintf(yyout, "\tldr ");
    this->def_list[0]->output();
    // fprintf(stderr,"MachineOperand se->se->isTemporary()\n");
    fprintf(yyout, ", ");

    // Load immediate num, eg: ldr r1, =8
    if(this->use_list[0]->isImm())
    {
        fprintf(yyout, "=%d\n", this->use_list[0]->getVal());
        return;
    }

    // Load address
    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "[");

    this->use_list[0]->output();
    if( this->use_list.size() > 1 )
    {
        fprintf(yyout, ", ");
        this->use_list[1]->output();
    }

    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "]");

    fprintf(yyout, "\n");
}
//modify bd finish
StoreMInstruction::StoreMInstruction(MachineBlock* p,MachineOperand* src1, MachineOperand* src2, MachineOperand* src3, int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::STORE;
    this->op = -1;
    this->cond = cond;
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    if (src3)
        this->use_list.push_back(src3);
    src1->setParent(this);
    src2->setParent(this);
    if (src3)
        src3->setParent(this);
}
//modify bd finish
void StoreMInstruction::output()
{
    // TODO
    fprintf(yyout, "\tstr ");

    this->use_list[0]->output();
    fprintf(yyout, ", ");

    // Store address
    if(this->use_list[1]->isReg()||this->use_list[1]->isVReg())
        fprintf(yyout, "[");

    this->use_list[1]->output();
    if( this->use_list.size() > 2 )
    {
        fprintf(yyout, ", ");
        this->use_list[2]->output();
    }

    if(this->use_list[1]->isReg()||this->use_list[1]->isVReg())
        fprintf(yyout, "]");

    fprintf(yyout, "\n");
}


//modify yxy
MovMInstruction::MovMInstruction(MachineBlock* p, int op, 
    MachineOperand* dst, MachineOperand* src,
    int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::MOV;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src);
    dst->setParent(this);
    src->setParent(this);
}
//modify yxy
void MovMInstruction::output() 
{
    // TODO
    fprintf(yyout, "\tmov");
    PrintCond();
    fprintf(yyout, " ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, "\n");
}



//modify bd finish
BranchMInstruction::BranchMInstruction(MachineBlock* p, int op, MachineOperand* dst, int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::BRANCH;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    dst->setParent(this);    
}
//modify bd finish
void BranchMInstruction::output()
{
    // TODO
switch (this->op)
    {
        //B, BL, BX
    case BranchMInstruction::B:
        fprintf(yyout, "\tb");
        this->PrintCond();
        fprintf(yyout, " ");
        this->def_list[0]->output();
        fprintf(yyout, "\n");
        break;
    case BranchMInstruction::BL:
        fprintf(yyout, "\tbl ");
        this->PrintCond();
        fprintf(yyout, " ");
        this->def_list[0]->output();
        fprintf(yyout, "\n");
        break;
    case BranchMInstruction::BX:
        fprintf(yyout, "\tbx ");
        this->PrintCond();
        fprintf(yyout, " ");
        this->def_list[0]->output();
        fprintf(yyout, "\n");
        break;
    default:
        break;
    }
}
//modify bd finish
CmpMInstruction::CmpMInstruction(MachineBlock* p, MachineOperand* src1, MachineOperand* src2, int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::CMP;
    this->cond = cond;
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    src1->setParent(this);
    src2->setParent(this);  
}
//modify bd finish
void CmpMInstruction::output()
{
    // TODO
    // Jsut for reg alloca test
    // delete it after test
    fprintf(yyout, "\tcmp ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
}



//modify yxy
StackMInstruction::StackMInstruction(MachineBlock* p, int op, 
    MachineOperand* src,
    int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::STACK;
    this->op = op;
    this->cond = cond;
    this->use_list.push_back(src);
    for(auto reg : use_list)
    {
        reg->setParent(this);
    }
}
StackMInstruction::StackMInstruction(MachineBlock* p, int op, 
                std::vector<MachineOperand*> src,
                int cond )
{
    this->parent = p;
    this->type = MachineInstruction::STACK;
    this->op = op;
    this->cond = cond;
    this->use_list = src;
    for(auto reg : use_list){
        reg->setParent(this);
    }
}

//modify yxy
void StackMInstruction::output()
{
    // TODO
    if(use_list.size()==0) return;
    switch(op){
    case PUSH:
        fprintf(yyout, "\tpush ");
        break;
    case POP:
        fprintf(yyout, "\tpop ");
        break;
    }
    fprintf(yyout, "{");
    unsigned int index = 0;
    while (index < use_list.size()) 
    {
        if(index == 0)
        {
            this->use_list[index]->output();
        }
        else
        {
            fprintf(yyout, ", ");
            this->use_list[index]->output();  
        } 
        index++;
    }
    fprintf(yyout, "}\n");
}

//modify zext指令 yxy
ZextMInstruction::ZextMInstruction(MachineBlock *p, MachineOperand *dst, MachineOperand *src, int cond) {
    this->parent = p;
    this->type = MachineInstruction::ZEXT;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src);
    dst->setParent(this);
    src->setParent(this);
}

void ZextMInstruction::output() {
    fprintf(yyout, "\tuxtb ");
    def_list[0]->output();
    fprintf(yyout, ", ");
    use_list[0]->output();
    fprintf(yyout, "\n");
}



MachineFunction::MachineFunction(MachineUnit* p, SymbolEntry* sym_ptr) 
{ 
    this->parent = p; 
    this->sym_ptr = sym_ptr; 
    this->stack_size = 0;
};

 

//modify 空块 undo
//quest
void MachineBlock::output()
{
    fprintf(yyout, ".L%d:\n", this->no);
    if(inst_list.size() != 0)
    {
        int Num = 0;
        for(auto iter : inst_list)
        {
            iter->output();  
            Num++;
            
        }
        //quest >500块分裂吗???真的用得到吗，有用吗……
        if(Num % 500 == 0) {
            fprintf(yyout, "\tb .B%d\n", spilt_label);
            fprintf(yyout, ".LTORG\n");
            parent->getParent()->PrintGlobal();
            fprintf(yyout, ".B%d:\n", spilt_label++);
        }              
    }

}

//modify bd finish 在前
void MachineBlock::insertBefore(MachineInstruction* dst, MachineInstruction* src)
{
    std::vector<MachineInstruction*>::iterator pos = find(inst_list.begin(), inst_list.end(), dst);
    inst_list.insert(pos, src);
}
//modify bd finish 之后
void MachineBlock::insertAfter(MachineInstruction* dst, MachineInstruction* src)
{
    std::vector<MachineInstruction*>::iterator pos = 1 + find(inst_list.begin(), inst_list.end(), dst);
    inst_list.insert(pos, src);    
}


//modify
void MachineFunction::output()
{
    const char *func_name = this->sym_ptr->toStr().c_str() + 1;
    fprintf(yyout, "\t.global %s\n", func_name);
    fprintf(yyout, "\t.type %s , %%function\n", func_name);
    fprintf(yyout, "%s:\n", func_name);
    // TODO
    /* Hint:
    *  1. Save fp
    *  2. fp = sp
    *  3. Save callee saved register
    *  4. Allocate stack space for local variable */

    MachineOperand *fp = new MachineOperand(MachineOperand::REG, 11);
    MachineOperand *sp = new MachineOperand(MachineOperand::REG, 13);
    MachineOperand *lr = new MachineOperand(MachineOperand::REG, 14);
    std::vector<MachineOperand*> regs;
    for (std::set<int>::iterator it = saved_regs.begin(); it != saved_regs.end(); it++) 
    {
        MachineOperand * reg = new MachineOperand(MachineOperand::REG, *it);
        regs.push_back(reg);
    }

    (new StackMInstruction(nullptr, StackMInstruction::PUSH, regs)) ->output();
    (new StackMInstruction(nullptr, StackMInstruction::PUSH, lr)) ->output();
    (new StackMInstruction(nullptr, StackMInstruction::PUSH, fp)) ->output();
    
    (new MovMInstruction(nullptr, MovMInstruction::MOV, fp, sp))->output();
    // Traverse all the block in block_list to print assembly code.
    (new BinaryMInstruction(nullptr, BinaryMInstruction::SUB, sp, sp, new MachineOperand(MachineOperand::IMM, AllocSpace(0))))->output();
    
    for(auto iter : block_list)
        iter->output();

    // output label .LEND
    fprintf(yyout, ".L%s_END:\n", this->sym_ptr->toStr().erase(0,1).c_str());
    //2. Restore callee saved registers and sp, fp
    sp = new MachineOperand(MachineOperand::REG, 13);
    MachineOperand *size =new MachineOperand(MachineOperand::IMM, this->AllocSpace(0));
    (new BinaryMInstruction(nullptr, BinaryMInstruction::ADD,sp, sp, size))->output();
    
    
    //恢复saved registers和fp
    // for (std::set<int>::iterator it = saved_regs.begin(); it != saved_regs.end(); it++) 
    // {
    //     MachineOperand * reg = new MachineOperand(MachineOperand::REG, *it);
    //     regs.push_back(reg);
    // }
    (new StackMInstruction(nullptr, StackMInstruction::POP, fp)) ->output();
    (new StackMInstruction(nullptr, StackMInstruction::POP, lr)) ->output();
    (new StackMInstruction(nullptr, StackMInstruction::POP, regs)) ->output();

    // 3. Generate bx instruction
    lr = new MachineOperand(MachineOperand::REG, 14);
    (new BranchMInstruction(nullptr, BranchMInstruction::BX, lr))->output();
    
    fprintf(yyout, "\n");
}


//modify bd finish了吧
void MachineUnit::PrintGlobalDecl()
{
    // TODO:
    // You need to print global variable/const declarition code;
    if(GlobalDecl_list.size() != 0) fprintf(yyout, "\t.data\n");

    for (auto global: GlobalDecl_list)
    {
        fprintf(yyout, "\t.global %s\n", global->getName().c_str());
        fprintf(yyout, "\t.align 4\n");
        fprintf(yyout,"\t.size %s, %d\n", global->getName().c_str(), global->getType()->getSize());
        fprintf(yyout, "%s:\n", global->getName().c_str());
        fprintf(yyout, "\t.word %d\n", global->value);      
    }

}
//modify bd finish了吧
void MachineUnit::PrintGlobal()
{
    for (auto global: GlobalDecl_list)
    {
        //quest 全局变量咋输出的？？？？？       有没有number啊
        fprintf(yyout, "addr_%s_%d:\n", global->getName().c_str(),no);
        fprintf(yyout, "\t.word %s\n", global->getName().c_str());  
    }
    no++;
}

//modify bd finish了吧
void MachineUnit::output()
{
    // TODO
    /* Hint:
    * 1. You need to print global variable/const declarition code;
    * 2. Traverse all the function in func_list to print assembly code;
    * 3. Don't forget print bridge label at the end of assembly code!! */
    fprintf(yyout, "\t.arch armv8-a\n");
    fprintf(yyout, "\t.arch_extension crc\n");
    fprintf(yyout, "\t.arm\n");
    //print global variable/const declarition code
    PrintGlobalDecl();
    //Traverse all the function in func_list to print assembly code
    for(auto iter : func_list)
        iter->output();
    //print bridge label at the end of assembly code
    PrintGlobal();
}
