#pragma once
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

enum Reg {
    s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11, //x18-27: maintained across calls
};

enum Para {
    a0,a1,a2,a3,a4,a5,a6,a7, //pass parameters
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
        bool usedPara[8] = {0,0,0,0,0,0,0,0};
        int labelCounter = 0;

        std::unordered_map<std::string, int> variable_offsets_;
        int current_stack_offset_ = -16;

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

        std::vector<std::unordered_map<std::string, int>> scopes_;
        std::vector<std::array<bool, 11>> scopes_reg_;

        int current_function_end_label_ = -1;

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
                    return i + 1;
                }
            }
            std::cerr << "no spare register"<< std::endl;
            exit(1);
        }

        bool Regused(int i){
            return (usedReg[i] == 1);
        }

        int allocatePara(){
            for (int i = 0; i < 8; i++){
                if (usedPara[i] == 0){
                    usedPara[i] = 1;
                    return i;
                }
            }
            std::cerr << "no spare parameter register to use" << std::endl;
            exit(1);
        }

        void freePara(){
            for (int i = 0; i < 8; i++){
                usedPara[i] = 0;
            }
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


        void enterScope() {
            std::cerr << "enterScope" << std::endl;
            scopes_.push_back(variable_offsets_);

            std::array<bool, 11> current_regs;
            for (int i = 0; i < 11; i++) {
                current_regs[i] = usedReg[i];
            }
            scopes_reg_.push_back(current_regs);
            variable_offsets_.clear();
            for (int i = 0; i < 11; i++){
                usedReg[i] = 0;
            }
        }

        void exitScope() {
            std::cerr << "exitScope" << std::endl;
            variable_offsets_ = scopes_.back();
            std::array<bool, 11> prev_reg = scopes_reg_.back();
            for (int i = 0; i < 11; i++){
                usedReg[i] = prev_reg[i];
            }
            scopes_.pop_back();
            scopes_reg_.pop_back();
        }




        int addLocalVariable(const std::string& name) {

            std::cout << current_stack_offset_ << std::endl;
            int offset = current_stack_offset_;
            variable_offsets_[name] = offset;
            current_stack_offset_ -= 4;


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

        void set_variable_stack_offset(int i){
            std::cerr << "set variable stack offset to:" << i << std::endl;
            current_stack_offset_ = i;
        }

        void resetVariables() {
            variable_offsets_.clear();
            current_stack_offset_ = -16;
        }

        int getLocalVariablesSize() const {
            return -current_stack_offset_ - 4;  // Convert to positive size
        }

        int get_current_stack_offset(){
            return current_stack_offset_;
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

        void setCurrentFunctionEndLabel(int label) {
            current_function_end_label_ = label;
        }

        int getCurrentFunctionEndLabel() const {
            return current_function_end_label_;
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
