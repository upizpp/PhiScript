#pragma once
#include "data_type.hpp"
#include "reference.hpp"
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#define BINARY_OPERATOR(op, fname)                                         \
    const Variant fname(const Variant &) const;                            \
    const Variant operator op(const Variant &v) const { return fname(v); } \
    template <typename T>                                                  \
    const Variant operator op(const T &v) const { return this->operator op(Variant(v)); }

namespace phi
{
    class Object;
    class Function;
    class Variant;

    template <typename T>
    class Singleton;

    struct VariantHash
    {
        size_t operator()(const Variant &value) const;
    };

    struct VariantEqual
    {
        size_t operator()(const Ref<Variant> &a, const Ref<Variant> &b) const;
    };

    typedef std::vector<Ref<Variant>> array;
    typedef std::vector<const Ref<Variant>> const_array;
    typedef std::list<Ref<Variant>> list;
    typedef std::unordered_map<Variant, Ref<Variant>, VariantHash, VariantEqual> dictionary;
    typedef std::vector<Variant> real_array;
    typedef std::list<Variant> real_list;
    typedef std::unordered_map<Variant, Variant, VariantHash, VariantEqual> real_dictionary;
    using std::string;

    enum Flag : uint8_t
    {
        FLAG_NONE,
        FLAG_MODULE,
        FLAG_METHOD,
        FLAG_OPCODE_ARGS,
        FLAG_VARIABLE_ADDRESS
    };

    class Variant
    {
    public:
        static Ref<Variant> Null;
        enum Type : uint8_t
        {
            NIL,
            INT,
            REAL,
            BOOL,
            STRING,
            ARRAY,
            LIST,
            DICTIONARY,
            FUNCTION,
            OBJECT,
            ANY,
            MAX
        };

    private:
        void *m_data = nullptr;
        bool m_is_borrowed = false;
        Flag m_flag = FLAG_NONE;
        Type m_type = Type::NIL;

        integer _hash() const;

    public:
        Variant();
        Variant(integer);
        Variant(real);
        Variant(bool);
        Variant(const char *);
        Variant(const string &);
        Variant(const array &);
        Variant(const list &);
        Variant(const dictionary &);
        Variant(const Object &);
        Variant(Object &&);
        Variant(Object *);
        Variant(const Function &);
        Variant(const Variant &);
        Variant(Variant &&);
        Variant(void*, Flag);
        explicit Variant(Type);
        ~Variant();

        Variant &operator=(integer);
        Variant &operator=(real);
        Variant &operator=(bool);
        Variant &operator=(const string &);
        Variant &operator=(const array &);
        Variant &operator=(const list &);
        Variant &operator=(const dictionary &);
        Variant &operator=(const Object &);
        Variant &operator=(const Function &);
        Variant &operator=(const Variant &);
        Variant &operator=(Variant &&);

        bool operator<(integer) const;
        bool operator<(real) const;
        bool operator<(bool) const;
        bool operator<(const char *) const;
        bool operator<(const string &) const;
        bool operator<(const Object &) const;
        bool operator<(const Variant &) const;

        bool operator>(integer) const;
        bool operator>(real) const;
        bool operator>(bool) const;
        bool operator>(const char *) const;
        bool operator>(const string &) const;
        bool operator>(const Object &) const;
        bool operator>(const Variant &) const;

        bool operator<=(integer) const;
        bool operator<=(real) const;
        bool operator<=(bool) const;
        bool operator<=(const char *) const;
        bool operator<=(const string &) const;
        bool operator<=(const Object &) const;
        bool operator<=(const Variant &) const;

        bool operator>=(integer) const;
        bool operator>=(real) const;
        bool operator>=(bool) const;
        bool operator>=(const char *) const;
        bool operator>=(const string &) const;
        bool operator>=(const Object &) const;
        bool operator>=(const Variant &) const;

        bool operator==(integer) const;
        bool operator==(real) const;
        bool operator==(bool) const;
        bool operator==(const char *) const;
        bool operator==(const string &) const;
        bool operator==(const Object &) const;
        bool operator==(const Variant &) const;

