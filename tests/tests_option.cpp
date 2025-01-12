//
// Created by user1 on 08/12/2024.
//

#include <gtest/gtest.h>

#include "../include/CppResultOption/option.h"
#include "../include/CppResultOption/result.h"

using namespace m24;
using namespace m24::Prelude;

// TODO: implement tests for deleted constructors/operators

#pragma region Option::And
TEST(Option, And_SomeSome)
{
    Option<int> const a = Some(42);
    Option<int> const b = Some(69);

    Option<int> const c1 = a.And(b);
    Option<int> const c2 = a & b;

    EXPECT_EQ(c1, b);
    EXPECT_EQ(c1, c2);
}

TEST(Option, And_SomeNone)
{
    Option<int> const a = Some(42);
    Option<int> const b = None;

    Option<int> const c1 = a.And(b);
    Option<int> const c2 = a & b;

    EXPECT_EQ(c1, b);
    EXPECT_EQ(c1, c2);
}

TEST(Option, And_NoneSome)
{
    Option<int> const a = None;
    Option<int> const b = Some(69);

    Option<int> const c1 = a.And(b);
    Option<int> const c2 = a & b;

    EXPECT_NE(c1, b);
    EXPECT_EQ(c1, c2);
}

TEST(Option, And_NoneNone)
{
    Option<int> const a = None;
    Option<int> const b = None;

    Option<int> const c1 = a.And(b);
    Option<int> const c2 = a & b;

    EXPECT_EQ(c1, b);
    EXPECT_EQ(c1, c2);
}
#pragma endregion

#pragma region Option::AndThen
TEST(Option, AndThen_Some)
{
    Option<int> const a = Some(42);

    auto mapper = [](int x)
    {
        return Some(x * 2);
    };
    Option<int> const actual = a.AndThen<int>(mapper);
    Option<int> const expected = Some(a.Unwrap() * 2);
    EXPECT_EQ(actual, expected);
}

TEST(Option, AndThen_None)
{
    Option<int> const a = None;

    Option<int> const b = a.AndThen<int>([](int x) { return Some(x * 2); });

    EXPECT_EQ(b, None);
}
#pragma endregion

#pragma region Option::Expect
TEST(Option, Expect_Some)
{
    Option<int> const a = Some(42);

    EXPECT_NO_THROW(a.Expect("Should be some"));
}

TEST(Option, Expect_None)
{
    Option<int> const a = None;

    EXPECT_THROW(a.Expect("Should be none"), SomeExpectedException);
}
#pragma endregion

#pragma region Option::Filter
TEST(Option, Filter_SomeTrue)
{
    Option<int> const a = Some(42);

    auto filter = [](int const& x)
    {
        return x > 0;
    };
    Option<int> const b = a.Filter(filter);

    EXPECT_EQ(b, a);
}

TEST(Option, Filter_SomeFalse)
{
    Option<int> const a = Some(42);
    Option<int> const b = a.Filter([](int const& x) { return x < 0; });

    EXPECT_EQ(b, None);
}

TEST(Option, Filter_NoneTrue)
{
    Option<int> const a = None;
    Option<int> const b = a.Filter([](int const& x) { return x < 0; });

    EXPECT_EQ(b, None);
}

TEST(Option, Filter_NoneFalse)
{
    Option<int> const a = None;
    Option<int> const b = a.Filter([](int const& x) { return x < 0; });

    EXPECT_EQ(b, None);
}
#pragma endregion

#pragma region Option::IsNone
TEST(Option, Option_IsNone_Some)
{
    Option<int> const option = Some(42);
    EXPECT_FALSE(option.IsNone());
}

TEST(Option, Option_IsNone_None)
{
    Option<int> const option = None;
    EXPECT_TRUE(option.IsNone());
}
#pragma endregion

#pragma region Option::IsSome
TEST(Option, Option_IsSome_Some)
{
    Option<int> const option = Some(42);
    EXPECT_TRUE(option.IsSome());
}

TEST(Option, Option_IsSome_None)
{
    Option<int> const option = None;
    EXPECT_FALSE(option.IsSome());
}
#pragma endregion

#pragma region Option::Map
TEST(Option, Map_Some)
{
    Option<int> const option = Some(42);
    auto mapper = [](int const& x)
    {
        return x * 2;
    };

    Option<int> const actual = option.Map<int>(mapper);
    Option<int> const expected = Some(option.Unwrap() * 2);
    EXPECT_EQ(actual, expected);
}

TEST(Option, Map_None)
{
    Option<int> const option = None;
    auto mapper = [](int const& x)
    {
        return x * 2;
    };
    EXPECT_EQ(option.Map<int>(mapper), None);
}
#pragma endregion

#pragma region Option::Match
TEST(Option, Match_Some)
{
    Option<int> const option = Some(42);

    auto someMatcher = [](int const& x)
    {
        return x * 2;
    };
    auto noneMatcher = []()
    {
        return -1;
    };

    int actual = option.Match<int>(someMatcher, noneMatcher);
    int expected = option.Unwrap() * 2;
    EXPECT_EQ(actual, expected);
}

TEST(Option, Match_None)
{
    Option<int> const option = None;

    auto someMatcher = [](int const& x)
    {
        return x * 2;
    };
    auto noneMatcher = []()
    {
        return -1;
    };

    EXPECT_EQ(option.Match<int>(someMatcher, noneMatcher), -1);
}
#pragma endregion

#pragma region Option::OkOr
TEST(Option, OkOr_Some)
{
    Option<int> const option = Some(42);
    int alternative = 69;

    Result<int, int> actual = option.OkOr<int>(alternative);
    Result<int, int> expected = Ok(option.Unwrap());
    EXPECT_EQ(actual, expected);
}

