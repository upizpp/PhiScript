#include "exception.hpp"
#include <phi/runtime/evaluator.hpp>

namespace phi
{
    OperateNullException::OperateNullException(const VariantPacker& value, const string &operation): 
        _M_operation(operation), _M_value(value.hasName() ? '(' + value.name() + ')' : "")
    {
    }
} // namespace phi