#include "tuplefunctor.h"

#include "gtest/gtest.h"

using ::testing::Test;

namespace testing {

TEST(TupleTest, maketuple)
{
    constexpr std::tuple<int, double, long> t = std::make_tuple(1, 2.3, 1l);
    auto arr = tuplefunctor::tuple_to_array(t);
    int i = std::get<int>(arr[0]);
    //static_assert(i == 1);
    EXPECT_EQ(1, i);

    auto transform = tuplefunctor::tuple_transform(t, [](auto i){return i+1;});
    tuplefunctor::print(std::cout, transform);
    std::cout << '\n';

    auto t2 = tuplefunctor::tuple_transform(std::make_tuple(4, 5.0),
                                            [](auto i){return i+1;});
    tuplefunctor::print(std::cout, t2);
    std::cout << '\n';
}

TEST(TupleTest, getters)
{
    constexpr std::tuple<int, double, long> t = std::make_tuple(1, 2.3, 1l);
    auto arr = tuplefunctor::tuple_getters(t);

    auto getter = arr[0];
    auto v = getter(t);
    int i = std::get<0>(v);
    EXPECT_EQ(1, i);
}

TEST(TupleTest, gettersStatic)
{
    constexpr std::tuple<int, double, long> t = std::make_tuple(1, 2.3, 1l);

    auto v = tuplefunctor::get(0, t);
    int i = std::get<0>(v);
    EXPECT_EQ(1, i);

    auto v2 = tuplefunctor::get(1, t);

    EXPECT_EQ(1ul, v2.index());
    double d = std::get<double>(v2);

    EXPECT_EQ(2.3, d);

    constexpr auto t2 = std::make_tuple(2.4, 1l);
    auto v3 = tuplefunctor::get(0, t2);
    double d2 = std::get<double>(v3);

    EXPECT_EQ(2.4, d2);

}

TEST(TupleTest, gettersDupTypes)
{
    auto t = std::make_tuple(1, 2, 3, 4);
    auto arr = tuplefunctor::tuple_getters(t);

    auto getter = arr[3];
    auto v = getter(t);
    EXPECT_EQ(3ul, v.index());
    int i = std::get<3>(v);
    EXPECT_EQ(4, i);
}

}
