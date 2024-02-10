#pragma once
#include <phi/class_db.hpp>
#include <phi/object.hpp>
#include <phi/variant.hpp>

namespace phi
{
    namespace modules
    {

        class ModuleDB
        {
        public:
            using getter_t = std::function<Ref<Object>()>;
            map<string, getter_t> _M_modules;

        public:
            void registerModule(const string &name, const getter_t &getter)
            {
                _M_modules[name] = getter;
            }
            bool has(const string& name)
            {
                return _M_modules.find(name) != _M_modules.end();
            }
            Ref<Object> get(const string& name)
            {
                return _M_modules[name]();
            }
        };
        class ModuleRegister
        {
        public:
            ModuleRegister(const string &name, const ModuleDB::getter_t &getter)
            {
                Singleton<ModuleDB>::instance()->registerModule(name, getter);
            }
        };
    } // namespace module
} // namespace phi
