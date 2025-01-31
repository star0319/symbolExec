//
// Created by star on 2024/3/15.
//

#include <fstream>
#include <iostream>
#include "func.h"
#include "limits"
#include "libcfg_utils.h"
#include "MyStack.h"
#include "MyRegister.h"
#include "toolUtil.h"
#include "elfio_dump.hpp"
#include "elfio.hpp"
#include "elfio_section.hpp"


std::string FILE_PATH= "../../symex-x86";
//std::string FILE_PATH= "../../symGlobalDemo";
//std::string FILE_PATH= "../../symmainStack-cin";
//std::string FILE_PATH= "../../symmainStack-args";
bool is_print_Register= false,is_print_Stack= false,is_print_RFlags= false,is_print_insn= true;
//bool is_print_Register= true,is_print_Stack= true,is_print_RFlags= false,is_print_insn= true;

int exectest(const ELFIO:: elfio& reader) {
//    std::cout << "libCFG" << std::endl;
//
//    std::ifstream file(FILE_PATH, std::ios::binary); // 以二进制模式打开文件
//
//    if (!file.is_open()) { // 检查文件是否成功打开
//        std::cerr << "Unable to open file" << std::endl;
//        return 1;
//    }

//    uint64_t textEntry=reader->get_entry();
//    file.seekg(0x1060, std::ios::beg);
//    file.seekg(textEntry, std::ios::beg);


    uint16_t numOfSections=reader.sections.size();
    uint64_t textSize;
    char* buffer;
    std::string func_name="main";
    uint64_t low_pc ;  //0x1060
    uint64_t high_pc;  //0x295
    uint64_t main_adress;//0x121a
    uint64_t main_return;  //0x1278

    for (uint16_t i = 0; i < numOfSections; ++i) {
        const ELFIO:: section* sec = reader.sections[i];
        //find text
        if (sec->get_name()==".text"){
            buffer=const_cast<char *>(sec->get_data());
            low_pc=sec->get_address();
            high_pc=sec->get_size();
        }
        //find symbol  -->  find main
        else if (sec->get_name()==".symtab"){
            ELFIO:: const_symbol_section_accessor symbols( reader, sec );
            uint64_t sym_no = symbols.get_symbols_num();
            for (  uint64_t i = 0; i < sym_no; ++i ) {
                std::string   name;
                uint64_t    value   = 0;
                uint64_t     size    = 0;
                unsigned char bind    = 0;
                unsigned char type    = 0;
                uint16_t      section = 0;
                unsigned char other   = 0;
                symbols.get_symbol( i, name, value, size, bind, type,
                                    section, other );
                if (name=="main"){
                    main_adress=value;
                    main_return=value+size-1;
//                    cout<<std::hex <<static_cast<int>( main_return)<<std::dec<<endl;
                    break;
                }

            }
        }


    }

//    // 分配内存，读取文件内容
//    char* buffer = new char[0x295];
//    file.read(buffer, 0x295); // 读取字节到缓冲区
//
//
//    // 关闭文件
//    file.close();




    libCFG::func f(func_name,low_pc,high_pc);


    f.insn_buffer=buffer;
    f.parse();
//    f.constructCFG(f.num_of_insn);

    //将指令adress与指令索引建立映射。
    std::map <uint64_t, int> index_insnAddressMap;
    for (int i = 0; i < f.num_of_insn; ++i) {
        cs_insn insn=f.instructions[i];
//        printInsn(is_print_insn,insn);
        index_insnAddressMap[insn.address]=i;
    }





    //find main_index
    int insn_index=index_insnAddressMap[main_adress];

    cs_insn  * instructions=f.instructions;
    MyRegister* execRegister=MyRegister::GetRegister();
    execRegister->set_f(&f);
    MyStack execStack= MyStack(1000);


//    uint64_t exec_curr_rbp=execStack.getRbp();
//    uint64_t exec_curr_rsp=execStack.getRsp();
//    execRegister->update(X86_REG_RBP,exec_curr_rbp);
//    execRegister->update(X86_REG_RSP,exec_curr_rsp);
//    execRegister->printRegister();

    bool exit= false;

    while (!exit){
        cs_insn insn=instructions[insn_index];

        switch (insn.id) {
            /* JUMP Instructments */
            case X86_INS_JMP  :
            case X86_INS_JA   :
            case X86_INS_JB   :
            case X86_INS_JG   :
            case X86_INS_JAE  :
            case X86_INS_JBE  :
            case X86_INS_JGE  :
            {
                printPassedInsn(is_print_insn,insn);


                break;
            }
            case X86_INS_JNE  : {
                printInsn(is_print_insn,insn);
//                std::cout<<insn.address<<endl;

                //根据ZF标志判断是否要跳转
                if (!execRegister->get_RFlags(ZF)){
                    //要跳转到的地址
                    uint64_t target_address;
                    cs_x86  x86=insn.detail->x86;
                    cs_x86_op ops=x86.operands[0];
                    if (ops.type==X86_OP_IMM){
                        target_address=ops.imm;
                    }
                    insn_index=index_insnAddressMap[ target_address ];
                } else{
                    insn_index++;
                }
                execStack.printStack(is_print_Stack);
                execRegister->printRegister(is_print_Register);



                break;
            }
            case X86_INS_JE   :
            {

                printInsn(is_print_insn,insn);
//                std::cout<<insn.address<<endl;

                //根据ZF标志判断是否要跳转
                if (execRegister->get_RFlags(ZF)){
                    //要跳转到的地址
                    uint64_t target_address;
                    cs_x86  x86=insn.detail->x86;
                    cs_x86_op ops=x86.operands[0];
                    if (ops.type==X86_OP_IMM){
                        target_address=ops.imm;
                    }
                    insn_index=index_insnAddressMap[ target_address ];
                } else{
                    insn_index++;
                }
                execStack.printStack(is_print_Stack);
                execRegister->printRegister(is_print_Register);

                break;
            }

                /* RETURN Instructments */
            case X86_INS_RET  : {
                //执行完退出循环
                if (insn.address==main_return) {
                    exit= true;
                    break;
                }

                printInsn(is_print_insn,insn);
                //跳转到调用函数的返回地址
                uint64_t target_address=execStack.pop64();

                insn_index=index_insnAddressMap[ target_address ];


                execStack.printStack(is_print_Stack);
                execRegister->printRegister(is_print_Register);


                break;
            }
                /* CALL Instructments */
            case X86_INS_CALL : {
                printInsn(is_print_insn,insn);
//                std::cout<<insn.address<<endl;
                //记录call指令的下一条指令地址push进stack  以便在ret时找到
                uint64_t next_address=instructions[insn_index+1].address;
                execStack.push64(next_address);


                //跳转到call的函数地址
                uint64_t target_address;
                cs_x86  x86=insn.detail->x86;
                cs_x86_op ops=x86.operands[0];
                if (ops.type==X86_OP_IMM){
                    target_address=ops.imm;
                }
                insn_index=index_insnAddressMap[ target_address ];


                execStack.printStack(is_print_Stack);
                execRegister->printRegister(is_print_Register);

                break;
            }

            // mov rsp,rbp;pop rbp
            case X86_INS_LEAVE:{
                printInsn(is_print_insn,insn);

                // mov rsp,rbp;
                uint64_t rbp_data =execRegister->registerMap[X86_REG_RBP];
                execRegister->update(X86_REG_RSP,rbp_data);

                // pop rbp
                execRegister->update(  X86_REG_RBP, execStack.pop64());

                execStack.printStack(is_print_Stack);
                execRegister->printRegister(is_print_Register);

                insn_index++;
                break;
            }



            case X86_INS_PUSH:{
                printInsn(is_print_insn,insn);

                cs_x86  x86=insn.detail->x86;
                cs_x86_op ops=x86.operands[0];

                if (ops.type==X86_OP_REG) {
//                    cout<<ops.reg<<endl;
                    execStack.push64(execRegister->registerMap[ops.reg]);

                    execStack.printStack(is_print_Stack);
                    execRegister->printRegister(is_print_Register);
                }

                insn_index++;
                break;

            }


        case X86_INS_POP:{
            printInsn(is_print_insn,insn);

            cs_x86  x86=insn.detail->x86;
            cs_x86_op ops=x86.operands[0];

            if (ops.type==X86_OP_REG) {
//                    cout<<ops.reg<<endl;
                execRegister->update(  ops.reg, execStack.pop64());

                execStack.printStack(is_print_Stack);
                execRegister->printRegister(is_print_Register);
            }

            insn_index++;
            break;

        }


        case X86_INS_MOV:{
                printInsn(is_print_insn,insn);
                cs_x86  x86=insn.detail->x86;
//                cout<<"op_count= "<<static_cast<int>( x86.op_count)<<endl;
                cs_x86_op ops[x86.op_count];
                for (int i = 0; i < x86.op_count; ++i) {
                    ops[i]=x86.operands[i];
//                    cout<<ops[i].type<<endl;
                }
//            cout<<"op0= "<<static_cast<int>(ops[1].size)<<endl;
//            cout<<"op1= "<<static_cast<int>(ops[1].size)<<endl;

                //第一位为寄存器
                if (  ops[0].type==X86_OP_REG) {
                    uint64_t data;

                    if (ops[1].type==X86_OP_REG){
                        data=execRegister->registerMap[ops[1].reg];
                    } else if(ops[1].type==X86_OP_IMM){
                        data= ops[1].imm;
                    }
                    else if (ops[1].type==X86_OP_MEM){
//                        execRegister->printRegister(true);
//                         cout<<"op1= "<<static_cast<int>(ops[1].mem.segment)<<endl;
                        uint64_t address=execRegister->registerMap[ops[1].mem.base]+ops[1].mem.disp;

                        if (address<execStack.getRsp())   std::cout<<"  data: "<< std::hex << std::setw(16) << std::setfill('0')<<static_cast<int>(address)<< std::dec<<std::endl;;
                        std::cout<<"  data: "<< std::hex << std::setw(16) << std::setfill('0')<<static_cast<int>(address)<< std::dec<<std::endl;;

//                        printInsn(is_print_insn,insn);
                        data=execStack.get32(address);
//                        cout<<"asdsd2"<<endl;
//                        printInsn(is_print_insn,insn);
                    }

                    execRegister->update(ops[0].reg,data);

//                    execRegister->printRegister();
                }
                //第一位为栈
                if (  ops[0].type==X86_OP_MEM) {

                    uint64_t data;

                    if (ops[1].type==X86_OP_REG){
                        data=execRegister->registerMap[ops[1].reg];
                    } else if(ops[1].type==X86_OP_IMM){
                        data= ops[1].imm;
                    }
                    else if (ops[1].type==X86_OP_MEM){
//                        data=ops[1].mem
                    }
//                    cout<<ops[0].mem.base<<"disp  "<<ops[0].mem.disp<<"scale  "<<ops[0].mem.scale<<endl;

                    uint64_t address=execRegister->registerMap[ops[0].mem.base]+ops[0].mem.disp;
                    execStack.update32(address,data);
//                    execStack.printStack();

                }

                insn_index++;
                break;

            }
            case X86_INS_SUB:{
                printInsn(is_print_insn,insn);


                cs_x86  x86=insn.detail->x86;
//                cout<<"op_count= "<<static_cast<int>( x86.op_count)<<endl;
                cs_x86_op ops[x86.op_count];
                for (int i = 0; i < x86.op_count; ++i) {
                    ops[i]=x86.operands[i];
//                    cout<<ops[i].type<<endl;
                }

                if (  ops[0].type==X86_OP_REG) {

                    uint64_t data;

                    if (ops[1].type==X86_OP_REG){
//                        data=execRegister->registerMap[ops[1].reg];
                    } else if(ops[1].type==X86_OP_IMM){
                        data= ops[1].imm;
                    }
                    else if (ops[1].type==X86_OP_MEM){
//                        data=ops[1].mem
                    }
//                    cout<<static_cast<int>(data) <<endl;
                    execRegister->update(ops[0].reg,execRegister->registerMap[ops[0].reg]- data);


                    execStack.printStack(is_print_Stack);
                    execRegister->printRegister(is_print_Register);
                }



                insn_index++;
                break;

            }

            case X86_INS_ADD:{
                printInsn(is_print_insn,insn);


                cs_x86  x86=insn.detail->x86;
//                cout<<"op_count= "<<static_cast<int>( x86.op_count)<<endl;
                cs_x86_op ops[x86.op_count];
                for (int i = 0; i < x86.op_count; ++i) {
                    ops[i]=x86.operands[i];
//                    cout<<ops[i].type<<endl;
                }

                if (  ops[0].type==X86_OP_REG) {

                    uint64_t data;

                    if (ops[1].type==X86_OP_REG){
                        data=execRegister->registerMap[ops[1].reg];
                    } else if(ops[1].type==X86_OP_IMM){
                        data= ops[1].imm;
                    }
                    else if (ops[1].type==X86_OP_MEM){
//                        data=ops[1].mem
                    }
//                    cout<<static_cast<int>(data) <<endl;
                    execRegister->update(ops[0].reg,execRegister->registerMap[ops[0].reg]+ data);

                    execStack.printStack(is_print_Stack);
                    execRegister->printRegister(is_print_Register);
                }



                insn_index++;
                break;

            }

            case X86_INS_CMP:{
                printInsn(is_print_insn,insn);

                execStack.printStack(is_print_Stack);
                execRegister->printRegister(is_print_Register);

                cs_x86  x86=insn.detail->x86;
//                cout<<"op_count= "<<static_cast<int>( x86.op_count)<<endl;
                cs_x86_op ops[x86.op_count];
                for (int i = 0; i < x86.op_count; ++i) {
                    ops[i]=x86.operands[i];
//                    cout<<ops[i].type<<endl;
                }
                uint64_t data0;
                uint64_t data1;

                //第一位为寄存器
                if (  ops[0].type==X86_OP_REG) {
                    data0=execRegister->registerMap[ops[0].reg];
                    if (ops[1].type==X86_OP_REG){
                        data1=execRegister->registerMap[ops[1].reg];
                    } else if(ops[1].type==X86_OP_IMM){
                        data1= ops[1].imm;
                    }
                    else if (ops[1].type==X86_OP_MEM){
                        uint64_t address=execRegister->registerMap[ops[1].mem.base]+ops[1].mem.disp;
                        data1=execStack.get32(address);
                    }

                }
                //第一位为栈
                if (  ops[0].type==X86_OP_MEM) {
                    uint64_t address=execRegister->registerMap[ops[0].mem.base]+ops[0].mem.disp;
                    data0=execStack.get32(address);

                    if (ops[1].type==X86_OP_REG){
                        data1=execRegister->registerMap[ops[1].reg];
                    } else if(ops[1].type==X86_OP_IMM){
                        data1= ops[1].imm;
                    }
                    else if (ops[1].type==X86_OP_MEM){
//                        data1=ops[1].mem
                    }
//                    cout<<ops[0].mem.base<<"disp  "<<ops[0].mem.disp<<"scale  "<<ops[0].mem.scale<<endl;


                }
//                cout<<"data0  "<<static_cast<long long int>( data0)<<"data1  "<< static_cast<long long int>( data1) <<endl;
                int64_t result=(static_cast<long long int>( data0) -static_cast<long long int>( data1));
                if (result==0){
                    execRegister->set_RFlags(ZF);
                }
                if (result<0){
                    execRegister->set_RFlags(SF);
                }

                execRegister->printRflags(is_print_RFlags);

                insn_index++;
                break;

            }
            default:
                printPassedInsn(is_print_insn,insn);
                insn_index++;
                break;
        }

    }
    return 0;
}

//TODO: 区分32  与64位寄存器     使用ops[].size  区分

//TODO: 完善指令操作

//TODO： 非栈内存


int main(){
    ELFIO:: elfio reader;
//    char * file="../../symex-x86-stack";
    if ( !reader.load( FILE_PATH) ) {
        printf( "File %s is not found or it is not an ELF file\n", FILE_PATH.c_str() );
        return 1;
    }

    exectest(reader);
}


