
#pragma once

namespace phi
{

    template <int... Ts>
    struct index_seq
    {
        template <typename T>
        struct append;

        template <template <int...> typename T, int... V>
        struct append<T<V...>>
        {
            using type = index_seq<Ts..., V...>;
        };

        template <typename T>
        using append_t = typename append<T>::type;
    };

    template <int C, int E, int S = 1>
    struct gen_seq
    {
        using type = typename index_seq<C>::template append_t<typename gen_seq<C + S, E, S>::type>;
    };

    template <int E, int S>
    struct gen_seq<E, E, S>
    {
        using type = index_seq<E>;
    };

    template <int C, int E, int S = 1>
    using gen_seq_t = typename gen_seq<C, E, S>::type;

    template <int C>
    struct gen_index_seq
    {
        using type = typename gen_seq<0, C - 1, 1>::type;
    };

    template <>
    struct gen_index_seq<0>
    {
        using type = index_seq<>;
    };

    template <int C>
    using gen_index_seq_t = typename gen_index_seq<C>::type;

    template <typename T>
    struct FuncTraits;

    template <typename R, typename... A>
    struct FuncTraits<R(A...)>
    {
        using ret = R;
        static constexpr int count = sizeof...(A);
    };
} // namespace phi
