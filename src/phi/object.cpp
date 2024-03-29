#include "object.hpp"
#include <phi/class_db.hpp>
#include <phi/exception.hpp>
#include <phi/function.hpp>
#include <phi/runtime/evaluator.hpp>

namespace phi {
#undef CLASS
#define CLASS Test
    Object *Object::makeObj(const string &classname) {
        return Singleton<ClassDB>::instance()->create(classname);
    }
    void Object::static_register() {}
    void Object::set(const string &property, Ref<Variant> value) {
        if (Singleton<ClassDB>::instance()->hasProperty(this, property))
            Singleton<ClassDB>::instance()->set(this, property, *value);
        else
            _M_properties[property] = value;
    }
    Ref<Variant> Object::get(const string &property) {
        return new Variant{Singleton<ClassDB>::instance()->get(this, property)};
    }
    bool Object::hasProperty(const string &name) const {
        return _M_properties.find(name) != _M_properties.end() ||
               Singleton<ClassDB>::instance()->hasMethod(this, name) ||
               Singleton<ClassDB>::instance()->hasProperty(this, name);
    }
    Ref<Variant> Object::call(const array &args) {
        if (!hasProperty("__call__"))
            throw RuntimeException("The object is not callable.");
        return call("__call__");
    }
    Ref<Variant> Object::call(const string &name, const array &args) {
        if (!hasProperty(name))
            throw RuntimeException("Cannot find the method. (%s)",
                                   name.c_str());
        return access({new Variant{name}})->call(args);
    }
    VariantPacker Object::access(const array &args) {
        if (args.size() == 1 && args[0]->type() == Variant::Type::STRING) {
            string str = *args[0];
            string name = getClass() + "::" + str;
            if (Singleton<ClassDB>::instance()->hasProperty(this, str))
                return {new Variant{get(str)}, name};
            else if (Singleton<ClassDB>::instance()->hasMethod(this, str))
                return {Ref<Variant>{new Variant{
                            Function{Singleton<ClassDB>::instance()->toCallable(
                                this, str)}}},
                        name};
            if (_M_properties.find(str) == _M_properties.end())
                _M_properties[str] = new Variant;
            return {_M_properties[str], name};
        } else {
            throw ArgumentException(1, args.size(), __FUNCTION__);
        }
    }
    Variant Object::convert(Variant::Type type) {
        if (!hasProperty("__convert__"))
            throw RuntimeException("The object is not convertible.");
        return *call("__convert__", {new Variant{(integer)type}});
    }
    string Object::toString() {
        if (!hasProperty("__str__")) {
            std::ostringstream os;
            os << "Object: " << this;
            return os.str();
        }
        return *call("__str__");
    }
    uinteger Object::hash() {
        if (hasProperty("__hash__"))
            return call("__hash__");
        return (uinteger)this;
    }
    bool Object::operator==(const Variant &other) const {
        if (!hasProperty("__eq__"))
            throw RuntimeException("The object is not comparable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::compare)");
        return const_cast<Object *>(this)->call(
            "__eq__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    bool Object::operator!=(const Variant &other) const {
        if (!hasProperty("__ne__"))
            throw RuntimeException("The object is not comparable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::compare)");
        return const_cast<Object *>(this)->call(
            "__ne__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    bool Object::operator>(const Variant &other) const {
        if (!hasProperty("__gt__"))
            throw RuntimeException("The object is not comparable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::compare)");
        return const_cast<Object *>(this)->call(
            "__gt__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    bool Object::operator>=(const Variant &other) const {
        if (!hasProperty("__ge__"))
            throw RuntimeException("The object is not comparable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::compare)");
        return const_cast<Object *>(this)->call(
            "__ge__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    bool Object::operator<(const Variant &other) const {
        if (!hasProperty("__lt__"))
            throw RuntimeException("The object is not comparable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::compare)");
        return const_cast<Object *>(this)->call(
            "__lt__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    bool Object::operator<=(const Variant &other) const {
        if (!hasProperty("__le__"))
            throw RuntimeException("The object is not comparable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::compare)");
        return const_cast<Object *>(this)->call(
            "__le__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator+(const Variant &other) const {
        if (!hasProperty("__add__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__add__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator-(const Variant &other) const {
        if (!hasProperty("__sub__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__sub__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator*(const Variant &other) const {
        if (!hasProperty("__mul__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__mul__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator/(const Variant &other) const {
        if (!hasProperty("__div__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__div__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator%(const Variant &other) const {
        if (!hasProperty("__mod__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__mod__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator&(const Variant &other) const {
        if (!hasProperty("__band__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__band__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator|(const Variant &other) const {
        if (!hasProperty("__bor__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__bor__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator^(const Variant &other) const {
        if (!hasProperty("__bxor__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__bxor__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator&&(const Variant &other) const {
        if (!hasProperty("__land__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__land__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator||(const Variant &other) const {
        if (!hasProperty("__lor__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__lor__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator<<(const Variant &other) const {
        if (!hasProperty("__lshift__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__lshift__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator>>(const Variant &other) const {
        if (!hasProperty("__rshift__"))
            throw RuntimeException("The object is not calculable.");
        if (!Ref<Variant>::hasReference(&other))
            throw CompilerException(
                "The operand has no reference. (Object::calculate)");
        return *const_cast<Object *>(this)->call(
            "__rshift__", {Ref<Variant>(const_cast<Variant *>(&other))});
    }
    Variant Object::operator!() const {
        if (!hasProperty("__not__"))
            throw RuntimeException("The object is not calculable.");
        return *const_cast<Object *>(this)->call("__not__");
    }
    Variant Object::operator-() const {
        if (!hasProperty("__neg__"))
            throw RuntimeException("The object is not calculable.");
        return *const_cast<Object *>(this)->call("__neg__");
    }
    Variant Object::operator~() const {
        if (!hasProperty("__rev__"))
            throw RuntimeException("The object is not calculable.");
        return *const_cast<Object *>(this)->call("__rev__");
    }
    Variant Object::operator++() {
        if (!hasProperty("__inc__"))
            throw RuntimeException("The object is not calculable.");
        return *const_cast<Object *>(this)->call("__inc__");
    }
    Variant Object::operator--() {
        if (!hasProperty("__red__"))
            throw RuntimeException("The object is not calculable.");
        return *const_cast<Object *>(this)->call("__red__");
    }
    Object::operator integer() {
        if (!hasProperty("__int__"))
            throw RuntimeException("The object is not convertible.");
        return *const_cast<Object *>(this)->call("__int__");
    }
    Object::operator real() {

        if (!hasProperty("__real__"))
            throw RuntimeException("The object is not convertible.");
        return *const_cast<Object *>(this)->call("__real__");
    }
    Object::operator bool() {
        if (!hasProperty("__bool__"))
            throw RuntimeException("The object is not convertible.");
        return *const_cast<Object *>(this)->call("__bool__");
    }
    Object::operator string() {
        if (!hasProperty("__stringify__")) {
            std::ostringstream os;
            os << "object: " << this;
            return os.str();
        }
        return *const_cast<Object *>(this)->call("__stringify__");
    }
} // namespace phi
