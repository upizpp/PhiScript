#pragma once

#undef MESSAGE_HEADER
#undef MESSAGE_HEADER_L
#undef MESSAGE_BODY
#define MESSAGE_HEADER MathLib
#define MESSAGE_HEADER_L math_lib
#define MESSAGE_BODY \
    FIELD_METHOD(sqrt)

#include "reflect_header"