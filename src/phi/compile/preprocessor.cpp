#include "preprocessor.hpp"
#include <exception.hpp>
#include <follower.hpp>
#include <map>
#include <set>


namespace phi {
unique_ptr<token::Token> Preprocessor::next() {
    unique_ptr<token::Token> token;

    if (_M_cache_line != -1)
        ProgramFollower::get().line = _M_cache_line;

    if (_M_cache) {
        token.reset(_M_cache);
        _M_cache = nullptr;
    } else {
        token = _M_generator.next();
    }

    while (!eof()) {
        uint64_t temp = ProgramFollower::get().line;
        auto next_tok = _M_generator.next();
        _M_cache_line = ProgramFollower::get().line;
        ProgramFollower::get().line = temp;
        if (!(    //
                ( //
                    token->tag == '\n' && (next_tok->tag == '\n' ||
                                           next_tok->tag == token::Tag::EOF)) //
                ||                                                            //
                (                                                             //
                    token->tag == ';' &&                                      //
                    (next_tok->tag == ';' ||                                  //
                     next_tok->tag == '\n' ||                                 //
                     next_tok->tag == token::Tag::EOF                         //
                     ))                                                       //
                )                                                             //
        ) {
            _M_cache = next_tok.release();
            break;
        }
    }

    // ASI
    static const std::set<char_t> BeginPairs = {
        '[',
        '(',
    };
    static const std::map<char_t, char_t> EndPairs = {
        {']', '['},
        {')', '('},
    };
    static const std::set<uint16_t> StructKeyword = {
        token::Tag::IF,    token::Tag::ELSE, token::Tag::FOR,
        token::Tag::WHILE, token::Tag::FN,   token::Tag::ARROW,
    };
    if (StructKeyword.find(token->tag) != StructKeyword.end())
        ++_M_struct_keywords;
    if (BeginPairs.find(token->tag) != BeginPairs.end()) {
        if (_M_unclosed_pair.find(token->tag) == _M_unclosed_pair.end())
            _M_unclosed_pair[token->tag] = 0;
        ++_M_unclosed_pair[token->tag];
    }
    if (EndPairs.find(token->tag) != EndPairs.end()) {
        char_t begin = EndPairs.at(token->tag);
        if (_M_unclosed_pair.find(begin) == _M_unclosed_pair.end())
            throw TokenException(*token);
        --_M_unclosed_pair[begin];
        if (_M_unclosed_pair[begin] == 0)
            _M_unclosed_pair.erase(begin);
    }
    if (token->tag == '\n' || token->tag == token::Tag::EOF) {
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