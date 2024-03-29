#include "class_db.hpp"
#include <phi/object.hpp>

namespace phi {
    ClassDB::type ClassDB::call(Object *obj, const string &method_name,
                                const array &args) {
        string class_name = obj->getClass();
        _M_calling = class_name + "::" + method_name;
        return _M_classes[class_name].call(obj, method_name, args);
    }
    void ClassDB::set(Object *obj, const string &property_name,
                      const type &value) {
        _M_classes[obj->getClass()].properties[property_name].second(obj,
                                                                     value);
    }
    ClassDB::type ClassDB::get(Object *obj, const string &property_name) {
        return _M_classes[obj->getClass()].properties[property_name].first(obj);
    }
    bool ClassDB::hasProperty(const Object *obj, const string &property_name) {
        return _M_classes[obj->getClass()].hasProperty(property_name);
    }
    bool ClassDB::hasMethod(const Object *obj, const string &method_name) {
        return _M_classes[obj->getClass()].hasMethod(method_name);
    }
    Object *ClassDB::create(const string &class_name) {
        return _M_classes[class_name].initializer();
    }
    ClassInfo::type ClassInfo::call(Object *obj, const string &method_name,
                                    const array &args) {
        return getMethod(method_name)(obj, args);
    }
    bool ClassInfo::hasProperty(const string &name) {
        return properties.find(name) != properties.end();
    }
    bool ClassInfo::hasMethod(const string &name) {
        return methods.find(name) != methods.end();
    }
    ClassInfo::method &ClassInfo::getMethod(const string &name) {
        return methods.find(name) != methods.end() ? methods[name]
                                                   : parent->getMethod(name);
    }
} // namespace phi
