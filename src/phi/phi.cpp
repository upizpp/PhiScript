#define BUILD_DLL
#include "phi.h"
#include <phi/compiler/compiler.hpp>
#include <phi/exception.hpp>
#include <phi/runtime/follower.hpp>
#include <version>
#include <cstring>

using namespace phi;

#define CHECK_PHI       \
    if (phi == nullptr) \
    NO_PHI_ERROR
#define NO_PHI_ERROR \
    return new PhiError { .msg = to_c_str("") }

struct Phi
{
    Environment env;
};

char* to_c_str(const string& str)
{
    char* res = new char[str.length() + 1];
    std::strcpy(res, str.c_str());
    return res;
}

const char *phiVersion()
{
    return PHI_VERSION;
}

Phi *phiCreate()
{
    return new Phi;
}

void phiDestroy(Phi *phi)
{
    delete phi;
}

PhiError *phiDoString(Phi *phi, const char *chunk)
{
    CHECK_PHI;
    try
    {
        Compiler compiler{new StringScanner{chunk}};
        phi->env = std::move(compiler.load().getMethod().call(phi->env, {}).second);
    }
    catch (const Exception &e)
    {
        return new PhiError{.msg = to_c_str(e.what()), .type = to_c_str(e.className())};
    }
    return nullptr;
}

PhiError *phiDoStringWithName(Phi *phi, const char *chunk, const char *chunk_name)
{
    CHECK_PHI;
    try
    {
        Compiler compiler{new StringScanner{chunk, chunk_name}};
        phi->env = std::move(compiler.load().getMethod().call(phi->env, {}).second);
    }
    catch (const Exception &e)
    {
        return new PhiError{.msg = to_c_str(e.what()), .type = to_c_str(e.className())};
    }
    return nullptr;
}

PhiError *phiDoFile(Phi *phi, const char *filename)
{
    CHECK_PHI;
    try
    {
        Compiler compiler{new FileScanner{filename}};
        phi->env = std::move(compiler.load().getMethod().call(phi->env, {}).second);
    }
    catch (const Exception &e)
    {
        return new PhiError{.msg = to_c_str(e.what()), .type = to_c_str(e.className())};
    }
    return nullptr;
}
