
#include <flow.hpp>

#include "catch.hpp"

#include <sstream>

namespace {

TEST_CASE("Output to (member)", "[flow.output_to]")
{
    std::vector<int> out;

    flow::of(1, 2, 3, 4, 5, 6)
        .filter(flow::pred::even)
        .output_to(std::back_inserter(out));

    REQUIRE((out == std::vector{2, 4, 6}));
}

TEST_CASE("Output to (free)", "[flow.output_to]")
{
    std::ostringstream oss;

    flow::output_to(std::vector{1, 2, 3, 4, 5},
                    std::ostream_iterator<int>(oss));

    REQUIRE(oss.str() == "12345");
}

}