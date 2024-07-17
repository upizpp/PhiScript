#pragma once
#include <compile/token.hpp>

namespace phi {
struct OPCode {
    enum Command : uint16_t {
        CLEAR,

        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        POW,
        BAND,
        BOR,
        BNOT,
        BXOR,
        LAND,
        LOR,
        LNOT,
        LSHIFT,
        RSHIFT,
        EQ,
        NE,
        LT,
        LE,
        GT,
        GE,
        INC,
        RED,
        ASSIGN,

        FLAG,
        ACCESS,
        CALL,

        LOAD,
        LOAD_CONST,
        ALLOCATE,

        ENTER_BLOCK,
        EXIT_BLOCK,

        GOTO,
        IFTRUE,
        IFFALSE,
    };

    PROPERTY(Command, command, Command::ADD)

    OPCode() = default;
    OPCode(Command command) : _M_command(command) {}
    OPCode(token::Tag tag);

    string_view toString() const;
};

} // namespace phi
