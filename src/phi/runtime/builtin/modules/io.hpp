#pragma once

#undef CLASS_NAME
#define CLASS_NAME IOLib
#undef MODULE_NAME
#define MODULE_NAME "io"
#undef MESSAGE_BODY
#define MESSAGE_BODY                                                           \
    FIELD_METHOD(print, void, RestParameters);                                 \
    FIELD_METHOD(print_variant, void, RestParameters);                         \
    FIELD_METHOD(input, string, const string &);                               \
    FIELD_METHOD(get_number, real, OptionalRef<const string>,                  \
                 OptionalRef<const string>,                                    \
                 OptionalRef<const array> whitelist);

#include "reflect_header"