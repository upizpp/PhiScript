#pragma once

#undef MESSAGE_HEADER
#undef MESSAGE_HEADER_L
#undef MESSAGE_BODY
#define MESSAGE_HEADER ArrayLib
#define MESSAGE_HEADER_L array_lib
#define MESSAGE_BODY \
    FIELD_METHOD(size)\
    FIELD_METHOD(push_back) 

#include "reflect_header"