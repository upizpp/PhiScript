#include "op_code.hpp"
#include "../singleton.hpp"
#include "../variant.hpp"
#include "../reference.hpp"
#include <sstream>

namespace phi
{
    namespace runtime
    {
        OPCode::OPCode(Command command) : m_command(command)
        {
        }

        OPCode::OPCode(const Reference<Variant>& data) : m_command(Command::DATA), m_data(data)
        {
        }

        OPCode::OPCode(const OPCode &code) : m_command(code.command()), m_data(code.m_data), m_position(code.position())
        {
        }

        OPCode &OPCode::operator=(const OPCode &code)
        {
            m_command = code.command();
            m_data = std::move(code.m_data);
            return *this;
        }

        std::string OPCode::stringify() const
        {
            std::ostringstream ss;
            ss << cmdStr(command());
            if (command() == Command::DATA)
            {
                ss << "\t";
                ss << m_data->toString();
            }
            ss << "\t";
            ss << m_position;
            return ss.str();
        }

        std::string OPCode::cmdStr(const Command &command)
        {
            switch (command)
            {
            case Command::DATA:
                return "DATA";
            case Command::ADD:
                return "ADD";
            case Command::SUB:
                return "SUB";
            case Command::MUL:
                return "MUL";
            case Command::DIV:
                return "DIV";
            case Command::POWER:
                return "POWER";
            case Command::MOD:
                return "MOD";
            case Command::NEG:
                return "NEG";
            case Command::BNOT:
                return "BNOT";
            case Command::BOR:
                return "BOR";
            case Command::BAND:
                return "BAND";
            case Command::BXOR:
                return "BXOR";
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
            case Command::LAND:
                return "LAND";
            case Command::LOR:
                return "LOR";
            case Command::LNOT:
                return "LNOT";
            case Command::LXOR:
                return "LXOR";
            case Command::COPY:
                return "COPY";
            case Command::LOAD:
                return "LOAD";
            case Command::ALLOCATE:
                return "ALLOCATE";
            case Command::ASSIGN:
                return "ASSIGN";
            case Command::ADDASN:
                return "ADDASN";
            case Command::SUBASN:
                return "SUBASN";
            case Command::MULASN:
                return "MULASN";
            case Command::DIVASN:
                return "DIVASN";
            case Command::MODASN:
                return "MODASN";
            case Command::ANDASN:
                return "ANDASN";
            case Command::ORASN:
                return "ORASN";
            case Command::XORASN:
                return "XORASN";
            case Command::LSASN:
                return "LSASN";
            case Command::RSASN:
                return "RSASN";
            case Command::CALL:
                return "CALL";
            case Command::ACCESS:
                return "ACCESS";
            case Command::MAKE_ARRAY:
                return "MAKE_ARRAY";
            case Command::MAKE_DICTIONARY:
                return "MAKE_DICTIONARY";
            case Command::ARGS:
                return "ARGS";
            case Command::FN_LOCATE:
                return "FN_LOCATE";
            default:
                return "UNKNOWN";
            }
        }
    } // namespace runtime
} // namespace phi
