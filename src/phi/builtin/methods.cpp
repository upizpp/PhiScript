#include "methods.hpp"
#include <iostream>
#include <phi/builtin/modules/modules>
#include <phi/phi_std.hpp>

static std::map<std::string, phi::Module *> modules = {
    {"io", &phi::builtin::io_lib},
    {"variant", &phi::builtin::variant_lib},
    {"array", &phi::builtin::array_lib},
    {"string", &phi::builtin::string_lib},
    {"math", &phi::builtin::math_lib},
    {"os", &phi::builtin::os_lib},
};

namespace phi
{
    namespace builtin
    {
        void import(Ref<runtime::Block> block, const string &module_name, const string &import_name)
        {
            if (modules.find(module_name) != modules.end())
            {
                Ref<Variant> module{new Variant{modules[module_name]}};
                module->setFlag(FLAG_MODULE);
                block->setLocal(import_name, module);
            }
            // TODO import
        }
    } // namespace builtin
} // namespace phi
