#include "global.hpp"

namespace phi
{
    static map<string, Ref<Variant>> phiGlobal;

    map<string, Ref<Variant>> &getGlobals()
    {
        return phiGlobal;
    }
    void setGlobal(const string &name, Ref<Variant> value)
    {
        phiGlobal[name] = value;
    }
    Ref<Variant> getGlobal(const string &name)
    {
        return phiGlobal[name];
    }
    bool hasGlobal(const string &name)
    {
        return phiGlobal.find(name) != phiGlobal.end();
    }
} // namespace phi