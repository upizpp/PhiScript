#pragma once
#include <ostream>
#include <phi/typedef.hpp>

namespace phi
{
    class Object;
    class Function;
    class Variant;

    struct VariantHash
    {
        size_t operator()(const Ref<Variant> &value) const;
    };

    struct VariantEqual
    {
        size_t operator()(const Ref<Variant> &lhs, const Ref<Variant> &rhs) const;
    };

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
        static Ref<Variant> Null;

        Variant();
        Variant(const int &);
        Variant(const double &);
        Variant(const integer &);
        Variant(const real &);
        Variant(const bool &);
        Variant(const char *);
        Variant(const string &);
        Variant(const array &);
        Variant(Owner<array>&&);
        Variant(const dict &);
        Variant(Owner<dict>&&);
        Variant(const Object &);
        Variant(const Function &);
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

        operator int() const;
        operator double() const;
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

        template <typename T>
        T *getPtr();

        template <typename T>
        T &seeAs()
        {
            return *getPtr<T>();
        }
        
        template <typename T>
        const T &seeAs() const
        {
            return *getPtr<T>();
        }


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

#define CALCULATE_DECL(fname, op)                                 \
    int operator op(const int &) const;                           \
    double operator op(const double &) const;                     \
    integer operator op(const integer &) const;                   \
    real operator op(const real &) const;                         \
    bool operator op(const bool &) const;                         \
    string operator op(const char *) const;                       \
    string operator op(const string &) const;                     \
    Variant operator op(const Object &) const;                    \
    Variant operator op(const Variant &) const;                   \
    Variant fname(const Variant &) const;                         \
    friend int operator op(const int &, const Variant &);         \
    friend double operator op(const double &, const Variant &);   \
    friend integer operator op(const integer &, const Variant &); \
    friend real operator op(const real &, const Variant &);       \
    friend bool operator op(const bool &, const Variant &);       \
    friend string operator op(const char *, const Variant &);     \
    friend string operator op(const string &, const Variant &);
#define CALCULATE_NUM_DECL(fname, op)           \
    int operator op(const int &) const;         \
    double operator op(const double &) const;   \
    integer operator op(const integer &) const; \
    real operator op(const real &) const;       \
    bool operator op(const bool &) const;       \
    Variant operator op(const Object &) const;  \
    Variant operator op(const Variant &) const; \
    Variant fname(const Variant &) const;
#define CALCULATE_INT_DECL(fname, op)           \
    int operator op(const int &) const;         \
    integer operator op(const integer &) const; \
    bool operator op(const bool &) const;       \
    Variant operator op(const Object &) const;  \
    Variant operator op(const Variant &) const; \
    Variant fname(const Variant &) const;
        CALCULATE_DECL(add, +)
        CALCULATE_NUM_DECL(sub, -)
        CALCULATE_NUM_DECL(mul, *)
        CALCULATE_NUM_DECL(div, /)
        CALCULATE_INT_DECL(mod, %)
        CALCULATE_INT_DECL(band, &)
        CALCULATE_INT_DECL(bor, |)
        CALCULATE_INT_DECL(bxor, ^)
        CALCULATE_INT_DECL(land, &&)
        CALCULATE_INT_DECL(lor, ||)
        CALCULATE_INT_DECL(lshift, <<)
        CALCULATE_INT_DECL(rshift, >>)

        real power(real) const;

        Variant operator-() const;
        Variant operator~() const;
        Variant operator!() const;
        Variant operator++();
        Variant operator--();

        Variant copy() const;
        Variant deepCopy() const;

        void convert(Type);
        inline Variant convertTo(Type type) const
        {
            Variant tmp = Variant{*this};
            tmp.convert(type);
            return tmp;
        }
        bool equals(const Variant &);

        bool isNull() const
        {
            return type() == Type::NIL;
        }
        void checkThis();

        Ref<Variant> call(const array& args);
        Ref<Variant>& access(const array& args);

        static bool isConvertible(Type, Type);
    };

    inline std::ostream &operator<<(std::ostream &os, const Variant &value)
    {
        os << value.toString();
        return os;
    }

    template <typename T>
    struct VariantType;
    template <>
    struct VariantType<int>
    {
        static const Variant::Type value = Variant::Type::INT;
    };
    template <>
    struct VariantType<integer>
    {
        static const Variant::Type value = Variant::Type::INT;
    };
    template <>
    struct VariantType<uinteger>
    {
        static const Variant::Type value = Variant::Type::INT;
    };
    template <>
    struct VariantType<float>
    {
        static const Variant::Type value = Variant::Type::REAL;
    };
    template <>
    struct VariantType<real>
    {
        static const Variant::Type value = Variant::Type::REAL;
    };
    template <>
    struct VariantType<bool>
    {
        static const Variant::Type value = Variant::Type::BOOL;
    };
    template <>
    struct VariantType<string>
    {
        static const Variant::Type value = Variant::Type::STRING;
    };
    template <>
    struct VariantType<char *>
    {
        static const Variant::Type value = Variant::Type::STRING;
    };
    template <>
    struct VariantType<array>
    {
        static const Variant::Type value = Variant::Type::ARRAY;
    };
    template <typename T>
    struct VariantType<vector<T>>
    {
        static const Variant::Type value = Variant::Type::REAL;
    };
    template <>
    struct VariantType<dict>
    {
        static const Variant::Type value = Variant::Type::DICTIONARY;
    };
    template <>
    struct VariantType<Object>
    {
        static const Variant::Type value = Variant::Type::OBJECT;
    };
    template <typename T>
    struct VariantType : std::enable_if<std::is_base_of<Object, T>::value>::type
    {
        static const Variant::Type value = Variant::Type::OBJECT;
    };
    template <>
    struct VariantType<Function>
    {
        static const Variant::Type value = Variant::Type::FUNCTION;
    };

    template <typename T>
    struct TypeOf
    {
        static const Variant::Type value = VariantType<typename std::remove_cv<typename std::remove_reference<typename std::remove_pointer<T>::type>::type>::type>::value;
    };

    template <typename T>
    T *Variant::getPtr()
    {
        if (type() != TypeOf<T>::value)
            return nullptr;
        switch (type())
        {
        case Type::NIL:
            return nullptr;
        case Type::INT:
            return (T*)&_M_int;
        case Type::REAL:
            return (T*)&_M_real;
        case Type::BOOL:
            return (T*)&_M_bool;
        case Type::STRING:
            return (T*)_M_string_P;
        case Type::DICTIONARY:
            return (T*)_M_dict_P;
        case Type::ARRAY:
            return (T*)_M_array_P;
        case Type::OBJECT:
            return (T*)_M_obj_P;
        case Type::FUNCTION:
            return (T*)_M_func_P;
        default:
            return nullptr;
        }
    }
} // namespace phi
