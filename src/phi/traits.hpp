
#pragma once
#include <functional>
#include <type_traits>

namespace phi
{
    template <typename T>
    struct function_traits;

    template <typename ReturnType, typename... Args>
    struct function_traits<ReturnType(Args...)>
    {
        enum
        {
            arity = sizeof...(Args)
        };
        using return_type = ReturnType;
        using function_type = ReturnType(Args...);
        using stl_function_type = std::function<function_type>;
        using pointer = ReturnType (*)(Args...);

        template <std::size_t I>
        struct args
        {
            static_assert(I < arity, "index is out of range, index must less than sizeof Args");
            using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
        };

        using tuple_type = std::tuple<typename std::remove_cv<typename std::remove_reference<Args>::type>::type...>;
        using bare_tuple_type = std::tuple<typename std::remove_const<typename std::remove_reference<Args>::type>::type...>;

        using class_type = void;
    };

    template <typename ReturnType, typename... Args>
    struct function_traits<ReturnType (*)(Args...)> : function_traits<ReturnType(Args...)>
    {
    };

    template <typename ReturnType, typename... Args>
    struct function_traits<std::function<ReturnType(Args...)>> : function_traits<ReturnType(Args...)>
    {
    };

#define FUNCTION_TRAITS(...)                                                                                      \
    template <typename ReturnType, typename ClassType, typename... Args>                                          \
    struct function_traits<ReturnType (ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)> \
    {                                                                                                             \
        using class_type = ClassType;                                                                             \
    };

    FUNCTION_TRAITS()
    FUNCTION_TRAITS(const)
    FUNCTION_TRAITS(volatile)
    FUNCTION_TRAITS(const volatile)

    template <typename Callable>
    struct function_traits : function_traits<decltype(&Callable::operator())>
    {
    };

    template <typename Function>
    typename function_traits<Function>::stl_function_type to_function(const Function &lambda)
    {
        return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
    }

    template <typename Function>
    typename function_traits<Function>::stl_function_type to_function(Function &&lambda)
    {
        return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
    }

    template <typename Function>
    typename function_traits<Function>::pointer to_function_pointer(const Function &lambda)
    {
        return static_cast<typename function_traits<Function>::pointer>(lambda);
    }

    template <size_t... Ts>
    struct index_seq
    {
        template <typename T>
        struct append;

        template <template <size_t...> class T, size_t... V>
        struct append<T<V...>>
        {
            using type = index_seq<Ts..., V...>;
        };

        template <typename T>
        using append_t = typename append<T>::type;
    };

    template <size_t C, size_t E, size_t S = 1>
    struct gen_seq
    {
        using type = typename index_seq<C>::template append_t<typename gen_seq<C + S, E, S>::type>;
    };

    template <size_t E, size_t S>
    struct gen_seq<E, E, S>
    {
        using type = index_seq<E>;
    };

    template <size_t C, size_t E, size_t S = 1>
    using gen_seq_t = typename gen_seq<C, E, S>::type;

    template <size_t C>
    struct gen_index_seq
    {
        using type = typename gen_seq<0, C - 1, 1>::type;
    };

    template <>
    struct gen_index_seq<0>
    {
        using type = index_seq<>;
    };

    template <size_t C>
    using gen_index_seq_t = typename gen_index_seq<C>::type;

    template <typename T>
    struct FuncTraits;

    template <typename R, typename... A>
    struct FuncTraits<R(A...)>
    {
        using ret = R;
        static constexpr size_t count = sizeof...(A);
    };
} // namespace phi
