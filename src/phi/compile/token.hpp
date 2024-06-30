#pragma once
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
    Tag tag;

    Token() = default;
    Token(uint16_t ch) : tag((Tag)ch) {}

    string toString() const;
    static string tagToString(Tag);
    static unique_ptr<Token> getKeyword(const string &keyword);
    static unique_ptr<Token> getDoubleOperator(const string &op);
};

struct Word : Token {
    string word;

    Word(string w) : word(w), Token(ID) {}
    Word(string w, uint16_t tag) : word(w), Token(tag) {}
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
