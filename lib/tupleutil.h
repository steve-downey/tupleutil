// tupleutil.h                                                     -*-C++-*-
#ifndef INCLUDED_TUPLEUTIL
#define INCLUDED_TUPLEUTIL

#include <iostream>
#include <tuple>
#include <functional>
#include <new>
#include <variant>

namespace tupleutil {

template <typename Func, typename Tuple, std::size_t... I>
void tuple_for_each_impl(Tuple&& tuple, Func&& f, std::index_sequence<I...>)
{
    auto swallow = {0,
                    (std::forward<Func>(f)(
                        I, std::get<I>(std::forward<Tuple>(tuple))))...};
    (void)swallow;
}

template <typename Func, typename... Args>
void tuple_for_each(std::tuple<Args...> const& tuple, Func&& f)
{
    tuple_for_each_impl(tuple, f, std::index_sequence_for<Args...>{});
}

template <typename Func, typename... Args>
void tuple_for_each(std::tuple<Args...>&& tuple, Func&& f)
{
    tuple_for_each_impl(tuple, f, std::index_sequence_for<Args...>{});
}

template <typename Func, typename Tuple, std::size_t... I>
auto tuple_transform_impl(Tuple&& tuple, Func&& f, std::index_sequence<I...>)
{
    return std::make_tuple(
        std::forward<Func>(f)(std::get<I>(std::forward<Tuple>(tuple)))...);
}

template <typename Func, typename... Args>
auto tuple_transform(std::tuple<Args...>&& tuple, Func&& f)
{
    return tuple_transform_impl(tuple, f, std::index_sequence_for<Args...>{});
}

template <typename Func, typename... Args>
auto tuple_transform(std::tuple<Args...> const& tuple, Func&& f)
{
    return tuple_transform_impl(tuple, f, std::index_sequence_for<Args...>{});
}

template <typename... Args, std::size_t... I>
constexpr std::array<std::variant<Args...>, sizeof...(Args)>
tuple_to_array_impl(std::tuple<Args...> const& tuple,
                    std::index_sequence<I...>)
{
    using V = std::variant<Args...>;
    std::array<V, sizeof...(Args)> array = {
        {V(std::in_place_index_t<I>{}, std::get<I>(tuple))...}};
    return array;
}

template <typename... Args>
constexpr std::array<std::variant<Args...>, sizeof...(Args)>
tuple_to_array(std::tuple<Args...> const& tuple)
{
    return tuple_to_array_impl(tuple, std::index_sequence_for<Args...>{});
}

template <typename V, typename T, size_t I> auto get_getter()
{
    return [](T const& t) {
        return V{std::in_place_index_t<I>{}, std::get<I>(t)};
    };
}

template <typename... Args, std::size_t... I>
auto tuple_getters_impl(std::index_sequence<I...>)
{
    using V = std::variant<Args...>;
    using T = std::tuple<Args...>;
    using F = V (*)(T const&);
    std::array<F, sizeof...(Args)> array
        //        = {{[](T const& tuple){return V{std::get<I>(tuple)};}...}};
        = {{get_getter<V, T, I>()...}};
    return array;
}

template <typename... Args> auto tuple_getters(std::tuple<Args...>)
{
    return tuple_getters_impl<Args...>(std::index_sequence_for<Args...>{});
}

const auto get = [](size_t i, auto t) {
    static auto tbl = tupleutil::tuple_getters(t);
    return tbl[i](t);
};

template <typename T>
struct TupleGetters {
    auto operator()();
};

template<typename ...Types>
struct TupleGetters<std::tuple<Types...>> {
    auto operator()() {
        return tuple_getters_impl<Types...>(std::index_sequence_for<Types...>{});
    }
};

template <typename T>
auto tuple_getters(){
    TupleGetters<T> g;
    return g();
}

template <typename... Args>
void print(std::ostream& os, std::tuple<Args...> const& tuple)
{
    auto printer = [&os](auto i, auto el) {
        os << (i == 0 ? "" : ", ") << el;
        return 0;
    };
    return tuple_for_each(tuple, printer);
}

} // close namespace tupleutil

#endif
