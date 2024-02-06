#include "function.hpp"
#include <phi/runtime/evaluator.hpp>

namespace phi
{
    Ref<Variant> Function::call(array &args)
    {
        if (isBuiltin())
            return new Variant{(*_M_callable)(args)};
        else
            return _M_method->call(args);
    }
    Ref<Variant> Method::call(array &args)
    {
        Evaluator evaluator(*_M_method);
        Env& env = evaluator.pushEnv();
        uinteger size = std::min(args.size(), _M_binds.size());
        for (uinteger i = 0; i < size; i++)
            env.setLocal(*_M_binds[i], args[i]);
        Ref<Variant> res = evaluator.eval();
        evaluator.popEnv();
        return res;
    }
} // namespace phi