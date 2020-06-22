
#ifndef FLOW_OP_PRODUCT_HPP_INCLUDED
#define FLOW_OP_PRODUCT_HPP_INCLUDED

#include <flow/core/flow_base.hpp>

namespace flow {

inline constexpr auto product = [](auto&& flowable)
{
  static_assert(is_flowable<decltype(flowable)>,
                "Argument to flow::sum() must be Flowable");
  return flow::from(FLOW_FWD(flowable)).product();
};

template <typename D>
constexpr auto flow_base<D>::product()
{
    static_assert(std::is_constructible_v<value_t<D>, int>,
                  "Flow's value type must be constructible from a literal 1");
    return derived().fold(std::multiplies<>{}, value_t<D>{1});
}

}

#endif
