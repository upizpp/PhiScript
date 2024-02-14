#include "token.hpp"

namespace phi
{
	namespace token
	{
#define WORD(what, tag)                   \
	{                                     \
		what, new Word { what, Tag::tag } \
	}

		map<string, Ref<string>> Word::_M_symbols;
		map<string, Ref<Word>> Word::_M_words{
			WORD("return", RETURN),
			WORD("fn", FN),
			WORD("func", FN),
			WORD("function", FN),
			WORD("if", IF),
			WORD("else", ELSE),
			WORD("for", FOR),
			WORD("while", WHILE),
			WORD("do", DO),
			WORD("break", BREAK),
			WORD("continue", CONTINUE),
			WORD("import", IMPORT),
			WORD("export", EXPORT),
			WORD("as", AS),
			WORD("var", VAR),
			WORD("eval", EVAL),
			WORD("true", TRUE),
			WORD("false", FALSE),
			WORD("del", DELETE),
			WORD("likely", LIKELY),
			WORD("unlikely", UNLIKELY),
			WORD("pass", PASS),
			WORD("=>", ARROW),
			WORD("&&", AND),
			WORD("||", OR),
			WORD("and", AND),
			WORD("or", OR),
			WORD("not", NOT),
			WORD("==", EQ),
			WORD("!=", NE),
			WORD(">=", GE),
			WORD("<=", LE),
			WORD("++", INC),
			WORD("--", RED),
			WORD("<<", LSHIFT),
			WORD(">>", RSHIFT),
			WORD("**", POW),
			WORD("@@", DCPY),
		};

		void Word::put(const string &word)
		{
			_M_words.insert({word, new Word(word, Tag::ID)});
		}
		void Word::put(const string &word, Tag tag)
		{
			_M_words.insert({word, new Word(word, tag)});
		}
		bool Word::has(const string &word)
		{
			return _M_words.find(word) != _M_words.end();
		}
		void Word::value(const string &value)
		{
			if (_M_symbols.find(value) == _M_symbols.end())
			{
				_M_symbols[value] = new string{value};
			}
			_M_value = _M_symbols[value];
		}
		size_t Word::hash() const
		{
			return tag() + std::hash<string>()(*_M_value);
		}
		Ref<Word> Word::get(const string &word)
		{
			return new Word{*_M_words[word]};
		}

#define REIDENTIFY_IMPL             \
	if (_M_tag > 256)               \
		switch (_M_tag)             \
		{                           \
		case Tag::REAL:             \
			return (Real *)this;    \
		case Tag::INT:              \
			return (Integer *)this; \
		default:                    \
			return (Word *)this;    \
		}                           \
	return this

		Token *Token::reidentify()
		{
			REIDENTIFY_IMPL;
		}

		const Token *Token::reidentify() const
		{

			REIDENTIFY_IMPL;
		}

		bool Token::operator==(const Token &token) const
		{
			return equals(*this, token);
		}

		size_t Token::hash() const
		{
			return tag();
		}

		size_t Integer::hash() const
		{
			return tag() + value();
		}

		size_t Real::hash() const
		{
			return tag() + value();
		}

		string toString(tag_t tag)
		{
			if (tag < 256)
				return string(1, char(tag));
			switch (tag)
			{
			case Tag::ID:
				return "identifier";
			case Tag::STRING:
				return "string";
			case Tag::INT:
				return "integer";
			case Tag::REAL:
				return "real";
			case Tag::PASS:
				return "pass";
			case Tag::VAR:
				return "var";
			case Tag::IF:
				return "if";
			case Tag::ELSE:
				return "else";
			case Tag::FOR:
				return "for";
			case Tag::WHILE:
				return "while";
			case Tag::DO:
				return "do";
			case Tag::BREAK:
				return "break";
			case Tag::CONTINUE:
				return "continue";
			case Tag::TRUE:
				return "true";
			case Tag::FALSE:
				return "false";
			case Tag::IMPORT:
				return "import";
			case Tag::EXPORT:
				return "export";
			case Tag::RETURN:
				return "return";
			case Tag::FN:
				return "fn";
			case Tag::AS:
				return "as";
			case Tag::EVAL:
				return "eval";
			case Tag::DELETE:
				return "del";
			case Tag::LIKELY:
				return "likely";
			case Tag::UNLIKELY:
				return "unlikely";
			case Tag::ARROW:
				return "=>";
			case Tag::NOT:
				return "!";
			case Tag::AND:
				return "&&";
			case Tag::OR:
				return "||";
			case Tag::EQ:
				return "==";
			case Tag::NE:
				return "!=";
			case Tag::GE:
				return ">=";
			case Tag::LE:
				return "<=";
			case Tag::INC:
				return "++";
			case Tag::RED:
				return "--";
			case Tag::LSHIFT:
				return "<<";
			case Tag::RSHIFT:
				return ">>";
			case Tag::POW:
				return "**";
			case Tag::DCPY:
				return "@@";
			}
			return "unknown";
		}
	} // namespace token
} // namespace phi
