#pragma once
#include <string>
#include "data_type.hpp"
#include "runtime/op_code.hpp"

namespace phi
{
    class Variant;

    namespace compiler
    {
        runtime::OPCode::Command getBinaryCommand(const std::string&);
        runtime::OPCode::Command getUnaryCommand(const std::string&);
        bool isUnary(const std::string&);
        bool isBinary(const std::string&);
        bool isKeyword(const std::string&);
        bool isStructuredKeyword(const std::string&);
        bool isOperator(const std::string&);
        bool isSymbol(char);
        bool isValidBase(char);
        bool isValidIdentifierChar(char);
        bool isValidIdentifier(const std::string&);
        int8_t getBase(char);
        bool isValidCharInBase(char, integer);
        Variant parseNumber(std::string);
    } // namespace compiler
} // namespace phi
