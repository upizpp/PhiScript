#pragma once
#include <phi/runtime/opcode.hpp>
#include <phi/variant.hpp>
#include <stack>

namespace phi
{
    class OPCodePacker
    {
    private:
        uinteger _M_index;
        Borrower<CodeSeq> _M_container;

    public:
        OPCodePacker() = default;
        OPCodePacker(const OPCodePacker&) = default;
        OPCodePacker(Borrower<CodeSeq> container, uinteger index) : _M_index(index), _M_container(container) {}

        OPCode *operator->() { return &(*_M_container)[_M_index]; }
        const OPCode *operator->() const { return &(*_M_container)[_M_index]; }
        OPCode &operator*() { return (*_M_container)[_M_index]; }
        const OPCode &operator*() const { return (*_M_container)[_M_index]; }
    };
    class State
    {
    public:
        using gcp_t = unordered_map<arg_t, Ref<Variant>>;

    private:
        Ref<gcp_t> _M_GCP; // Global Constant Pool
        CodeSeq _M_codes;
        unordered_map<arg_t, arg_t> _M_labels;

        static Borrower<gcp_t> _M_globalPool;

    public:
        inline static Ref<Variant> lookupGlobal(arg_t id)
        {
            return _M_globalPool ? _M_globalPool->find(id)->second : nullptr;
        }

    public:
        inline void setGCP(Ref<gcp_t> gcp)
        {
            _M_GCP = gcp;
            _M_globalPool = gcp;
        }
        inline void emitLabel(arg_t label) { emitLabel(label, static_cast<phi::arg_t>(_M_codes.size())); }
        inline void emitLabel(arg_t label, arg_t position) { _M_labels.insert({label, position}); }
        inline OPCodePacker push(const OPCode &code)
        {
            _M_codes.push_back(code);
            return OPCodePacker{&_M_codes, _M_codes.size() - 1};
        }
        inline Ref<Variant> lookup(arg_t id) const { return _M_GCP->find(id)->second; }
        inline CodeSeq &getCodes() { return _M_codes; }
        inline const CodeSeq &getCodes() const { return _M_codes; }
        inline OPCode &top() { return _M_codes.back(); }
        inline OPCode& getCode(uinteger index) {return _M_codes[index] ;}
        inline const OPCode& getCode(uinteger index) const {return _M_codes[index] ;}
        inline const OPCode &top() const { return _M_codes.back(); }
        inline arg_t label(arg_t label) const { return _M_labels.at(label); }
        inline const unordered_map<arg_t, arg_t> &labels() const { return _M_labels; }

        ~State()
        {
            if (_M_GCP.count() == 1 && _M_globalPool == _M_GCP)
                _M_globalPool = nullptr;
        }
    };
} // namespace phi
