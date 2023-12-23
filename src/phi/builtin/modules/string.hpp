#pragma once

#undef MESSAGE_HEADER
#undef MESSAGE_HEADER_L
#undef MESSAGE_BODY
#define MESSAGE_HEADER StringLib
#define MESSAGE_HEADER_L string_lib
#define MESSAGE_BODY \
    FIELD_METHOD(is_integer)\
    FIELD_METHOD(is_number)

#include "reflect_header"