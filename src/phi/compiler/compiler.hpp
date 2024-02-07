#pragma once
#include <phi/compiler/generator.hpp>
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/optimizer.hpp>
#include <phi/compiler/parser.hpp>
#include <phi/compiler/preprocessor.hpp>

namespace phi
{
    struct CompileOption
    {
        enum OptimizeLevel
        {
            NONE,
            SIMPLE,
            COMPLEX // to be implemented
        };

        OptimizeLevel optimizeLevel = OptimizeLevel::NONE;
        Owner<PreprocessRule> preprocessRule;

        CompileOption() = default;
        CompileOption(const CompileOption &option) : 
                optimizeLevel(option.optimizeLevel), preprocessRule(new PreprocessRule{*option.preprocessRule}) {}
    };

    class Compiler
    {
    private:
        CompileOption _M_option;
        Owner<Scanner> _M_scanner;

    public:
        static Borrower<CompileOption> globalOption;

        Compiler(Scanner *scanner) : _M_scanner(scanner), _M_option(globalOption ? *globalOption : CompileOption{}) {}
        Compiler(Scanner *scanner, CompileOption &&option) : _M_scanner(scanner), _M_option(std::move(option)) {}

        Ref<State> compile();
        Function load();
        token::tokens tokenize();
        Ref<ast::Node> parse();

        static void optimize(State &, const CompileOption &);
        static Ref<ast::Node> parse(token::tokens &);
        static Function load(Ref<ast::Node>, const CompileOption &option);
        static token::tokens preprocess(const token::tokens &, const CompileOption &);
        static Ref<State> gen(Ref<ast::Node>);
    };
} // namespace phi
