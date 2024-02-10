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
            map<string, Ref<Object>> _M_modules;
            map<Variant::Type, Ref<Object>> _M_type_modules;

        public:
            void registerModule(const string &name, const Ref<Object> &instance)
            {
                _M_modules[name] = instance;
            }
            void registerModule(const string &name, const Variant::Type& type,  const Ref<Object> &instance)
            {
                registerModule(name, instance);
                _M_type_modules[type] = instance;
            }
            bool has(const string& name)
            {
                return _M_modules.find(name) != _M_modules.end();
            }
            bool has(const Variant::Type& type)
            {
                return _M_type_modules.find(type) != _M_type_modules.end();
            }
            Ref<Object> get(const string& name)
            {
                return _M_modules[name];
            }
            Ref<Object> get(const Variant::Type& type)
            {
                return _M_type_modules[type];
            }
        };
        class ModuleRegister
        {
        public:
            ModuleRegister(const string &name, const Ref<Object> &instance)
            {
                Singleton<ModuleDB>::instance()->registerModule(name, instance);
            }
        };
        class TypeModuleRegister
        {
        public:
            TypeModuleRegister(const string &name, const Variant::Type& type , const Ref<Object> &instance)
            {
                Singleton<ModuleDB>::instance()->registerModule(name, type, instance);
            }
        };
    } // namespace module
} // namespace phi
