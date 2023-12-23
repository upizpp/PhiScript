#include "phi_entry.hpp"
#include "exception.hpp"
#include "logger.hpp"
#include "singleton.hpp"
#include <exception>
#include <iostream>
#include <list>
#include <sstream>
#include <windows.h>
#include <phi/phi_interact.hpp>
#include <phi/phi_args.hpp>

#define LOG_LEVEL DEBUG
#undef ERROR

namespace phi
{
    static std::list<Canceler> _Cancelers;
    static compiler::Position _CurrentPosition;

    void PhiRegisterCanceller(const Canceler& canc)
    {
        _Cancelers.push_front(canc);
    }

    void PhiSetPosition(const compiler::Position &pos)
    {
        _CurrentPosition = pos;
    }

    const compiler::Position &PhiGetPosition()
    {
        return _CurrentPosition;
    }

    void PhiTerminate()
    {
        using std::endl;
        std::ostringstream os;

        try
        {
            throw;
        }
        catch (const phi::Exception &exception)
        {
            os << "Program threw an phi::exception:" << endl;
            os << exception.getClassName() << ":"
               << "\n"
               << exception.what() << endl;
            os << "Position: " << PhiGetPosition() << endl;
        }
        catch (const std::exception &exception)
        {
            os << "Program threw an std::exception:" << endl;
            os << exception.what() << endl;
            os << "Position: " << PhiGetPosition() << endl;
        }

        std::cout << os.str();
        LERROR(os.str().c_str());

        phi::PhiExit();
        std::abort();
    }

    void PhiInitialize()
    {
        SetConsoleOutputCP(CP_UTF8);
        std::set_terminate(PhiTerminate);

        LOGGER_INSTANCE->setLevel(Logger::Level::LOG_LEVEL);
        LINFO("Logging started...");
    }

    int PhiEntry(int &argc, char **&args)
    {
        if (argc == 1)
            interact(args[0]);
        else
            parseArgs(argc, args);
        PhiExit();
        return 0;
    }

    void PhiExit()
    {
        for (auto &&canceler : _Cancelers)
        {
            canceler();
        }
    }

    void exit(int code)
    {
        PhiExit();
        std::exit(code);
    }
} // namespace phi
