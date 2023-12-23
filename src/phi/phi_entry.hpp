#pragma once
#include <list>
#include <functional>
#include "compiler/token.hpp"

namespace phi
{
    typedef std::function<void()> Canceler;
    typedef void(*Handler)(void);
    void PhiRegisterCanceller(const Canceler&);

    void PhiTerminate();
    void PhiInitialize();
    int PhiEntry(int &, char **&);

    void PhiSetPosition(const compiler::Position&);
    const compiler::Position& PhiGetPosition();
    
    void PhiExit();

    void exit(int code);
} // namespace phi