#pragma once
#include <typedef.hpp>

#undef EOF

namespace phi {
namespace token {

enum Type {
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
    uint16_t id;

    Token() = default;
    Token(uint16_t ch) : id(ch) {}

    string toString() const;
    static unique_ptr<Token> getKeyword(const string &keyword);
    static unique_ptr<Token> getDoubleOperator(const string &op);
};

struct Word : Token {
    string word;

    Word(string w) : word(w), Token(ID) {}
    Word(string w, uint16_t id) : word(w), Token(id) {}
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
