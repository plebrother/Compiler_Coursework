#include "ast_for_loop.hpp"

namespace ast {

    void ForStatement::EmitRISC(std::ostream& stream, Context& context) const {

        int old_loop_start = context.getCurrentLoopStart();
        int old_loop_end = context.getCurrentLoopEnd();
        int old_loop_continue = context.getCurrentLoopContinue();


        int loop_start = context.getNextLabel();
        int loop_end = context.getNextLabel();
        int loop_continue = context.getNextLabel();


        context.setCurrentLoopStart(loop_start);
        context.setCurrentLoopEnd(loop_end);
        context.setCurrentLoopContinue(loop_continue);


        context.enterForLoop();


        if (init_ != nullptr) {
            init_->EmitRISC(stream, context);
        }


        //stream << "    # For loop begins" << std::endl;
        stream << "for_start_" << loop_start << ":" << std::endl;


        if (condition_ != nullptr) {
            //stream << "    # Loop condition" << std::endl;
            condition_->EmitRISC(stream, context);
            stream << "    beqz a0, for_end_" << loop_end << std::endl;
        }


        //stream << "    # Loop body" << std::endl;
        if (body_ != nullptr) {
            body_->EmitRISC(stream, context);
        }


        stream << "for_continue_" << loop_continue << ":" << std::endl;


        //stream << "    # Loop increment" << std::endl;
        if (increment_ != nullptr) {
            increment_->EmitRISC(stream, context);
        }


        stream << "    j for_start_" << loop_start << std::endl;


        stream << "for_end_" << loop_end << ":" << std::endl;
        //stream << "    # For loop ends" << std::endl;


        context.setCurrentLoopStart(old_loop_start);
        context.setCurrentLoopEnd(old_loop_end);
        context.setCurrentLoopContinue(old_loop_continue);


        context.exitLoop();
    }

void ForStatement::Print(std::ostream& stream) const {
    stream << "for (";

    // Print initialization
    if (init_ != nullptr) {
        init_->Print(stream);
    } else {
        stream << " ";
    }
    stream << "; ";

    // Print condition
    if (condition_ != nullptr) {
        condition_->Print(stream);
    }
    stream << "; ";

    // Print increment
    if (increment_ != nullptr) {
        increment_->Print(stream);
    }
    stream << ") {" << std::endl;

    // Print body
    if (body_ != nullptr) {
        body_->Print(stream);
    }
    stream << "}" << std::endl;
}

} // namespace ast
