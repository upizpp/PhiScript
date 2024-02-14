#pragma once

#undef CLASS_NAME
#define CLASS_NAME FileSystemLib
#undef MODULE_NAME
#define MODULE_NAME "fs"
#undef MESSAGE_BODY
#define MESSAGE_BODY \
    FIELD_METHOD(is_dir, bool, const string&)\
    FIELD_METHOD(is_file, bool, const string&)\
    FIELD_METHOD(type, integer, const string&)\
    FIELD_METHOD(exists, bool, const string&)

#include "reflect_header"