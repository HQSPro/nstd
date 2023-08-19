#ifndef __NSTD_OPTION_HPP__
#define __NSTD_OPTION_HPP__

#include <cassert>
#include "type_traits.hpp"
#include "variant.hpp"

namespace nstd {

enum OptionStatus
{
    SOME = 0,
    NONE,
};

template <typename SomeType>
class Some {
private:
    SomeType value;

public:
    Some(nstd::add_rvalue_reference_t<SomeType> v) : value(std::move(v)) {}
    Some(nstd::add_const_t<nstd::add_lvalue_reference_t<SomeType>> v) : value(v) {}
    Some(Some<SomeType>&& some) : value(std::move(some.value)) {}
    Some(const Some<SomeType>& some) : value(some.value) {}
    Some<SomeType>& operator=(Some<SomeType>&&)      = default;
    Some<SomeType>& operator=(const Some<SomeType>&) = default;
    inline constexpr nstd::add_const_t<nstd::add_lvalue_reference_t<SomeType>>
    operator*() const noexcept
    {
        return value;
    }
    inline constexpr nstd::add_lvalue_reference_t<SomeType> operator*() noexcept { return value; }
    inline constexpr nstd::add_const_t<nstd::add_pointer_t<SomeType>> operator->() const noexcept
    {
        return &value;
    }
    inline constexpr nstd::add_pointer_t<SomeType> operator->() noexcept { return &value; }
};

using None = nstd::monostate;

template <typename SomeType>
class Option : nstd::variant<Some<SomeType>, None> {
private:
    OptionStatus m_status;

public:
    Option(nstd::add_rvalue_reference_t<SomeType> some)
        : m_status(OptionStatus::SOME), nstd::variant<Some<SomeType>, None>(
                                            nstd::in_place_type<Some<SomeType>>, std::move(some))
    {
    }
    Option(nstd::add_const_t<nstd::add_lvalue_reference_t<SomeType>> some)
        : m_status(OptionStatus::SOME), nstd::variant<Some<SomeType>, None>(
                                            nstd::in_place_type<Some<SomeType>>, some)
    {
    }
    Option()
        : m_status(OptionStatus::NONE), nstd::variant<Some<SomeType>, None>(
                                            std::in_place_type<None>, None{})
    {
    }
    Option(const Option<SomeType>& option)
        : m_status(option.m_status), nstd::variant<Some<SomeType>, None>(option)
    {
    }
    Option(Option<SomeType>&& option)
        : m_result(option.m_status), nstd::variant<Some<SomeType>, None>(std::move(option))
    {
    }
    Option<SomeType>& operator=(Option<SomeType>&&)      = default;
    Option<SomeType>& operator=(const Option<SomeType>&) = default;

    inline constexpr OptionStatus status() const noexcept { return m_status; }
    inline constexpr operator OptionStatus() const noexcept { return m_status; }
    inline constexpr bool is_some() const noexcept { return OptionStatus::SOME == m_status; }
    inline constexpr bool is_none() const noexcept { return OptionStatus::NONE == m_status; }
};

template <typename T, nstd::enable_if_t<nstd::is_pointer_v<T>, bool> = true>
class NotNull {
    T p;

public:
    NotNull(T ptr) : p(ptr) { assert(ptr != nullptr); }
    inline constexpr nstd::add_const_t<nstd::add_lvalue_reference_t<nstd::remove_pointer_t<T>>>
    operator*() const noexcept
    {
        return *p;
    }
    inline constexpr nstd::add_lvalue_reference_t<nstd::remove_pointer_t<T>> operator*() noexcept
    {
        return *p;
    }
    inline constexpr nstd::add_const_t<T> operator->() const noexcept { return p; }
    inline constexpr T operator->() noexcept { return p; }
    inline constexpr T get() noexcept { return p; }
};

class MayNull;

template <typename T, nstd::enable_if_t<nstd::is_pointer_v<T>, bool> = true>
class MayNull {
    T p;

public:
    MayNull(T ptr) : p(ptr) {}
    inline constexpr T get() noexcept { return p; }
};

}  // namespace nstd

#endif
