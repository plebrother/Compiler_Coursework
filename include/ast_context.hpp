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

        int current_loop_start_ = -1;  // Label for the start of current loop
        int current_loop_end_ = -1;    // Label for the end of current loop

        enum class LoopType {
            None,
            While,
            DoWhile,
            For
        };

        LoopType current_loop_type_ = LoopType::None;
        int current_loop_continue_ = -1;

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

        int getCurrentLoopStart() const {
            return current_loop_start_;
        }

        int getCurrentLoopEnd() const {
            return current_loop_end_;
        }

        void setCurrentLoopStart(int label) {
            current_loop_start_ = label;
        }

        void setCurrentLoopEnd(int label) {
            current_loop_end_ = label;
        }

        void enterWhileLoop() {
            current_loop_type_ = LoopType::While;
        }

        void exitLoop() {
            current_loop_type_ = LoopType::None;
            current_loop_start_ = -1;
            current_loop_end_ = -1;
            current_loop_continue_ = -1;
        }

        bool isInWhileLoop() const {
            return current_loop_type_ == LoopType::While;
        }

        void enterForLoop() {
            current_loop_type_ = LoopType::For;
        }

        bool isInForLoop() const {
            return current_loop_type_ == LoopType::For;
        }

        int getCurrentLoopContinue() const {
            return current_loop_continue_;
        }

        void setCurrentLoopContinue(int label) {
            current_loop_continue_ = label;
        }

        bool isInDoWhileLoop() const {
            return current_loop_type_ == LoopType::DoWhile;
        }

        void enterDoWhileLoop() {
            current_loop_type_ = LoopType::DoWhile;
        }
};

} // namespace ast
