#pragma once
#include <phi/exception.hpp>
#include <phi/function.hpp>
#include <phi/object.hpp>
#include <phi/typedef.hpp>
#include <phi/variant.hpp>
#include <phi/runtime/state.hpp>

namespace phi
{
    inline static void insertEnd(list<byte> &result, const list<byte> &other)
    {
        result.insert(result.end(), other.begin(), other.end());
    }

    template <typename T>
    struct Serialize
    {
        list<byte> operator()(const T &x) const
        {
            return {reinterpret_cast<const byte *>(&x), reinterpret_cast<const byte *>(&x) + sizeof(T)};
        }
    };

    template <>
    struct Serialize<string>
    {
        list<byte> operator()(const string &x) const
        {
            list<byte> result;
            insertEnd(result, Serialize<uinteger>()(x.length()));
            result.insert(result.end(), x.begin(), x.end());
            return result;
        }
    };

    template <typename T>
    struct Serialize<Ref<T>>
    {
        list<byte> operator()(const Ref<T> &x) const
        {
            return Serialize<T>()(*x);
        }
    };

    template <typename T>
    struct Serialize<vector<T>>
    {
        list<byte> operator()(const vector<T> &x) const
        {
            list<byte> result;
            insertEnd(result, Serialize<uinteger>()(x.size()));
            for (auto &&item : x)
                insertEnd(result, Serialize<T>()(item));
            return result;
        }
    };
    
    template <typename K, typename V, typename C, typename A>
    struct Serialize<map<K, V, C, A>>
    {
        list<byte> operator()(const map<K, V, C, A> &x) const
        {
            list<byte> result;
            insertEnd(result, Serialize<uinteger>()(x.size()));
            for (auto &&item : x)
            {
                insertEnd(result, Serialize<K>()(item.first));
                insertEnd(result, Serialize<V>()(item.second));
            }
            return result;
        }
    };

    template <typename K, typename V, typename H, typename P, typename A>
    struct Serialize<unordered_map<K, V, H, P, A>>
    {
        list<byte> operator()(const unordered_map<K, V, H, P, A> &x) const
        {
            list<byte> result;
            insertEnd(result, Serialize<uinteger>()(x.size()));
            for (auto &&item : x)
            {
                insertEnd(result, Serialize<K>()(item.first));
                insertEnd(result, Serialize<V>()(item.second));
            }
            return result;
        }
    };

    template <>
    struct Serialize<Object>
    {
        list<byte> operator()(const Object &x) const
        {
            list<byte> result;
            insertEnd(result, Serialize<string>()(x.className()));
            insertEnd(result, Serialize<map<string, Ref<Variant>>>()(x.properties()));
            return result;
        }
    };

    template<>
    struct Serialize<State>
    {
        list<byte> operator()(const State &x) const
        {
            list<byte> result;
            insertEnd(result, Serialize<State::gcp_t>()(x.getGCP()));
            insertEnd(result, Serialize<CodeSeq>()(x.getCodes()));
            insertEnd(result, Serialize<unordered_map<arg_t, arg_t>>()(x.labels()));
            insertEnd(result, Serialize<vector<arg_t>>()(x.lines()));
            insertEnd(result, Serialize<Ref<string>>()(x.chunk()));
            return result;
        }
    };
    

    template <>
    struct Serialize<Method>
    {
        list<byte> operator()(const Method &x) const
        {
            list<byte> result;
            insertEnd(result, Serialize<State>()(x.getState()));
            insertEnd(result, Serialize<vector<Ref<string>>>()(x.bound() ? x.binds() : vector<Ref<string>>()));
            return result;
        }
    };

    template <>
    struct Serialize<Function>
    {
        list<byte> operator()(const Function &x) const
        {
            list<byte> result;
            if (x.isBuiltin())
                throw NoImplementException(__FUNCTION__);
            result.push_back((byte)1);
            insertEnd(result, Serialize<Method>()(x.getMethod()));
            insertEnd(result, Serialize<map<uinteger, Ref<Variant>>>()(x.binds()));
            insertEnd(result, Serialize<map<string, Ref<Variant>>>()(x.properties()));
            return result;
        }
    };


#define SERIALIZE_VARIANT(T) insertEnd(result, Serialize<T>()(*x.get<T>()))

    template <>
    struct Serialize<Variant>
    {
        list<byte> operator()(const Variant &x) const
        {
            list<byte> result;
            result.push_back((byte)x.type());
            switch (x.type())
            {
            case Variant::Type::NIL:
            {
                break;
            }
            case Variant::Type::INT:
            {
                SERIALIZE_VARIANT(integer);
                break;
            }
            case Variant::Type::REAL:
            {
                SERIALIZE_VARIANT(real);
                break;
            }
            case Variant::Type::BOOL:
            {
                SERIALIZE_VARIANT(bool);
                break;
            }
            case Variant::Type::STRING:
            {
                SERIALIZE_VARIANT(string);
                break;
            }
            case Variant::Type::ARRAY:
            {
                SERIALIZE_VARIANT(array);
                break;
            }
            case Variant::Type::DICTIONARY:
            {
                SERIALIZE_VARIANT(dict);
                break;
            }
            case Variant::Type::BORROWED_OBJECT:
            case Variant::Type::OBJECT:
            {
                SERIALIZE_VARIANT(Object);
                break;
            }
            case Variant::Type::FUNCTION:
            {
                SERIALIZE_VARIANT(Function);
                break;
            }
            default:
                throw RuntimeException("Serialize<Variant> bad branch.");
                break;
            }
            return result;
        }
    };
} // namespace phi
