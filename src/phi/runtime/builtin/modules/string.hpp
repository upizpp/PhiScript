#pragma once

#undef CLASS_NAME
#define CLASS_NAME StringLib
#undef MODULE_NAME
#define MODULE_NAME "string"

#define MESSAGE_BODY \
    FIELD_METHOD(is_number, bool, const string &)

#include "reflect_header"