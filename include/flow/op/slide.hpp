
#ifndef FLOW_OP_SLIDE_HPP_INCLUDED
#define FLOW_OP_SLIDE_HPP_INCLUDED

#include <flow/core/flow_base.hpp>

namespace flow {

namespace detail {

template <typename Flow>
struct slide_adaptor : flow_base<slide_adaptor<Flow>>
{
private:
    Flow flow_;
    subflow_t<Flow> prev_ = flow_.subflow();
    dist_t win_;
    dist_t step_;
    bool partial_;
    bool first_ = true;
    bool done_ = false;

    using item_type = decltype(flow_.subflow().take(win_));

    constexpr auto do_next_partial() -> maybe<item_type>
    {
        if (step_ > 1 && !first_) {
            if (!flow_.advance(step_ - 1).has_value()) {
                done_ = true;
                return {};
            }
        }

        first_ = false;
        auto sub = flow_.subflow();

        if (flow_.next()) {
            return std::move(sub).take(win_);
        } else {
            done_ = true;
            return {};
        }
    }

    // If we don't want partial windows, we need to iterate the main flow
    // "ahead" window_size places, and return a subflow
    constexpr auto do_next_no_partial() -> maybe<item_type>
    {
        if (first_) {
            first_ = false;
            if (!flow_.advance(win_)) {
                done_ = true;
                return {};
            }
        } else if (step_ > 1) {
            if (!flow_.advance(step_ - 1)) {
                done_ = true;
                return {};
            }

            // We should be able to do this safely since we've already
            // advanced the "main" flow, which is window_size steps ahead
            (void) prev_.advance(step_ - 1);
        }

        auto sub = prev_.subflow();
        if (!flow_.next()) {
            done_ = true; // This is the last window
        }

#ifndef NDEBUG
        assert(prev_.next().has_value());
#else
        (void) prev_.next();
#endif
        return std::move(sub).take(win_);
    }

public:
    constexpr slide_adaptor(Flow&& flow, dist_t win, dist_t step, bool partial)
        : flow_(std::move(flow)),
          win_(win),
          step_(step),
          partial_(win_ == 1 || partial) // partial windows cannot occur with window size of 1
    {}

    constexpr auto next() -> maybe<item_type>
    {
        if (done_) {
            return {};
        }

        if (partial_) {
            return do_next_partial();
        } else {
            return do_next_no_partial();
        }
    }

    template <typename F = Flow>
    constexpr auto subflow() -> slide_adaptor<subflow_t<F>>
    {
        // FIXME: Why do we need to drop an item from the subflows here?
        if (partial_) {
            auto sub = flow_.subflow();
            (void) sub.next();
            return slide_adaptor<subflow_t<Flow>>(std::move(sub), win_, step_, partial_);
        } else {
            auto sub = prev_.subflow();
            (void) sub.next();
            return slide_adaptor<subflow_t<Flow>>(std::move(sub), win_, step_, partial_);
        }
    }
};

}

template <typename D>
constexpr auto flow_base<D>::slide(dist_t window_size,
                                   dist_t step_size,
                                   bool partial_windows) &&
{
    static_assert(is_multipass_flow<D>,
                  "slide() can only be used with multipass flows");
    assert(window_size > 0);
    assert(step_size > 0);

    return detail::slide_adaptor<D>(consume(), window_size, step_size, partial_windows);
}

}

#endif
