#include "compiler.hpp"

namespace phi
{
    Ref<State> Compiler::compile()
    {
        Preprocessor preprocessor(tokenize());
        token::tokens& tokens = preprocessor.getTokens();
        Ref<State> state = gen(parse(tokens));
        switch (_M_option.optimizeLevel)
        {
        case CompileOption::OptimizeLevel::NONE:
            break;
        case CompileOption::OptimizeLevel::SIMPLE:
            Optimizer::optimizeSimply(*state);
            break;
        case CompileOption::OptimizeLevel::COMPLEX:
            break;
        }
        return state;
    }
    token::tokens Compiler::tokenize()
    {
        Lexer lexer(_M_scanner.data());
        return lexer.getTokens();
    }
    token::tokens Compiler::preprocess(const token::tokens &tokens)
    {
        Preprocessor preprocessor(tokens);
        if (_M_option.preprocessRule)
            preprocessor.setRule(*_M_option.preprocessRule);
        return preprocessor.getTokens();
    }
    Ref<ast::Node> Compiler::parse(token::tokens &tokens)
    {
        Parser parser;
        return parser.parse(tokens);
    }
    Ref<State> Compiler::gen(Ref<ast::Node> ast)
    {
        Ref<Generator> generator = new Generator;
        return generator->gen(ast);
    }
} // namespace phi
