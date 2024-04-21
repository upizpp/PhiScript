#include "compiler.hpp"
#include <phi/function.hpp>

namespace phi {
    Borrower<CompileOption> Compiler::globalOption;

    Function Compiler::load() {
        Lexer lexer(_M_scanner.data());
        Preprocessor preprocessor(&lexer);
        Parser parser;
        parser.parse(&preprocessor);
    }
    Function Compiler::load(Ref<ast::Node> tree, const CompileOption &option) {
        Ref<State> state = gen(tree);
        optimize(*state, option);
        return Function{Method{state}};
    }
    void Compiler::optimize(State &state, const CompileOption &option) {
        switch (option.optimizeLevel) {
        case CompileOption::OptimizeLevel::NONE:
            break;
        case CompileOption::OptimizeLevel::SIMPLE:
            Optimizer::optimizeSimply(state);
            break;
        case CompileOption::OptimizeLevel::COMPLEX:
            break;
        }
    }
    Ref<State> Compiler::gen(Ref<ast::Node> ast) {
        Ref<Generator> generator = new Generator;
        return generator->gen(ast);
    }
} // namespace phi
