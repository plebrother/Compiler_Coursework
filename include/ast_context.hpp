#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

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
        int labelCounter = 0;

        std::unordered_map<std::string, int> variable_offsets_;
        int current_stack_offset_ = -4;

        bool in_global_scope_ = true;

    public:
        void useReg(int i){
            usedReg[i] = 1;
        }
        void freeReg(int i){
            usedReg[i] = 0;
        }

        int allocate(){
            for (int i = 0; i < 11; i++){
                if (usedReg[i] == 0){
                    useReg(i);
                    return i;
                }
            }
            std::cerr << "no spare register"<< std::endl;
            exit(1);
        }

        std::string getRegName(int regIndex) const {
            return "s" + std::to_string(regIndex);
        }

        int getNextLabel() {
            return labelCounter++;
        }

        void resetLabelCounter() {
            labelCounter = 0;
        }

        int addLocalVariable(const std::string& name) {
            // Check if variable already exists
            if (variable_offsets_.find(name) != variable_offsets_.end()) {
                std::cerr << "Error: Variable '" << name << "' already declared" << std::endl;
                exit(1);
            }

            // Allocate space on stack and track the offset
            int offset = current_stack_offset_;
            variable_offsets_[name] = offset;
            current_stack_offset_ -= 4;  // Move to next position (4 bytes for int)
            return offset;
        }

        int getVariableOffset(const std::string& name) const {
            auto it = variable_offsets_.find(name);
            if (it == variable_offsets_.end()) {
                std::cerr << "Error: Undefined variable '" << name << "'" << std::endl;
                exit(1);
            }
            return it->second;
        }

        bool hasVariable(const std::string& name) const {
            return variable_offsets_.find(name) != variable_offsets_.end();
        }

        void resetVariables() {
            variable_offsets_.clear();
            current_stack_offset_ = -4;
        }

        int getLocalVariablesSize() const {
            return -current_stack_offset_ - 4;  // Convert to positive size
        }

        bool isGlobalScope() const {
            return in_global_scope_;
        }

        void enterFunctionScope() {
            in_global_scope_ = false;
            resetVariables();
        }

        void exitFunctionScope() {
            in_global_scope_ = true;
            resetVariables();
        }
};

} // namespace ast
