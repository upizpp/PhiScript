#include "preprocessor.hpp"
#include <iostream>

namespace phi {
unique_ptr<Token> Preprocessor::next() {
    unique_ptr<Token> token = _M_generator.next();
    uint16_t tag = (uint16_t)token->tag();
    {
        auto it = _M_pairs.find(tag);
        if (it != _M_pairs.end())
            it->second++;
    }
    {
        auto it = _M_closers.find(tag);
        if (it != _M_closers.end())
            _M_pairs[it->second]--;
    }

    // ASI
    auto skipNewlines = [&]() {
        while (tag == '\n') {
            token.reset(_M_generator.next().release());
            tag = (uint16_t)token->tag();
            ++line;
        }
    };
    if (tag == '\n') {
        if (count_pairs() == 0) {
            if (!_M_ast_flag) {
                token.reset(new Token(';'));
                ++line;
            } else
                skipNewlines();
        } else
            skipNewlines();
    }
    _M_ast_flag = tag == '\n';
    if (tag == 0)
        token.reset(new Token(';'));
    _M_last = token->tag();
    return token;
}
} // namespace phi
