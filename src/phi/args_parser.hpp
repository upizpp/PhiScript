#pragma once
#include <phi/typedef.hpp>

namespace phi
{
    class ArgsParser
    {
    public:
        enum class Importance
        {
            OPTIONAL,
            REQUIRED
        };
        enum class Type
        {
            SHORT,
            LONG
        };

        struct Option
        {
            Importance importance;
            Type type;

            Option() {}
            explicit Option(Importance importance, Type type = Type::SHORT)
            {
                this->importance = importance;
                this->type = type;
            }
            explicit Option(Type type, Importance importance = Importance::OPTIONAL)
            {
                this->importance = importance;
                this->type = type;
            }
        };

        using Options = map<string, Option>;

    private:
        string _M_name;
        Options _M_options;
        map<string, string> _M_values;
        vector<string> _M_arguments;

    public:
        ArgsParser() = default;
        explicit ArgsParser(const string &name, const Options &options = {}) : _M_name(name), _M_options(options) {}
        explicit ArgsParser(const Options &options) : _M_options(options) {}

        void addOption(const string &name, Importance importance = Importance::OPTIONAL, Type type = Type::SHORT);
        void addOption(const string &name, Type type);
        bool hasOption(const string &name) const;

        bool hasNoExtraArgs() const { return _M_values.empty(); }

        void show() const;
        void parse(int argc, char *args[]);

        bool has(const string &name) const { return _M_values.find(name) != _M_values.end(); }
        const string &get(const string &name) { return _M_values[name]; }
        const vector<string> &getArgs() const { return _M_arguments; };

        void name(const string &name) { _M_name = name; }
        string &name() { return _M_name; }
        const string &name() const { return _M_name; }
    };
} // namespace upi
