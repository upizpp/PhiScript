#include "import.hpp"
#include <phi/runtime/builtin/modules/filesystem.hpp>
#include <phi/runtime/builtin/modules/module_db.hpp>

namespace phi {
    Ref<Variant> import(const string &name) {
        using namespace modules;
        if (Singleton<ModuleDB>::instance()->has(name))
            return new Variant{
                Singleton<ModuleDB>::instance()->get(name).borrow()};
        if (FileSystemLib::instance->is_file(name)) {
        }
        throw RuntimeException("No such a module named \"%s\".", name.c_str());
    }
} // namespace phi
