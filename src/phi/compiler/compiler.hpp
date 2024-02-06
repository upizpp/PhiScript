#pragma
#include <phi/compiler/lexer.hpp>
#include <phi/compiler/parser.hpp>
#include <phi/compiler/preprocessor.hpp>
#include <phi/compiler/generator.hpp>
#include <phi/compiler/optimizer.hpp>

namespace phi
{
    struct CompileOption
    {
        enum OptimizeLevel{
            NONE,
            SIMPLE,
            COMPLEX // to be implemented
        };

        OptimizeLevel optimizeLevel = OptimizeLevel::NONE;
        Owner<PreprocessRule> preprocessRule;
    };
    

    class Compiler
    {
    private:
        CompileOption _M_option;
        Owner<Scanner> _M_scanner;
    public:
        Compiler(Scanner* scanner) : _M_scanner(scanner) {}
        Compiler(Scanner* scanner, CompileOption&& option) : _M_scanner(scanner), _M_option(std::move(option)) {}

        Ref<State> compile();
        token::tokens tokenize();
        token::tokens preprocess(const token::tokens&);
        Ref<ast::Node> parse(token::tokens&);
        Ref<State> gen(Ref<ast::Node>);
    };
} // namespace phi
