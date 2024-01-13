#pragma once
#include <ostream>
#include <phi/typedef.hpp>

namespace phi
{
    class Object;
    class Function;
    class Variant;

    struct VariantHash;
    struct VariantEqual;

    using array = vector<Ref<Variant>>;
    using dict = unordered_map<Ref<Variant>, Ref<Variant>, VariantHash, VariantEqual>;

    class Variant
    {
    public:
        static uinteger hashSeed;

        enum class Type : char
        {
            NIL,
            INT,
            REAL,
            BOOL,
            STRING,
            ARRAY,
            DICTIONARY,
            OBJECT,
            FUNCTION,
            MAX
        };

    private:
        union
        {
            integer _M_int;
            real _M_real;
            bool _M_bool;
            string *_M_string_P;
            array *_M_array_P;
            dict *_M_dict_P;
            Object *_M_obj_P;
            Function *_M_func_P;
        };
        Type _M_type;

    public:
        Variant();
        explicit Variant(const int &);
        explicit Variant(const double &);
        explicit Variant(const integer &);
        explicit Variant(const real &);
        explicit Variant(const bool &);
        explicit Variant(const char *);
        explicit Variant(const string &);
        explicit Variant(const array &);
        explicit Variant(const dict &);
        explicit Variant(const Object &);
        explicit Variant(const Function &);
        Variant(const Variant &);
        Variant(Variant &&);
        ~Variant() { free(); }

        Variant &operator=(const int &);
        Variant &operator=(const double &);
        Variant &operator=(const integer &);
        Variant &operator=(const real &);
        Variant &operator=(const bool &);
        Variant &operator=(const char *);
        Variant &operator=(const string &);
        Variant &operator=(const array &);
        Variant &operator=(const dict &);
        Variant &operator=(const Object &);
        Variant &operator=(const Function &);
        Variant &operator=(const Variant &);
        Variant &operator=(Variant &&);

        operator integer() const;
        operator real() const;
        operator bool() const;
        operator string() const;
        operator array() const;
        operator dict() const;
        operator Object() const;
        operator Function() const;

        void free();
        Type type() const { return _M_type; }
        static string stringifyType(Type);

        string toString() const;
        uinteger hash() const;

#define COMPARE_DECL(op)                      \
    bool operator op(const int &) const;      \
    bool operator op(const double &) const;   \
    bool operator op(const integer &) const;  \
    bool operator op(const real &) const;     \
    bool operator op(const bool &) const;     \
    bool operator op(const char *) const;     \
    bool operator op(const string &) const;   \
    bool operator op(const array &) const;    \
    bool operator op(const dict &) const;     \
    bool operator op(const Object &) const;   \
    bool operator op(const Function &) const; \
    bool operator op(const Variant &) const;

        COMPARE_DECL(==)
        COMPARE_DECL(!=)
        COMPARE_DECL(>)
        COMPARE_DECL(>=)
        COMPARE_DECL(<)
        COMPARE_DECL(<=)
    };

    inline std::ostream &operator<<(std::ostream &os, const Variant &value)
    {
        os << value.toString();
        return os;
    }

    struct VariantHash
    {
        size_t operator()(const Ref<Variant> &value)
        {
            return value->hash();
        }
    };

    struct VariantEqual
    {
        size_t operator()(const Ref<Variant> &lhs, const Ref<Variant> &rhs)
        {
            return *lhs == *rhs;
        }
    };

} // namespace phi
