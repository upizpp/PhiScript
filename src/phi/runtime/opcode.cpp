#include "opcode.hpp"
#include <exception.hpp>

namespace phi {
using namespace token;
OPCode::OPCode(Tag tag) {
    switch (tag) {
    case '=':
        _M_command = Command::ASSIGN;
        break;
    case '+':
        _M_command = Command::ADD;
        break;
    case '-':
        _M_command = Command::SUB;
        break;
    case '*':
        _M_command = Command::MUL;
        break;
    case '/':
        _M_command = Command::DIV;
        break;
    case '%':
        _M_command = Command::MOD;
        break;
    case Tag::POW:
        _M_command = Command::POW;
        break;
    case Tag::LSHIFT:
        _M_command = Command::LSHIFT;
        break;
    case Tag::RSHIFT:
        _M_command = Command::RSHIFT;
        break;
    case '&':
        _M_command = Command::BAND;
        break;
    case '|':
        _M_command = Command::BOR;
        break;
    case '^':
        _M_command = Command::BXOR;
        break;
    case '~':
        _M_command = Command::BNOT;
        break;
    case Tag::EQ:
        _M_command = Command::EQ;
        break;
    case Tag::NE:
        _M_command = Command::NE;
        break;
    case Tag::GE:
        _M_command = Command::GE;
        break;
    case Tag::LE:
        _M_command = Command::LE;
        break;
    case Tag::INC:
        _M_command = Command::INC;
        break;
    case Tag::RED:
        _M_command = Command::RED;
        break;
    case Tag::AND:
        _M_command = Command::LAND;
        break;
    case Tag::OR:
        _M_command = Command::LOR;
        break;
    case '!':
        _M_command = Command::LNOT;
        break;
    default:
        throw CompilerException("Unknown tag.");
    }
}
string_view OPCode::toString() const {
    switch (command()) {
    case Command::CLEAR:
        return "CLEAR";
    case Command::ADD:
        return "ADD";
    case Command::SUB:
        return "SUB";
    case Command::MUL:
        return "MUL";
    case Command::DIV:
        return "DIV";
    case Command::MOD:
        return "MOD";
    case Command::POW:
        return "POW";
    case Command::BAND:
        return "BAND";
    case Command::BOR:
        return "BOR";
    case Command::BNOT:
        return "BNOT";
    case Command::BXOR:
        return "BXOR";
    case Command::LAND:
        return "LAND";
    case Command::LOR:
        return "LOR";
    case Command::LNOT:
        return "LNOT";
    case Command::LSHIFT:
        return "LSHIFT";
    case Command::RSHIFT:
        return "RSHIFT";
    case Command::EQ:
        return "EQ";
    case Command::NE:
        return "NE";
    case Command::LT:
        return "LT";
    case Command::LE:
        return "LE";
    case Command::GT:
        return "GT";
    case Command::GE:
        return "GE";
    case Command::INC:
        return "INC";
    case Command::RED:
        return "RED";
    case Command::ASSIGN:
        return "ASSIGN";
    case Command::ACCESS:
        return "ACCESS";
    case Command::FLAG:
        return "FLAG";
    case Command::CALL:
        return "CALL";
    case Command::LOAD:
        return "LOAD";
    case Command::LOAD_CONST:
        return "LOAD_CONST";
    case Command::ALLOCATE:
        return "ALLOCATE";
    case Command::ENTER_BLOCK:
        return "ENTER_BLOCK";
    case Command::EXIT_BLOCK:
        return "EXIT_BLOCK";
    case Command::GOTO:
        return "GOTO";
    case Command::IFTRUE:
        return "IFTRUE";
    case Command::IFFALSE:
        return "IFFALSE";
    default:
        throw CompilerException("Unknown command.");
    }
}
} // namespace phi
