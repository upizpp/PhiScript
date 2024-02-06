#include "opcode.hpp"
#include <iomanip>
#include <sstream>
#include <phi/exception.hpp>
#include <phi/runtime/state.hpp>
#include <phi/compiler/generator.hpp>
#include <limits>

namespace phi
{
	extern const arg_t STACK_TOP = std::numeric_limits<arg_t>::max();

	string OPCode::toString() const
	{
		static std::ostringstream os;
		os.str("");
		os << std::left << std::setw(3) << std::to_string(_M_op) << ' ';
		os << std::left << std::setw(16) << stringifyCmd(_M_op);

		string v = std::to_string(_M_value);
		if (_M_op >= Command::LOAD && _M_op <= Command::ALLOCATE)
			v += "(" + State::lookupGlobal(_M_value)->toString() + ")";
		else if (_M_op >= Command::IFFALSE && _M_op <= Command::GOTO)
			v += "(" + std::to_string(Generator::instance()->getState().label(_M_value)) + ")";

		os << std::left << std::setw(16) << v;
		
		return os.str();
	}

	string OPCode::stringifyCmd(Command cmd)
	{
		switch (cmd)
		{
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
            case Command::LSHIFT:
				return "LSHIFT";
            case Command::RSHIFT:
				return "RSHIFT";
            case Command::BXOR:
				return "BXOR";
            case Command::LAND:
				return "LAND";
            case Command::LOR:
				return "LOR";
            case Command::ASSIGN:
				return "ASSIGN";
            case Command::LT:
				return "LT";
            case Command::LE:
				return "LE";
            case Command::GT:
				return "GT";
            case Command::GE:
				return "GE";
            case Command::EQ:
				return "EQ";
            case Command::NE:
				return "NE";
            case Command::NOT:
				return "NOT";
            case Command::NEG:
				return "NEG";
            case Command::REV:
				return "REV";
            case Command::INC:
				return "INC";
            case Command::RED:
				return "RED";
			case Command::COPY:
				return "COPY";
			case Command::DCPY:
				return "DCPY";
            case Command::ARGS:
				return "ARGS";
            case Command::CALL:
				return "CALL";
            case Command::ACCESS:
				return "ACCESS";
            case Command::MAKE_ARRAY:
				return "MAKE_ARRAY";
            case Command::MAKE_DICT:
				return "MAKE_DICT";
            case Command::IMPORT:
				return "IMPORT";
            case Command::CLEAR:
				return "CLEAR";
            case Command::DEL:
				return "DEL";
            case Command::LOAD:
				return "LOAD";
            case Command::LOAD_CONST:
				return "LOAD_CONST";
            case Command::ALLOCATE:
				return "ALLOCATE";
            case Command::PUSH_VAL:
				return "PUSH_VAL";
            case Command::POP_VAL:
				return "POP_VAL";
            case Command::PUSH_ENV:
				return "PUSH_ENV";
            case Command::POP_ENV:
				return "POP_ENV";
            case Command::IFFALSE:
				return "IFFALSE";
            case Command::IFTRUE:
				return "IFTRUE";
            case Command::GOTO:
				return "GOTO";
			case Command::RETURN:
				return "RETURN";
		}
		return "UNKNOWN";
	}
	OPCode::Command OPCode::unaryToCommand(token::tag_t tag)
	{
		switch (tag)
		{
		case '-':
			return Command::NEG;
		case '~':
			return Command::REV;
		case '!':
		case token::Tag::NOT:
			return Command::NOT;
		case '@':
			return Command::COPY;
		case token::Tag::DCPY:
			return Command::DCPY;
		case token::Tag::INC:
			return Command::INC;
		case token::Tag::RED:
			return Command::RED;
		default:
			throw ValueException("The tag '%s' cannot be converted to command.", token::toString(tag).c_str());
		}
	}
	OPCode::Command OPCode::binaryToCommand(token::tag_t tag)
	{
		switch (tag)
		{
		case '+':
			return Command::ADD;
		case '-':
			return Command::SUB;
		case '*':
			return Command::MUL;
		case '/':
			return Command::DIV;
		case '%':
			return Command::MOD;
		case token::Tag::POW:
			return Command::POW;
		case '&':
			return Command::BAND;
		case '|':
			return Command::BOR;
		case '^':
			return Command::BXOR;
		case token::Tag::LSHIFT:
			return Command::LSHIFT;
		case token::Tag::RSHIFT:
			return Command::RSHIFT;
		case token::Tag::AND:
			return Command::LAND;
		case token::Tag::OR:
			return Command::LOR;
		case '=':
			return Command::ASSIGN;
		case '<':
			return Command::LT;
		case token::Tag::LE:
			return Command::LE;
		case '>':
			return Command::GT;
		case token::Tag::GE:
			return Command::GE;
		case token::Tag::EQ:
			return Command::EQ;
		case token::Tag::NE:
			return Command::NE;
		default:
			throw ValueException("The tag '%s' cannot be converted to command.", token::toString(tag).c_str());
		}
	}
} // namespace phi