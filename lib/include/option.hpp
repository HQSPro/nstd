#ifndef __NSTD_OPTION_HPP__
#define __NSTD_OPTION_HPP__

#include <type_traits>
#include <variant>
#include "type_magic.hpp"

namespace nstd {

enum OptionStatus
{
    SOME,
    NONE,
};

template <typename SomeType>
class Some {
private:
    SomeType value;

public:
    constexpr Some(std::add_rvalue_reference_t<SomeType> v) : value(std::forward<SomeType>(v)) {}
    constexpr Some(std::add_const_t<std::add_lvalue_reference_t<SomeType>> v) : value(v) {}
    constexpr Some(Some&& some) : value(std::move(some.value)) {}
    constexpr Some(const Some& some) : value(some.value) {}
    inline constexpr std::add_const_t<std::add_lvalue_reference_t<SomeType>> operator*() const { return value; }
    inline constexpr std::add_lvalue_reference_t<SomeType> operator*() { return value; }
    inline constexpr std::add_const_t<std::add_pointer_t<SomeType>> operator->() const { return &value; }
    inline constexpr std::add_pointer_t<SomeType> operator->() { return &value; }
};

class None {};

template <typename SomeType>
class Option {
private:
    OptionStatus m_status;
    std::variant<Some<SomeType>, None> value;

public:
    constexpr Option(std::add_rvalue_reference_t<SomeType> some)
        : m_status(OptionStatus::SOME), value(std::in_place_type_t<Some<SomeType>>{}, std::forward<SomeType>(some))
    {
    }
    constexpr Option(std::add_const_t<std::add_lvalue_reference_t<SomeType>> some)
        : m_status(OptionStatus::SOME), value(std::in_place_type_t<Some<SomeType>>{}, std::forward<SomeType>(some))
    {
    }
    constexpr Option() : m_status(OptionStatus::NONE), value(std::in_place_type_t<None>{}, None{}) {}
    constexpr Option(const Option<SomeType>& option) : m_status(option.m_status), value(option.value) {}
    constexpr Option(Option<SomeType>&& option) : m_result(option.m_status), value(std::move(option.value)) {}
    inline constexpr OptionStatus status() const { return m_status; }
    inline constexpr operator OptionStatus() const noexcept { return m_status; }
    inline constexpr bool is_some() const noexcept { return OptionStatus::SOME == m_status; }
    inline constexpr bool is_none() const noexcept { return OptionStatus::NONE == m_status; }
    inline constexpr std::add_const_t<std::add_lvalue_reference_t<std::variant<Some<SomeType>, None>>> operator*() const { return value; }
    inline constexpr std::add_lvalue_reference_t<std::variant<Some<SomeType>, None>> operator*() { return value; }
    inline constexpr std::add_const_t<std::add_pointer_t<std::variant<Some<SomeType>, None>>> operator->() const { return &value; }
    inline constexpr std::add_pointer_t<std::variant<Some<SomeType>, None>> operator->() { return &value; }
};

typedef<typename T, std::enable_if_t<std::is_pointer_v<T>, bool> = true> class NotNull {
    T p;

public:
    constexpr NotNull(T ptr) : p(ptr) {}
    inline constexpr std::add_const_t<std::add_lvalue_reference_t<std::remove_pointer_t<T>>> operator*() const { return *p; }
    inline constexpr std::add_lvalue_reference_t<std::remove_pointer_t<T>> operator*() { return *p; }
    inline constexpr std::add_const_t<T> operator->() const { return p; }
    inline constexpr T operator->() { return p; }
}

}  // namespace nstd

namespace std {
template <std::size_t I, typename SomeType>
constexpr std::add_lvalue_reference_t<nstd::type_pack_element_t<I, SomeType, nstd::None>> get(nstd::Option<SomeType>& r)
{
    return *std::get<I>(*r);
}
template <std::size_t I, typename SomeType>
constexpr std::add_rvalue_reference_t<nstd::type_pack_element_t<I, SomeType, nstd::None>> get(nstd::Option<SomeType>&& r)
{
    return std::move(*std::get<I>(std::move(*r)));
}
template <std::size_t I, typename SomeType>
constexpr std::add_const_t<std::add_lvalue_reference_t<nstd::type_pack_element_t<I, SomeType, nstd::None>>> get(const nstd::Option<SomeType>& r)
{
    return *std::get<I>(*r);
}
template <std::size_t I, typename SomeType>
constexpr std::add_const_t<std::add_rvalue_reference_t<nstd::type_pack_element_t<I, SomeType, nstd::None>>> get(const nstd::Option<SomeType>&& r)
{
    return std::move(*std::get<I>(std::move(*r)));
}

template <typename T, typename SomeType>
constexpr std::add_lvalue_reference_t<T> get(nstd::Option<SomeType>& r)
{
    return *std::get<T>(*r);
}
template <typename T, typename SomeType>
constexpr std::add_rvalue_reference_t<T> get(nstd::Option<SomeType>&& r)
{
    return std::move(*std::get<T>(std::move(*r)));
}
template <typename T, typename SomeType>
constexpr std::add_const_t<std::add_lvalue_reference_t<T>> get(const nstd::Option<SomeType>& r)
{
    return *std::get<T>(*r);
}
template <typename T, typename SomeType>
constexpr std::add_const_t<std::add_rvalue_reference_t<T>> get(const nstd::Option<SomeType>&& r)
{
    return std::move(*std::get<T>(std::move(*r)));
}

template <std::size_t I, typename SomeType>
constexpr std::add_pointer_t<nstd::type_pack_element_t<I, SomeType, nstd::None>> get_if(nstd::Option<SomeType>* r)
{
    return &**std::get_if<I>(&**r);
}
template <std::size_t I, typename SomeType>
constexpr std::add_const_t<std::add_pointer_t<nstd::type_pack_element_t<I, SomeType, nstd::None>>> get_if(const nstd::Option<SomeType>* r)
{
    return &**std::get<I>(&**r);
}
template <typename T, typename SomeType>
constexpr std::add_pointer_t<T> get_if(nstd::Option<SomeType>* r)
{
    return &**std::get<T>(&**r);
}
template <typename T, typename SomeType>
constexpr std::add_const_t<std::add_pointer_t<T>> get_if(const nstd::Option<SomeType>* r)
{
    return &**std::get<T>(&**r);
}
}  // namespace std

#endif
