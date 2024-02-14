#pragma once

#undef CLASS_NAME
#define CLASS_NAME StringLib
#undef MODULE_NAME
#define MODULE_NAME "string"
#undef MESSAGE_BODY
#define MESSAGE_BODY \
    FIELD_METHOD(is_number, bool, const string &)\
    FIELD_METHOD(split, array, const string& str, const string& delimiter) \
    FIELD_METHOD(absolute, string, const string& base, const string& relative)

#include "reflect_header"