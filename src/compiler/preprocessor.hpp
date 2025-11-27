#pragma once
#include "token.hpp"
#include <map>

namespace phi {
struct Preprocessor : TokenGenerator {
    Preprocessor(TokenGenerator &generator)
        : _M_generator(generator), _M_pairs({{'(', 0}, {'[', 0}}),
          _M_closers({{')', '('}, {']', '['}}) {}
    virtual unique_ptr<Token> next() override;
    virtual bool eof() override { return _M_generator.eof(); }

  private:
    TokenGenerator &_M_generator;
    Token::Tag _M_last;

    std::map<char, uint64_t> _M_pairs;
    std::map<char, char> _M_closers;
    bool _M_ast_flag = false;

    inline uint64_t count_pairs() const {
        uint64_t sum = 0;
        for (auto &&pair : _M_pairs)
            sum += pair.second;
        return sum;
    }
};

} // namespace phi
