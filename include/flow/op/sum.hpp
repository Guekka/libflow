
#ifndef FLOW_OP_SUM_HPP_INCLUDED
#define FLOW_OP_SUM_HPP_INCLUDED

#include <flow/core/flow_base.hpp>

namespace flow {

inline constexpr auto sum = [](auto&& flowable)
{
    static_assert(is_flowable<decltype(flowable)>,
                  "Argument to flow::sum() must be Flowable");
    return flow::from(FLOW_FWD(flowable)).sum();
};

template <typename D>
constexpr auto flow_base<D>::sum()
{
    return derived().fold(std::plus<>{});
}

}

#endif
