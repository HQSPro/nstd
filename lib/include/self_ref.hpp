
#ifndef __NSTD_SELF_REF_HPP__
#define __NSTD_SELF_REF_HPP__

#include "type_traits.hpp"
#include "utility.hpp"
#include "any.hpp"

namespace nstd {

template <typename B>
class SelfRef {
    std::any owner;
    B& ref;

public:
    template <typename D, typename... Args>
    constexpr SelfRef(nstd::in_place_type_t<D> it, Args&&... args)
        : owner(it, std::forward<Args>(args)...), ref(*nstd::any_cast<D>(&owner))
    {
    }
    inline constexpr nstd::add_const_t<nstd::add_lvalue_reference_t<B>> operator*() const
    {
        return ref;
    }
    inline constexpr nstd::add_lvalue_reference_t<B> operator*() noexcept { return ref; }
    inline constexpr nstd::add_const_t<nstd::add_pointer_t<B>> operator->() const noexcept { return &ref; }
    inline constexpr nstd::add_pointer_t<B> operator->() noexcept { return &ref; }
};
template <typename B, typename D, typename... Args>
constexpr SelfRef<B> make_self_ref(Args&&... args)
{
    return SelfRef<B>(nstd::in_place_type_t<D>{}, std::forward<Args>(args)...);
}
}  // namespace nstd

#endif
