#pragma once
#include <map>
#include <typedef.hpp>

#undef EOF

namespace phi {
namespace token {

enum Tag : uint16_t {
    EOF = 255,
    ID,
    INT,
    REAL,
    STRING,
    // operators
    AND,
    OR,
    NOT,
    LE,
    GE,
    EQ,
    NE,
    INC, // increase
    RED, // reduce
    LSHIFT,
    RSHIFT,
    POW,
    DCPY,  // deep copy
    ARROW, // =>
    // keywords
    IN,
    PASS,
    RETURN,
    FN,
    VAR,
    IF,
    ELSE,
    FOR,
    WHILE,
    DO,
    BREAK,
    CONTINUE,
    TRUE,
    FALSE,
    EXPORT,
    IMPORT,
    AS,
    EVAL,
    DELETE,
    LIKELY,
    UNLIKELY
};

struct Token {
    union {
        Tag tag;
        char_t _;
    };

    Token() = default;
    Token(uint16_t ch) : tag((Tag)ch) {}

    string toString() const;
    static string tagToString(Tag);
    static unique_ptr<Token> getKeyword(const string &keyword);
    static unique_ptr<Token> getDoubleOperator(const string &op);

    const int64_t &getInt() const;
    const real_t &getReal() const;
    const string &getString() const;
    const shared_ptr<string> &getStringPtr() const;
};

struct Word : Token {
    Word(const string &w);
    Word(const string &w, uint16_t tag);

    const shared_ptr<string> &wordPtr() const { return _M_word; }
    const string &word() const { return *wordPtr(); }

  private:
    static std::map<string, shared_ptr<string>> _M_words;
    shared_ptr<string> _M_word;
};

struct Integer : Token {
    int64_t num;

    Integer(int64_t n) : num(n), Token(INT) {}
};

struct Real : Token {
    real_t num;

    Real(real_t n) : num(n), Token(REAL) {}
};

std::ostream &operator<<(std::ostream &os, const Token &token);

} // namespace token

} // namespace phi
