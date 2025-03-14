#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

enum Reg {
    s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11, //x18-27: maintained across calls
};
namespace ast {
// An object of class Context is passed between ast nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).
class Context
{
    /* TODO decide what goes inside here */
    private:
        bool usedReg[11] = {0,0,0,0,0,0,0,0,0,0,0};

    public:
        void useReg(int i){
            usedReg[i] = 1;
        }
        void freeReg(int i){
            usedReg[i] = 0;
        }

        int allocate(){
            for (int i = 0; i < 12; i ++){
                if (usedReg[i] == 0){
                    useReg(i);
                    return i;
                }
            }
            std::cerr << "no spare register"<< std::endl;
        }
};

} // namespace ast
