#include <phi/typedef.hpp>

namespace phi
{
    class Object;
    class Function;
    class Variant;

    using array = vector<Ref<Variant>>;
    using dict = map<Ref<Variant>, Ref<Variant>>;

    class Variant
    {
    public:
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

        operator integer() {return _M_int;}
        operator real() {return _M_real;}
        operator bool() {return _M_bool;}
        operator string() {return *_M_string_P;}
        operator array() {return *_M_array_P;}
        operator dict() {return *_M_dict_P;}
        operator Object();
        operator Function();

        void free();
    };
} // namespace phi
