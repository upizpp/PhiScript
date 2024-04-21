#pragma once

#include <array>
#include <phi/exception.hpp>
#include <token.hpp>
#include <type_traits>

namespace phi {
    struct TokenGenerator {
        virtual ~TokenGenerator() {}
        virtual Ref<token::Token> getNextToken() = 0;
        virtual bool eof() = 0;
    };

    struct TokenIterator {
        using iterator_category = std::input_iterator_tag;
        using value_type = Ref<token::Token>;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

        TokenIterator operator++() {
            _M_token = _M_generator->getNextToken();
            return *this;
        }
        TokenIterator operator++(int) {
            TokenIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        value_type operator*() { return _M_token; }
        value_type operator*() const { return _M_token; }
        value_type operator->() { return _M_token; }
        value_type operator->() const { return _M_token; }

        virtual ~TokenIterator() {}

      private:
        value_type _M_token;
        TokenGenerator *_M_generator;
    };

    template <size_t Cache> struct CachedTokenIterator : TokenGenerator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Ref<token::Token>;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

        CachedTokenIterator<Cache> operator++() {
            _M_token = _M_generator->getNextToken();
            if (_M_tail < Cache) {
                _M_cache[_M_tail++] = _M_token;
            } else {
                std::copy(_M_cache.begin() + 1, _M_cache.end(),
                          _M_cache.begin());
                _M_cache[Cache - 1] = _M_token;
            }
            return *this;
        }

        CachedTokenIterator<Cache> operator--() {
            if (_M_tail == 0) {
                throw RuntimeException("CachedTokenIterator: Cache runs out.");
            }
            _M_token = _M_cache[--_M_tail];
        }
        CachedTokenIterator<Cache> operator--(int) {
            CachedTokenIterator<Cache> tmp = *this;
            --(*this);
            return tmp;
        } 

      private:
        std::array<value_type> _M_cache;
        size_t _M_tail = 0;
    };

} // namespace phi
