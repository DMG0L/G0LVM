//Минимальная реализация простой виртуальной машины

#include <iostream>
#include <string.h>
#include <fstream>
enum instructions {
    MOV = 0x01,  //Перенос данных в регистр [Синтаксис: MOV, {Номер регистра}, {Значение}]
    ADD = 0x02,  //Сложение [Синтаксис: ADD, {Номер регистра}, {Значение}]
    PUT = 0x03,  //Вывести значение регистра [Синтаксис: PUT, {Номер регистра}]
    HLT = 0x04,  //Остановка ВМ [Синтаксис: HLT]
    SUB = 0x05,  //Вычитание [Синтаксис: SUB, {Номер регистра}, {Значение}]
};
struct vm {
    int memory[64];
    int regs[4];
    int PC;
    bool runing = false;
};
bool startVM(vm* VM) {
    for (int i = 0; i < 64; i++) { VM->memory[i] = 0; }
    for (int i = 0; i < 4; i++) { VM->regs[i] = 0; }
    VM->PC = 0;
    VM->runing = true;
    //for (int i = 0; i < 4; i++) { std::cout << VM->regs[i] << std::endl; }
    return 0;

}
void vmStep(vm* VM) {
    if (!VM->runing) return;
    int opcode = VM->memory[VM->PC];
    VM->PC++;
    switch (opcode) {
    case MOV: {
        int reg = VM->memory[VM->PC++];
        int val = VM->memory[VM->PC++];
        VM->regs[reg] = val;
        //for (int i = 0; i < 4; i++) { std::cout << VM->regs[i] << std::endl; }
        break;
    }
    case PUT: {
        int reg = VM->memory[VM->PC++];
        std::cout << VM->regs[reg] << std::endl;
        break;
    }
    case ADD: {
        int dest = VM->memory[VM->PC++];
        int src = VM->memory[VM->PC++];
        VM->regs[dest] += src;
        break;
    }
    case SUB: {
        int dest = VM->memory[VM->PC++];
        int src = VM->memory[VM->PC++];
        VM->regs[dest] -= src;
        break;
    }
    case HLT:
        VM->runing = false;
        return;
        
    }

}
void vmRun(vm* VM) {
    while (VM->runing) {
        vmStep(VM);
    }
}
int pr[64];
int loadProgram(std::string fileName) {
    std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
    ifs.read((char*)pr, sizeof(pr));
    return 0;
}
//int pr[64];
/* int pr[64] = {
    MOV, 0, 10,
    MOV, 1, 15,
    ADD, 0, 5,
    SUB, 0, 10,
    PUT, 0,
    PUT, 1,
    HLT
}; */
int main()
{
    vm x;
    //std::ofstream ofs("program.gpg", std::ios::binary | std::ios::out);
    //ofs.write((char*)pr, sizeof(pr));
    loadProgram("program.gpg");
    startVM(&x);
    for (int i = 0; i < std::size(pr); i++) {
        x.memory[i] = pr[i];
    }
    vmRun(&x);

}