TEST(Option, OkOr_None)
{
    Option<int> const option = None;
    int alternative = 69;

    Result<int, int> actual = option.OkOr<int>(alternative);
    Result<int, int> expected = Err(alternative);
    EXPECT_EQ(actual, expected);
}
#pragma endregion

#pragma region Option::OkOrElse
TEST(Option, OkOrElse_Some)
{
    Option<int> const option = Some(42);
    auto alternative = []()
    {
        return 69;
    };

    Result<int, int> const actual = option.OkOrElse<int>(alternative);
    Result<int, int> const expected = Ok(option.Unwrap());
    EXPECT_EQ(actual, expected);
}

TEST(Option, OkOrElse_None)
{
    Option<int> const option = None;
    auto alternative = []()
    {
        return 69;
    };

    Result<int, int> const actual = option.OkOrElse<int>(alternative);
    Result<int, int> const expected = Err(alternative());
    EXPECT_EQ(actual, expected);
}
#pragma endregion

#pragma region Option::Or
TEST(Option, Or_SomeSome)
{
    Option<int> const a = Some(42);
    Option<int> const b = Some(69);

    EXPECT_EQ(a.Or(b), a);
}

TEST(Option, Or_SomeNone)
{
    Option<int> const a = Some(42);
    Option<int> const b = None;

    EXPECT_EQ(a.Or(b), a);
}

TEST(Option, Or_NoneSome)
{
    Option<int> const a = None;
    Option<int> const b = Some(69);

    EXPECT_EQ(a.Or(b), b);
}

TEST(Option, Or_NoneNone)
{
    Option<int> const a = None;
    Option<int> const b = None;

    EXPECT_EQ(a.Or(b), b);
}
#pragma endregion

#pragma region Option::Replace
TEST(Option, Replace_Some)
{
    Option<int> a = Some(42);
    Option<int> b = a.Replace(69);

    EXPECT_EQ(a, Some(69));
    EXPECT_EQ(b, Some(42));
}

TEST(Option, Replace_None)
{
    Option<int> a = None;
    Option<int> b = a.Replace(69);

    EXPECT_EQ(a, Some(69));
    EXPECT_EQ(b, None);
}
#pragma endregion

#pragma region Option::Take
TEST(Option, Take_Some)
{
    Option<int> a = Some(42);
    Option<int> b = a.Take();

    EXPECT_EQ(a, None);
    EXPECT_EQ(b, Some(42));
}

TEST(Option, Take_None)
{
    Option<int> a = None;
    Option<int> b = a.Take();

    EXPECT_EQ(a, None);
    EXPECT_EQ(b, None);
}
#pragma endregion

#pragma region Option::Unwrap
TEST(Option, Unwrap_Some)
{
    Option<int> a = Some(42);
    EXPECT_NO_THROW(a.Unwrap());
}

TEST(Option, Unwrap_None)
{
    Option<int> a = None;
    EXPECT_THROW(a.Unwrap(), SomeExpectedException);
}
#pragma endregion

#pragma region Option::UnwrapOr
TEST(Option, UnwrapOr_Some)
{
    Option<int> const option = Some(42);
    EXPECT_EQ(option.UnwrapOr(69), 42);
}

TEST(Option, UnwrapOr_None)
{
    Option<int> const option = None;
    EXPECT_EQ(option.UnwrapOr(69), 69);
}
#pragma endregion

#pragma region Option::Xor
TEST(Option, Xor_SomeSome)
{
    Option<int> a = Some(42);
    Option<int> b = Some(69);

    Option<int> actual = a.Xor(b);
    Option<int> expected = None;
    EXPECT_EQ(actual, expected);
}

TEST(Option, Xor_SomeNone)
{
    Option<int> a = Some(42);
    Option<int> b = None;

    Option<int> actual = a.Xor(b);
    const Option<int>& expected = a;
    EXPECT_EQ(actual, expected);
}

TEST(Option, Xor_NoneSome)
{
    Option<int> a = None;
    Option<int> b = Some(69);

    Option<int> actual = a.Xor(b);
    const Option<int>& expected = b;
    EXPECT_EQ(actual, expected);
}

TEST(Option, Xor_NoneNone)
{
    Option<int> a = None;
    Option<int> b = None;

    Option<int> actual = a.Xor(b);
    Option<int> expected = None;
    EXPECT_EQ(actual, expected);
}
#pragma endregion

#pragma region Option::Zip
TEST(Option, Zip_SomeSome)
{
    Option<int> a = Some(42);
    Option<int> b = Some(69);

    Option<std::pair<int, int>> actual = a.Zip(b);
    Option<std::pair<int, int>> expected = Some(std::make_pair(42, 69));

    EXPECT_EQ(actual, expected);
}

TEST(Option, Zip_SomeNone)
{
    Option<int> a = Some(42);
    Option<int> b = None;

    Option<std::pair<int, int>> actual = a.Zip(b);
    Option<std::pair<int, int>> expected = None;

    EXPECT_EQ(actual, expected);
}

TEST(Option, Zip_NoneSome)
{
    Option<int> a = None;
    Option<int> b = Some(69);

    Option<std::pair<int, int>> actual = a.Zip(b);
    Option<std::pair<int, int>> expected = None;

    EXPECT_EQ(actual, expected);
}

TEST(Option, Zip_NoneNone)
{
    Option<int> a = None;
    Option<int> b = None;

    Option<std::pair<int, int>> actual = a.Zip(b);
    Option<std::pair<int, int>> expected = None;

    EXPECT_EQ(actual, expected);
}

#pragma endregion

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};
