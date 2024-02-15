#pragma once
#include <phi/deserialize.hpp>
#include <phi/serialize.hpp>

namespace phi {
    class Pout {
      public:
        static const std::array<byte, 8> query;

        static bool isPout(const vector<byte> &bytes);
        static bool isPout(const string &path);
        static Function load(const vector<byte> &bytes);
        static Function load(const string &path);
        static vector<byte> dump(const Function &func);
    };
} // namespace phi
