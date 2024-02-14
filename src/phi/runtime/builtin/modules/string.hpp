#pragma once

#undef CLASS_NAME
#define CLASS_NAME StringLib
#undef MODULE_NAME
#define MODULE_NAME "string"
#undef MESSAGE_BODY
#define MESSAGE_BODY                                                                                 \
    FIELD_METHOD(is_empty, bool, const string &base)                                                 \
    FIELD_METHOD(is_number, bool, const string &base)                                                \
    FIELD_METHOD(split, vector<string>, const string &base, const string &delimiter, Optional<bool>) \
    FIELD_METHOD(get_slice, string, const string &base, const string& delimiter, integer index)                               \
    FIELD_METHOD(get_slice_count, integer, const string &base, const string &delimiter)              \
    FIELD_METHOD(absolute, string, const string &base, const string &relative)

#include "reflect_header"