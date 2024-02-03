#pragma once
#include <phi/runtime/opcode.hpp>
#include <phi/variant.hpp>
#include <stack>

namespace phi
{
    class State
    {
    public:
        using gcp_t = unordered_map<arg_t, Ref<Variant>>;

    private:
        Ref<gcp_t> _M_GCP; // Global Constant Pool
        vector<OPCode> _M_codes;
        unordered_map<arg_t, arg_t> _M_labels;

    public:
        inline void setGCP(Ref<gcp_t> gcp) { _M_GCP = gcp; }
        inline Ref<Variant> lookup(arg_t id) const { return _M_GCP->find(id)->second; }
        inline const vector<OPCode> &getCodes() const { return _M_codes; }
        inline arg_t label(arg_t label) const { return _M_labels.at(label); }
    };
} // namespace phi
