#include "import.hpp"
#include <phi/runtime/builtin/modules/module_db.hpp>

namespace phi
{
    Ref<Variant> import(const string &name)
    {
        using namespace modules;
        if (Singleton<ModuleDB>::instance()->has(name))
            return new Variant{Singleton<ModuleDB>::instance()->get(name).borrow()};
        return Variant::Null;
    }
} // namespace phi
