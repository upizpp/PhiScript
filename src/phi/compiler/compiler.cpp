#include "compiler.hpp"
#include <phi/function.hpp>

namespace phi
{
    Borrower<CompileOption> Compiler::globalOption;

    Ref<State> Compiler::compile()
    {
        Preprocessor preprocessor(tokenize());
        token::tokens &tokens = preprocessor.getTokens();
        Ref<State> state = gen(parse(tokens));
        optimize(*state, _M_option);
        return state;
    }
    Function Compiler::load()
    {
        return Function{Method{compile()}};
    }
    Function Compiler::load(Ref<ast::Node> tree, const CompileOption& option)
    {
        Ref<State> state = gen(tree);
        optimize(*state, option);
        return Function{Method{state}};
    }
    void Compiler::optimize(State& state, const CompileOption& option)
    {
        switch (option.optimizeLevel)
        {
        case CompileOption::OptimizeLevel::NONE:
            break;
        case CompileOption::OptimizeLevel::SIMPLE:
            Optimizer::optimizeSimply(state);
            break;
        case CompileOption::OptimizeLevel::COMPLEX:
            break;
        }
    }
    token::tokens Compiler::tokenize()
    {
        Lexer lexer(_M_scanner.data());
        return lexer.getTokens();
    }
    token::tokens Compiler::preprocess(const token::tokens &tokens, const CompileOption& option)
    {
        Preprocessor preprocessor(tokens);
        if (option.preprocessRule)
            preprocessor.setRule(*option.preprocessRule);
        return preprocessor.getTokens();
    }
    Ref<ast::Node> Compiler::parse(token::tokens &tokens)
    {
        Parser parser;
        return parser.parse(tokens);
    }
    Ref<ast::Node> Compiler::parse()
    {
        Preprocessor preprocessor(tokenize());
        token::tokens &tokens = preprocessor.getTokens();
        return parse(tokens);
    }
    Ref<State> Compiler::gen(Ref<ast::Node> ast)
    {
        Ref<Generator> generator = new Generator;
        return generator->gen(ast);
    }
} // namespace phi
