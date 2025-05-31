#pragma once
#include <global.hpp>
#include <iterator>
#include <map>
#include <ostream>

namespace phi {
struct Word;
struct Integer;
struct Real;

struct Token {
    enum class Tag : uint16_t {
        INT = 256,
        REAL,
        STRING,
        IDENTIFIER,

        VAR, // variable
        CON, // constant

        IF,
        ELSE,
        FOR,
        WHILE,
        BREAK,
        CONTINUE,
        FN,
        RETURN,
        CLASS,
        IMPORT,
        EXPORT,
        AS,

        AND,
        OR,
        NOT,

        DCP, // deep copy

        INC,
        DEC,

        EQ,
        NE,
        LE,
        GE,
        LSHIFT,
        RSHIFT,
    };

    Token(Tag tag) : _M_tag(tag) {}
    Token(char tag) : _M_tag((Tag)tag) {}

    template <typename T> const T &as() const;
    inline const string &readString() const;
    inline int_t readInt() const;
    inline real_t readReal() const;

    string stringify() const;

  private:
    PROPERTY(Tag, tag)
};

struct Word : Token {
    Word(const string &value) : _M_value(value), Token(Tag::IDENTIFIER) {
        auto it = _M_words.find(value);
        if (it != _M_words.end())
            tag() = it->second;
    }
    Word(const string &value, bool) : _M_value(value), Token(Tag::STRING) {}

  private:
    static std::map<string, Tag> _M_words;

    PROPERTY(string, value)
};

struct Integer : Token {
    Integer(int64_t value) : _M_value(value), Token(Tag::INT) {}

  private:
    PROPERTY(int64_t, value)
};

struct Real : Token {
    Real(real_t value) : _M_value(value), Token(Tag::REAL) {}

  private:
    PROPERTY(real_t, value)
};

template <typename T> inline const T &Token::as() const {
    return *static_cast<const T *>(this);
}

std::ostream &operator<<(std::ostream &os, const Token &token);

struct TokenGenerator {
    virtual unique_ptr<Token> next() = 0;
    virtual bool eof() = 0;
};
} // namespace phi
