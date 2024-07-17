#include <compile/token_generator.hpp>
#include <map>

namespace phi {
struct Preprocessor : TokenGenerator {
    Preprocessor(TokenGenerator &generator) : _M_generator(generator) {}

    virtual unique_ptr<token::Token> next();
    virtual bool eof() { return _M_generator.eof() && _M_cache == nullptr; }
    virtual void reset() { _M_generator.reset(); }

  private:
    token::Token *_M_cache = nullptr;
    uint64_t _M_cache_line = -1;
    TokenGenerator &_M_generator;
    uint64_t _M_struct_keywords = 0;
    std::map<uint16_t, uint64_t> _M_unclosed_pair;
};

} // namespace phi
