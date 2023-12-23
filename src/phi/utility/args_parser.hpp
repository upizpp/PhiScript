#pragma once
#include <map>
#include <phi/data_type.hpp>
#include <vector>

#undef OPTIONAL

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

        using Options = std::map<std::string, Option>;

    private:
        std::string m_name;
        Options m_options;
        std::map<std::string, std::string> m_values;
        std::vector<std::string> m_arguments;

    public:
        ArgsParser() = default;
        explicit ArgsParser(const std::string &name, const Options &options = {}) : m_name(name), m_options(options) {}
        explicit ArgsParser(const Options &options) : m_options(options) {}

        void addOption(const std::string &name, Importance importance = Importance::OPTIONAL, Type type = Type::SHORT);
        void addOption(const std::string &name, Type type);
        bool hasOption(const std::string &name) const;

        void show() const;
        void parse(int argc, char *args[]);

        const std::vector<std::string> &getArguments() const { return m_arguments; }
        const std::string &getValue(const std::string &key) { return m_values[key]; }
        std::string &operator[](const std::string &key) { return m_values[key]; }

        void name(const std::string &name) { m_name = name; }
        std::string &name() { return m_name; }
        const std::string &name() const { return m_name; }
    };
} // namespace upi
