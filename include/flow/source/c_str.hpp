
#ifndef FLOW_SOURCE_C_STR_HPP_INCLUDED
#define FLOW_SOURCE_C_STR_HPP_INCLUDED

#include <flow/core/flow_base.hpp>

namespace flow {

template <typename CharT, typename Traits = std::char_traits<CharT>>
struct c_str : flow_base<c_str<CharT, Traits>> {

    constexpr explicit c_str(CharT* str)
        : str_(str)
    {}

    constexpr auto next() -> maybe<CharT&>
    {
        CharT& c = str_[idx_];
        if (Traits::eq(c, CharT{})) {
            return {};
        }
        ++idx_;
        return {c};
    }

private:
    CharT* str_;
    dist_t idx_ = 0;
};

}

#endif
