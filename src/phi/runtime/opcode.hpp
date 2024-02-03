#pragma once
#include <phi/typedef.hpp>

namespace phi
{
    using cmd_t = uint8_t;
    using arg_t = uint64_t;
    class OPCode
    {
    public:
        enum class Command : cmd_t{
            // stack control
            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            BAND,
            BOR,
            LSHIFT,
            RSHIFT,
            XOR,
            LAND,
            LOR,
            ASSIGN,
            LT,
            LE,
            RT,
            RE,
            EQ,
            NE,
            NOT,
            NEG,
            REV,
            INC,
            RED,
            CALL,
            ACCESS,
            CLEAR,
            // env control
            PUSH_VAL,
            POP_VAL,
            PUSH_ENV,
            POP_ENV,
            // stream control
            IFFALSE,
            IFTRUE,
            GOTO
        };

    private:
        Command _M_op;
        arg_t _M_value;
    public:
        OPCode(Command op, cmd_t value) : _M_op(op), _M_value(value) {}
        OPCode(Command op) : _M_op(op), _M_value(0) {}

        Command op() const { return _M_op; }
        void op(Command value) { _M_op = value; }
        cmd_t value() const { return _M_value; }
        void value(cmd_t value) { _M_value = value; }
    };
} // namespace phi
