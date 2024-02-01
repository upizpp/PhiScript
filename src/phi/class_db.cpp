#include "class_db.hpp"
#include <phi/object.hpp>

namespace phi
{
    map<string, ClassInfo> ClassDB::_M_classes;
    string ClassDB::_M_bound;
    string ClassDB::_M_calling;

    ClassDB::type ClassDB::call(Object* obj, const string &method_name, arg_list args)
    {
        string class_name = obj->getClass();
        _M_calling = class_name + "::" + method_name;
        return _M_classes[class_name].call(obj, method_name, args);
    }
    void ClassDB::set(Object *obj, const string &property_name, const type &value)
    {
        _M_classes[obj->getClass()].properties[property_name].second(obj, value);
    }
    ClassDB::type ClassDB::get(Object *obj, const string &property_name)
    {
        return _M_classes[obj->getClass()].properties[property_name].first(obj);
    }
    Variant ClassInfo::call(Object* obj, const string& method_name, arg_list& args)
    {
        return getMethod(method_name)(obj, args);
    }
    ClassInfo::method& ClassInfo::getMethod(const string& name)
    {
        return methods.find(name) != methods.end() ? methods[name] : parent->getMethod(name);
    }
} // namespace phi
