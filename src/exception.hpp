#pragma once
#include <global.hpp>

#define DEFINE_EXCEPTION(child, parent)                                        \
    struct child : public parent {                                             \
        using parent::parent;                                                  \
        virtual const char *classCame() const override { return #child; }      \
    }

namespace phi {
struct Exception {
    Exception(const string &message) : _M_message(message) {}

    virtual ~Exception() = default;
    virtual const char *classCame() const { return "Exception"; }

  private:
    PROPERTY(string, message);
};

DEFINE_EXCEPTION(SyntaxException, Exception);

struct FileException : public Exception {

    virtual const char *classCame() const override { return "FileException"; }

    FileException(const string &filename)
        : Exception("File not found: " + filename + ".") {}
};

} // namespace phi
