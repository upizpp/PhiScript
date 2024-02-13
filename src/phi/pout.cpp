#include "pout.hpp"
#include <array>
#include <fstream>

namespace phi
{
    const std::array<byte, 8> Pout::query{
        0x42,
        0x2a,
        0x4b,
        0xf0,
        0x7a,
        0x37,
        0x37,
        0x37,
    };

    bool Pout::isPout(const vector<byte> &bytes)
    {
        if (bytes.size() < 8)
            return false;
        for (uinteger i = 0; i < query.size(); ++i)
            if (bytes[i] != query[i])
                return false;
        return true;
    }
    bool Pout::isPout(const string &path)
    {
        std::ifstream is(path, std::ios::binary);
        byte *bytes = new byte[8];
        is.read((char*)bytes, 8);
        is.close();
        for (uinteger i = 0; i < query.size(); ++i)
            if (bytes[i] != query[i])
            {
                delete bytes;
                return false;
            }
        delete bytes;
        return true;
    }

    Function Pout::load(const string &path)
    {
        std::ifstream is(path, std::ios::binary);
        is.seekg(0, std::ios::end);
        std::streampos filesize = is.tellg();
        is.seekg(0, std::ios::beg);
        byte* bytes = new byte[filesize];
        is.read((char*)bytes, filesize);
        Function result = load(vector<byte>{bytes, bytes + filesize});
        delete bytes;
        return result;
    }
    Function Pout::load(const vector<byte> &bytes)
    {
        auto it = bytes.cbegin() + query.size();
        return Deserialize<Function>()(it);
    }
    vector<byte> Pout::dump(const Function &func)
    {
        auto bytes = Serialize<Function>()(func);
        bytes.insert(bytes.begin(), query.begin(), query.end());
        return {bytes.begin(), bytes.end()};
    }
} // namespace phi