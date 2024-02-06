#pragma once
#include <phi/typedef.hpp>
#include <phi/compiler/token.hpp>

namespace phi
{
    using cmd_t = uint8_t;
    using arg_t = uint32_t;
    
    extern const arg_t STACK_TOP;

    class OPCode
    {
    public:
        enum Command : cmd_t{
            // stack control
            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            POW,
            BAND,
            BOR,
            BXOR,
            LSHIFT,
            RSHIFT,
            LAND,
            LOR,
            ASSIGN,
            LT,
            LE,
            GT,
            GE,
            EQ,
            NE,
            NOT,
            NEG,
            REV,
            INC,
            RED,

            ARGS,
            CALL,
            ACCESS,

            MAKE_ARRAY,
            MAKE_DICT,

            IMPORT,

            CLEAR,
            DEL,

            LOAD,
            LOAD_CONST,
            ALLOCATE,
            // env control
            PUSH_VAL,
            POP_VAL,
            PUSH_ENV,
            POP_ENV,
            // stream control
            IFFALSE,
            IFTRUE,
            GOTO,
            RETURN
        };

    private:
        Command _M_op;
        arg_t _M_value;
    public:
        OPCode(Command opt, arg_t value) : _M_op(opt), _M_value(value) {}
        OPCode(Command opt) : _M_op(opt), _M_value(0) {}
        OPCode(const OPCode&) = default;

        OPCode& operator=(const OPCode&) = default;

        Command opt() const { return _M_op; }
        void opt(Command value) { _M_op = value; }
        arg_t value() const { return _M_value; }
        void value(arg_t value) { _M_value = value; }

        string toString() const;

        static string stringifyCmd(Command);
        static Command unaryToCommand(token::tag_t);
        static Command binaryToCommand(token::tag_t);
    };

    using CodeSeq = vector<OPCode>;

    inline std::ostream& operator<<(std::ostream& os, const OPCode& code)
    {
        return os << code.toString();
    }
} // namespace phi
