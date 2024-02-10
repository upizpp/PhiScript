#pragma once

#undef CLASS_NAME
#define CLASS_NAME IOLib
#undef MODULE_NAME
#define MODULE_NAME "io"

#define MESSAGE_BODY                          \
    FIELD_METHOD(print, void, RestParameters) \
    FIELD_METHOD(input, string, string)       \
    FIELD_METHOD(get_number, integer, string)

#include "reflect_header"