#pragma once

#undef MESSAGE_HEADER
#undef MESSAGE_HEADER_L
#undef MESSAGE_BODY
#define MESSAGE_HEADER IOLib
#define MESSAGE_HEADER_L io_lib
#define MESSAGE_BODY    \
    FIELD_METHOD(print) \
    FIELD_METHOD(input) \
    FIELD_METHOD(get_number)

#include "reflect_header"