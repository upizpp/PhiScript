#pragma once
#include "../reference.hpp"
#include <iostream>
#include <phi/compiler/token.hpp>
#include <string>

namespace phi
{
    class Variant;

    namespace runtime
    {
        class OPCode
        {
        public:
            enum class Command
            {
                DATA,            // Data
                ADD,             // Addition operation                  +
                SUB,             // Subtraction operation               -
                MUL,             // Multiplication operation            *
                DIV,             // Division operation                  /
                MOD,             // Modulo operation                    %
                NEG,             // Negation operation                  -
                POWER,           // Power operation                     **
                BNOT,            // Bitwise NOT operation               ~
                BOR,             // Bitwise OR operation                 |
                BAND,            // Bitwise AND operation                &
                BXOR,            // Bitwise XOR operation                ^
                COPY,            // Copy operation                       @
                COPYDEEP,        // Deep Copy operation                  @@
                LSHIFT,          // Left shift operation                <<
                RSHIFT,          // Right shift operation               >>
                EQ,              // Equality operation                  ==
                NE,              // Inequality operation                !=
                LT,              // Less than operation                  <
                LE,              // Less than or equal operation         <=
                GT,              // Greater than operation               >
                GE,              // Greater than or equal operation      >=
                LAND,            // Logical AND operation                &&
                LOR,             // Logical OR operation                 ||
                LXOR,            // Logical XOR operation                ^^
                LNOT,            // Logical NOR operation                !
                LOAD,            // Load(Load a allocated variable) operation
                ALLOCATE,        // Allocate(Create a new variable) operation
                DELETE,          // Delete operation
                ASSIGN,          // Assignment operation                 =
                ADDASN,          // Addition assignment operation        +=
                SUBASN,          // Subtraction assignment operation     -=
                MULASN,          // Multiplication assignment operation  *=
                DIVASN,          // Division assignment operation        /=
                MODASN,          // Modulo assignment operation          %=
                ANDASN,          // Bitwise AND assignment operation     &=
                ORASN,           // Bitwise OR assignment operation       |=
                XORASN,          // Bitwise XOR assignment operation      ^=
                LSASN,           // Left shift assignment operation      <<=
                RSASN,           // Right shift assignment operation     >>=
                CALL,            // Function call operation
                ACCESS,          // Access operation                     .
                MAKE_ARRAY,      // Make an array
                MAKE_DICTIONARY, // Make a dictionary
                ARGS,             // As an argument mark
                FN_LOCATE ,       // Locate the block of a function
            };

        private:
            Command m_command = Command::DATA;
            Reference<Variant> m_data;
            bool m_in_constant = false;
            compiler::Position m_position;

        public:
            OPCode() = default;
            OPCode(Command);
            OPCode(const Reference<Variant> &);
            OPCode(const OPCode &);

            OPCode &operator=(const OPCode &);

            Command &command() { return m_command; }
            const Command &command() const { return m_command; }
            Reference<Variant> &data() { return m_data; }
            const Reference<Variant> &data() const { return m_data; }

            compiler::Position &position() { return m_position; }
            const compiler::Position &position() const { return m_position; }
            OPCode &position(const compiler::Position &position)
            {
                m_position = position;
                return *this;
            }

            Reference<Variant> &operator->() { return m_data; }
            const Reference<Variant> &operator->() const { return m_data; }

            std::string stringify() const;

            static std::string cmdStr(const Command &);
        };

        inline std::ostream &operator<<(std::ostream &os, const OPCode &code)
        {
            return os << code.stringify();
        }
    } // namespace runtime
} // namespace phi