        bool operator!=(integer) const;
        bool operator!=(real) const;
        bool operator!=(bool) const;
        bool operator!=(const char *) const;
        bool operator!=(const string &) const;
        bool operator!=(const Object &) const;
        bool operator!=(const Variant &) const;

        BINARY_OPERATOR(+, add)
        BINARY_OPERATOR(-, sub)
        BINARY_OPERATOR(*, mul)
        BINARY_OPERATOR(/, div)
        BINARY_OPERATOR(&, band)
        BINARY_OPERATOR(|, bor)
        BINARY_OPERATOR(^, bxor)
        BINARY_OPERATOR(%, mod)
        integer left_shift(integer) const;
        integer operator<<(integer bits) const { return left_shift(bits); }
        integer right_shift(integer) const;
        integer operator>>(integer bits) const { return right_shift(bits); }
        Variant negative() const;
        Variant operator-() const { return negative(); }
        Variant reverse() const;
        Variant operator~() const { return reverse(); }
        integer power(real pow) const;
        bool land(const Variant &v) const { return bool(*this) && bool(v); }
        bool lor(const Variant &v) const { return bool(*this) || bool(v); }
        bool lxor(const Variant &v) const { return bool(*this) ^ bool(v); }
        bool lnot() const { return !bool(*this); }
        Variant copy() const;
        Variant copyDeep() const;

        operator integer() const;
        operator real() const;
        operator bool() const;
        operator string() const;
        operator array() const;
        operator list() const;
        operator dictionary() const;
        operator Object() const;
        operator Function() const;

        template <typename T>
        const T *get() const;
        template <typename T>
        T *get();

        integer *getInteger();
        real *getReal();
        bool *getBool();
        string *getString();
        array *getArray();
        list *getList();
        dictionary *getDictionary();
        Function *getFunction();
        Object *getObject();
        void *getAny();

        const integer *getInteger() const;
        const real *getReal() const;
        const bool *getBool() const;
        const string *getString() const;
        const array *getArray() const;
        const list *getList() const;
        const dictionary *getDictionary() const;
        const Function *getFunction() const;
        const Object *getObject() const;
        const void *getAny() const;

        Type type() const { return m_type; };
        const void *data() const { return m_data; }

        void changeType(Type);
        void freeMemory();

        void setFlag(Flag f) { m_flag = f; }
        Flag flag() const { return m_flag; }

        void makeConstant() const;
        bool isConstant() const;

        // for any
        void setData(void *data) { m_data = data; }

    private:
        list::iterator getIterator(integer);
        list::const_iterator getIterator(integer) const;

    public:
        // array
        void pushBack(const Ref<Variant> &);
        void pushFront(const Ref<Variant> &);
        void insert(integer, const Ref<Variant> &);
        void remove(integer);
        void remove(integer, integer);

        // dictionary
        Variant &getProperty(const Variant &key);
        const Variant &getProperty(const Variant &key) const;
        void setProperty(const Variant &, const Ref<Variant> &);
        void removeProperty(const Variant &);
        bool hasProperty(const Variant &) const;

        Variant call(array &args);
        Variant operator()(array &args) { return call(args); }

        Ref<Variant> access(const array &args, const Ref<Variant>& = nullptr);
        uinteger size() const;

        const Variant &operator[](const Variant &key) const { return getProperty(key); }
        Variant &operator[](const Variant &key) { return getProperty(key); }

        string toString() const;
        integer hash() const
        {
            integer result = _hash() ^ (m_type << 8);
            return result != -1 ? result : -0xffffffff;
        }

        static string stringifyType(Type);
    };

    inline std::ostream &operator<<(std::ostream &os, const Variant &value) { return os << value.toString(); }

    template <typename T>
    inline const T *Variant::get() const
    {
        return reinterpret_cast<const T *>(m_data);
    }

    template <typename T>
    inline T *Variant::get()
    {
        return reinterpret_cast<T *>(m_data);
    }

    Variant makeArray(const real_array &);
    Variant makeList(const real_list &);
    Variant makeDict(const real_dictionary &);
} // namespace phi
