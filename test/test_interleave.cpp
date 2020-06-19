
#include <flow.hpp>

#include "catch.hpp"

#include <iostream>

namespace {

constexpr bool test_interleave()
{
    {
        auto b = flow::c_str("ABC")
                     .copy()
                     .interleave(flow::iota('1'))
                     .equal(flow::c_str("A1B2C3"));
        if (!b) {
            return false;
        }
    }

    {
        auto is_even = [](auto i) { return i % 2 == 0; };

        auto evens = flow::ints().filter(is_even);
        auto odds = flow::ints().filter(flow::pred::not_(is_even));

        bool b = std::move(evens)
            .interleave(odds)
            .take(10)
            .equal(flow::ints().take(10));
        if (!b) {
            return false;
        }
    }

    {
        bool b = flow::c_str("ABCDEF")
                     .interleave(flow::c_str(" ").cycle())
                     .equal(flow::c_str("A B C D E F "));
        if (!b) {
            return false;
        }
    }

    return true;
}
static_assert(test_interleave());

TEST_CASE("interleave", "[flow.interleave]")
{
    REQUIRE(test_interleave());
}

}