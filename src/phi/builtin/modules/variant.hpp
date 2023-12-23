#pragma once

#undef MESSAGE_HEADER
#undef MESSAGE_HEADER_L
#undef MESSAGE_BODY
#define MESSAGE_HEADER VariantLib
#define MESSAGE_HEADER_L variant_lib
#define MESSAGE_BODY                                           \
    FIELD_PROPERTY(TYPE_NIL, Variant::Type::NIL)               \
    FIELD_PROPERTY(TYPE_INT, Variant::Type::INT)               \
    FIELD_PROPERTY(TYPE_REAL, Variant::Type::REAL)             \
    FIELD_PROPERTY(TYPE_BOOL, Variant::Type::BOOL)             \
    FIELD_PROPERTY(TYPE_STRING, Variant::Type::STRING)         \
    FIELD_PROPERTY(TYPE_ARRAY, Variant::Type::ARRAY)           \
    FIELD_PROPERTY(TYPE_LIST, Variant::Type::LIST)             \
    FIELD_PROPERTY(TYPE_DICTIONARY, Variant::Type::DICTIONARY) \
    FIELD_PROPERTY(TYPE_FUNCTION, Variant::Type::FUNCTION)     \
    FIELD_PROPERTY(TYPE_OBJECT, Variant::Type::OBJECT)         \
    FIELD_PROPERTY(TYPE_ANY, Variant::Type::ANY)               \
    FIELD_METHOD(convert)                                      \
    FIELD_METHOD(type)                                       \
    FIELD_METHOD(type_s)                                     \
    FIELD_METHOD(stringify_type)

#include "reflect_header"