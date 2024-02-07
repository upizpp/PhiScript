#pragma once
#include <phi/variant.hpp>

namespace phi
{
    struct RestParameters
    {
    private:
        array _M_data;

    public:
        RestParameters() = default;
        RestParameters(const RestParameters &) = default;
        RestParameters(const array &data) : _M_data{data} {}
        RestParameters(array &&data) : _M_data{std::move(data)} {}

        array::reference operator[](uinteger index)
        {
            return _M_data[index];
        }
        array::const_reference operator[](uinteger index) const
        {
            return _M_data[index];
        }
        uinteger size() const
        {
            return _M_data.size();
        }

        array::iterator begin()
        {
            return _M_data.begin();
        }
        array::const_iterator begin() const
        {
            return _M_data.begin();
        }
        array::iterator end()
        {
            return _M_data.end();
        }
        array::const_iterator end() const
        {
            return _M_data.end();
        }
    };

} // namespace phi
