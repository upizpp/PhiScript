#pragma once

#undef CLASS_NAME
#define CLASS_NAME StringLib
#undef MODULE_NAME
#define MODULE_NAME "string"
#undef MESSAGE_BODY
#define MESSAGE_BODY                                                           \
    FIELD_METHOD(is_empty, bool, const string &self)                           \
    FIELD_METHOD(is_number, bool, const string &self)                          \
    FIELD_METHOD(begins_with, bool, const string &self, const string &what)                             \ 
    FIELD_METHOD(ends_with, bool, const string &self, const string &what)                               \                                        
    FIELD_METHOD(                                                              \
        split, vector<string>, const string &self, const string &delimiter,    \
        Optional<bool>) FIELD_METHOD(get_slice, string, const string &self,    \
                                     const string &delimiter, integer index)   \
        FIELD_METHOD(get_slice_count, integer, const string &self,             \
                     const string &delimiter)                                  \
            FIELD_METHOD(get_extension, string, const string &self)            \
                FIELD_METHOD(get_basedir, string, const string &self)          \
                    FIELD_METHOD(path_join, string, const string &self,        \
                                 const string &to_join)                        \
                        FIELD_METHOD(ltrim, string, const string &self,        \
                                     OptionalRef<string> ws)                   \
                            FIELD_METHOD(rtrim, string, const string &self,    \
                                         OptionalRef<string> ws)               \
                                FIELD_METHOD(trim, string, const string &self, \
                                             OptionalRef<string> ws)           \
                                    FIELD_METHOD(absolute, string,             \
                                                 const string &self,           \
                                                 const string &relative)

#include "reflect_header"