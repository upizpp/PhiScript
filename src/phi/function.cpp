#include "function.hpp"
#include <phi/runtime/evaluator.hpp>

namespace phi
{
    Ref<Variant> Function::call(const array &args)
    {
        if (isBuiltin())
            return new Variant{(*_M_callable)(args)};
        else
            return _M_method->call(args);
    }
    Ref<Variant> Method::call(const array &args)
    {
        Evaluator evaluator(*_M_method);
        Env& env = evaluator.pushEnv();
        if (_M_binds){
            uinteger size = std::min(args.size(), _M_binds->size());
            for (uinteger i = 0; i < size; i++)
                env.setLocal(*(*_M_binds)[i], args[i]);
        }
        env.setLocal("this", _M_this);
        env.setLocal("__args__", new Variant{args});
        Ref<Variant> res = evaluator.eval();
        return res;
    }
} // namespace phi