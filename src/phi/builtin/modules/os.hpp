#pragma once

#undef MESSAGE_HEADER
#undef MESSAGE_HEADER_L
#undef MESSAGE_BODY
#define MESSAGE_HEADER OSLib
#define MESSAGE_HEADER_L os_lib
#define MESSAGE_BODY \
    FIELD_METHOD(system)

#include "reflect_header"