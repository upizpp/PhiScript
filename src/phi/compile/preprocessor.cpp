#include "preprocessor.hpp"
#include <exception.hpp>
#include <map>
#include <set>

namespace phi {
unique_ptr<token::Token> Preprocessor::next() {
    unique_ptr<token::Token> token;

    if (_M_cache) {
        token.reset(_M_cache);
        _M_cache = nullptr;
    } else {
        token = _M_generator.next();
    }

    while (!eof()) {
        auto next_tok = _M_generator.next();
        if (!(    //
                ( //
                    token->id == '\n' && (next_tok->id == '\n' ||
                                          next_tok->id == token::Type::EOF)) //
                ||                                                           //
                (                                                            //
                    token->id == ';' &&                                      //
                    (next_tok->id == ';' ||                                  //
                     next_tok->id == '\n' ||                                 //
                     next_tok->id == token::Type::EOF                        //
                     ))                                                      //
                )                                                            //
        ) {
            _M_cache = next_tok.release();
            break;
        }
    }

    // ASI
    static const std::set<char_t> BeginPairs = {
        '{',
        '[',
        '(',
    };
    static const std::map<char_t, char_t> EndPairs = {
        {'}', '{'},
        {']', '['},
        {')', '('},
    };
    static const std::set<uint16_t> StructKeyword = {
        token::Type::IF,
        token::Type::FOR,
        token::Type::WHILE,
    };
    if (StructKeyword.find(token->id) != StructKeyword.end())
        ++_M_struct_keywords;
    if (BeginPairs.find(token->id) != BeginPairs.end()) {
        if (_M_unclosed_pair.find(token->id) == _M_unclosed_pair.end())
            _M_unclosed_pair[token->id] = 0;
        ++_M_unclosed_pair[token->id];
    }
    if (EndPairs.find(token->id) != EndPairs.end()) {
        char_t begin = EndPairs.at(token->id);
        if (_M_unclosed_pair.find(begin) == _M_unclosed_pair.end())
            throw TokenException(*token);
        --_M_unclosed_pair[begin];
        if (_M_unclosed_pair[begin] == 0)
            _M_unclosed_pair.erase(begin);
    }
    if (token->id == '\n' || token->id == token::Type::EOF) {
        if (_M_unclosed_pair.empty())
            if (_M_struct_keywords == 0)
                return make_unique<token::Token>(';');
            else
                --_M_struct_keywords;
        return next();
    }

    return token;
}
} // namespace phi