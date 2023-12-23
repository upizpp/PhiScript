#pragma once
#if __cplusplus > 202000LL
#include <concepts>
#include <iostream>
#include <phi/data_type.hpp>
#include <set>
#include <string>

namespace phi
{
    template <typename T>
    concept Iterable = requires(T x) {
        {
            x.begin()
        } -> std::same_as<typename T::iterator>;
        {
            x.end()
        } -> std::same_as<typename T::iterator>;
    };

    template <typename T>
    concept Tree = requires(T tree) {
        {
            tree.stringify()
        } -> std::convertible_to<std::string>;
        tree.getChildren();
        requires phi::Iterable<std::remove_reference_t<decltype(tree.getChildren())>>;
    };

    template <Tree T>
    void __print_impl(const T &tree, integer level, bool is_last, std::set<uinteger> ignore_list)
    {
        for (integer i = 0; i < level - 1; ++i)
        {
            if (ignore_list.find(i) != ignore_list.end())
                std::cout << "    ";
            else
                std::cout << "┃   ";
        }
        if (level > 0)
        {
            if (is_last)
                std::cout << "┗━━━";
            else
                std::cout << "┣━━━";
        }
        const auto &children = tree.getChildren();
        std::cout << tree.stringify() << std::endl;
        auto back = --children.end();
        for (auto it = children.begin(); it != children.end(); ++it)
        {
            if (it == back)
                ignore_list.insert(level);
            __print_impl(**it, level + 1, it == back, ignore_list);
        }
    }

    template <Tree T>
    void print(const T &tree)
    {
        __print_impl(tree, 0, false, {});
    }
} // namespace phi
#else
#error The C++standard is too low, requiring at least C++20
#endif