#include <phi/data_type.hpp>
#include "ast/node.hpp"

namespace phi
{
    namespace runtime
    {
        class Block;
    } // namespace runtime
    
    namespace compiler
    {
        class Compiler
        {
        public:
            static ast::Node compileFile(const std::string&, Ref<runtime::Block>);
            static ast::Node compile(const std::string&, Ref<runtime::Block>);
        };
    } // namespace compiler
    
} // namespace phi
