#include "phi_args.hpp"
#include <phi/utility/branch_parser.hpp>
#include <phi/runtime/block.hpp>
#include <phi/owner.hpp>
#include <phi/compiler/compiler.hpp>
#include <iostream>

namespace phi
{
    void parseArgs(int &argc, char **&args)
    {
        ArgsParser parser;

        parser.parse(argc, args);
        auto files = parser.getArguments();
        Ref<runtime::Block> block(new runtime::Block);
        for (const std::string &file : files)
        {
            compiler::Compiler::compileFile(file, block).run();
        }
    }
} // namespace phi
