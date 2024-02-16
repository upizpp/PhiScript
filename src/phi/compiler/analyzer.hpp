#pragma once
#include <phi/compiler/ast.hpp>

namespace phi {
    class Analyzer {
      public:
        static void analyze(Ref<ast::Node>);
    };
} // namespace phi
