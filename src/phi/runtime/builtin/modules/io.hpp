#pragma once

#define CLASS_NAME IO
#define MODULE_NAME "io"
#define MESSAGE_BODY                          \
    FIELD_METHOD(print, void, RestParameters) \
    FIELD_METHOD(input, string, string)

#include "reflect_header"