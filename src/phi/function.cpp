#include "function.hpp"
#include <phi/runtime/evaluator.hpp>

namespace phi
{
    Ref<Variant> Function::call(const array &args)
    {
        if (isBuiltin())
            return (*_M_callable)(args);
        else
            return _M_method->call(args);
    }
    
    Ref<Variant> &Function::access(const array &args)
    {
        if (args.size() == 1 && args[0]->type() == Variant::Type::STRING)
        {
            string str = *args[0];
            if (_M_properties.find(str) == _M_properties.end())
                _M_properties[str] = new Variant;
            return _M_properties[str];
        }
        else
        {
            throw ArgumentException(1, args.size(), __FUNCTION__);
        }
    }
    Function Function::deepCopy()
    {
        Function res = copy();
        for (auto &&pair : _M_properties)
            res._M_properties[pair.first] = new Variant{pair.second->deepCopy()};
        return res;
    }
    Ref<Variant> Method::call(const array &args)
    {
        Evaluator evaluator(*_M_method);
        Env &env = evaluator.pushEnv();
        if (_M_binds)
        {
            uinteger size = std::min(args.size(), _M_binds->size());
            for (uinteger i = 0; i < size; i++)
                env.setLocal(*(*_M_binds)[i], args[i]);
        }
        if (_M_this)
            env.setLocal("this", _M_this);
        env.setLocal("__args__", new Variant{args});
        return evaluator.eval();
    }
} // namespace phi