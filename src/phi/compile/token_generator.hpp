#pragma once
#include <array>
#include <compile/token.hpp>
#include <exception.hpp>

namespace phi {
struct TokenGenerator {
    virtual unique_ptr<token::Token> next() = 0;
    virtual bool eof() = 0;
    virtual void reset() = 0;
};

template <size_t C = 8> struct CachedGenerator : TokenGenerator {
    CachedGenerator(TokenGenerator &g) : _M_generator(g) {
        _M_tail = _M_cache.begin();
    }

    virtual unique_ptr<token::Token> last() {
        if (_M_tail == _M_cache.end())
            throw Exception("Out of cache.");
        return std::move(*_M_tail--);
    }
    virtual unique_ptr<token::Token> next() override {
        if (_M_tail == _M_cache.end()) {
            {
                auto end = _M_cache.end();
                --end;
                for (auto it = _M_cache.begin(); it != end; ++it) {
                    auto it_next = it;
                    ++it_next;
                    it->swap(*it_next);
                }
            }
            _M_tail = _M_cache.end();
            --_M_tail;
        }
        auto tmp = _M_tail;
        ++tmp;
        if (tmp != _M_cache.end() && !*tmp || tmp == _M_cache.end())
            *_M_tail = _M_generator.next();
        return std::move(*_M_tail++);
    }
    virtual bool eof() override { return _M_generator.eof(); }
    virtual void reset() override {
        _M_tail = _M_cache.begin();
        _M_generator.reset();
    }

  private:
    TokenGenerator &_M_generator;
    using cache_t = std::array<unique_ptr<token::Token>, C>;
    cache_t _M_cache;
    typename cache_t::iterator _M_tail;
};
} // namespace phi
