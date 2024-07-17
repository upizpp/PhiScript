#pragma once
#include <compile/token.hpp>
#include <typedef.hpp>

#define DEFINE_EXCEPTION(name, parent)                                         \
    struct name : parent {                                                     \
        using parent::parent;                                                  \
        virtual string className() const override { return #name; }            \
    }

namespace phi {
struct Exception {
    const string &msg() const { return _M_msg; }
    virtual string what() const { return msg(); }

    virtual string className() const { return "Exception"; }

    Exception() = default;
    Exception(const string &msg) : _M_msg(msg) {}

  private:
    string _M_msg;
};

DEFINE_EXCEPTION(RuntimeException, Exception);
DEFINE_EXCEPTION(ConvertException, RuntimeException);
DEFINE_EXCEPTION(CompileException, Exception);
DEFINE_EXCEPTION(CompilerException, RuntimeException);
DEFINE_EXCEPTION(SyntaxException, CompileException);

struct TokenException : CompileException {
    using CompileException::CompileException;

    TokenException(const token::Token &token) : _M_token(token) {}

    virtual string className() const override { return "TokenException"; }
    virtual string what() const {
        return "Unexpected token \'" + _M_token.toString() + '\'';
    }

  private:
    const token::Token &_M_token;
};

} // namespace phi
