#include "compiler.hpp"

#include <compile/analyzer.hpp>
#include <compile/generator.hpp>
#include <compile/lexer.hpp>
#include <compile/parser.hpp>
#include <compile/preprocessor.hpp>

namespace phi {
unique_ptr<Program> compile(Scanner &&scanner) {
    Lexer lexer(scanner);
    Preprocessor preprocessor(lexer);
    Parser parser(preprocessor);
    Parser::node_t node = parser.parse();
    Analyzer analyzer(node);
    analyzer.analyze();
    Generator generator(node, parser.chunk());
    return generator.generate();
}
} // namespace phi
