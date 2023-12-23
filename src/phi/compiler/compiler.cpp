#include "compiler.hpp"
#include <phi/compiler/analyzer.hpp>
#include <phi/compiler/code_reader.hpp>
#include <phi/compiler/parser.hpp>
#include <phi/compiler/preprocessor.hpp>
#include <phi/compiler/tokenizer.hpp>

namespace phi
{
    namespace compiler
    {
        ast::Node Compiler::compileFile(const std::string &path, Ref<runtime::Block> block)
        {
            Tokenizer tokenizer = Tokenizer(new FileReader{path});
            Token::Container tokens = tokenizer.getTokens();

            Preprocessor::handleString(&tokens);
            StaticAnalyzer::analyze(&tokens);
            return Parser::parseStmts(&tokens, block);
        }

        ast::Node phi::compiler::Compiler::compile(const std::string &code, Ref<runtime::Block> block)
        {
            Tokenizer tokenizer = Tokenizer(new StringReader{code});
            Token::Container tokens = tokenizer.getTokens();

            Preprocessor::handleString(&tokens);
            StaticAnalyzer::analyze(&tokens);
            return Parser::parseStmts(&tokens, block);
        }
    } // namespace compiler
} // namespace phi
