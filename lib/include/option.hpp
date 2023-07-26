#ifndef __NSTD_OPTION_HPP__
#define __NSTD_OPTION_HPP__

#include <type_traits>
#include <variant>
#include <type_magic.hpp>

namespace nstd {

enum OptionStatus
{
    SOME,
    NONE,
};

template <typename SomeType>
class Some
{
private:
    SomeType value;

public:
    Some(std::add_rvalue_reference_t<SomeType> v) : value(std::forward<SomeType>(v)) {}
    Some(std::add_const_t<std::add_lvalue_reference_t<SomeType>> v) : value(v) {}
    Some(Some&& some) : value(std::move(some.value)) {}
    Some(const Some& some) : value(some.value) {}
    std::add_const_t<std::add_lvalue_reference_t<SomeType>> operator*() const { return value; }
    std::add_lvalue_reference_t<SomeType> operator*() { return value; }
    std::add_const_t<std::add_pointer_t<SomeType>> operator->() const { return &value; }
    std::add_pointer_t<SomeType> operator->() { return &value; }
};

class None
{
};

template <typename SomeType>
class Option
{
private:
    OptionStatus m_status;
    std::variant<Some<SomeType>, None> value;

public:
    Option(std::add_rvalue_reference_t<SomeType> some)
        : m_status(OptionStatus::SOME),
          value(std::in_place_type_t<Some<SomeType>>{}, std::forward<SomeType>(some))
    {
    }
    Option(std::add_const_t<std::add_lvalue_reference_t<SomeType>> some)
        : m_status(OptionStatus::SOME),
          value(std::in_place_type_t<Some<SomeType>>{}, std::forward<SomeType>(some))
    {
    }
    Option() : m_status(OptionStatus::NONE), value(std::in_place_type_t<None>{}, None{}) {}
    Option(const Option<SomeType>& option) : m_status(option.m_status), value(option.value) {}
    Option(Option<SomeType>&& option) : m_result(option.m_status), value(std::move(option.value)) {}
    inline OptionStatus status() const { return m_status; }
    operator OptionStatus() const noexcept { return m_status; }
    inline bool is_some() const noexcept { return OptionStatus::SOME == m_status; }
    inline bool is_none() const noexcept { return OptionStatus::NONE == m_status; }
    std::add_const_t<std::add_lvalue_reference_t<std::variant<Some<SomeType>, None>>>
    operator*() const
    {
        return value;
    }
    std::add_lvalue_reference_t<std::variant<Some<SomeType>, None>> operator*() { return value; }
    std::add_const_t<std::add_pointer_t<std::variant<Some<SomeType>, None>>> operator->() const
    {
        return &value;
    }
    std::add_pointer_t<std::variant<Some<SomeType>, None>> operator->() { return &value; }
};

namespace std {
template <std::size_t I, typename SomeType>
constexpr std::add_lvalue_reference_t<type_pack_element_t<I, nstd::Some<SomeType>, nstd::None>>
get(nstd::Option<SomeType>& r)
{
    return std::get<I>(*r);
}
template <std::size_t I, typename SomeType>
constexpr std::add_rvalue_reference_t<type_pack_element_t<I, nstd::Some<SomeType>, nstd::None>>
get(nstd::Option<SomeType>&& r)
{
    return std::get<I>(std::move(*r));
}
template <std::size_t I, typename SomeType>
constexpr std::add_const_t<
    std::add_lvalue_reference_t<type_pack_element_t<I, nstd::Some<SomeType>, nstd::None>>>
get(const nstd::Option<SomeType>& r)
{
    return std::get<I>(*r);
}
template <std::size_t I, typename SomeType>
constexpr std::add_const_t<
    std::add_rvalue_reference_t<type_pack_element_t<I, nstd::Some<SomeType>, nstd::None>>>
get(const nstd::Option<SomeType>&& r)
{
    return std::get<I>(std::move(const_cast<nstd::Option<SomeType>&>(*r)));
}

template <typename T, typename SomeType>
constexpr std::add_lvalue_reference_t<T> get(nstd::Option<SomeType>& r)
{
    return std::get<T>(*r);
}
template <typename T, typename SomeType>
constexpr std::add_rvalue_reference_t<T> get(nstd::Option<SomeType>&& r)
{
    return std::get<T>(std::move(*r));
}
template <typename T, typename SomeType>
constexpr std::add_const_t<std::add_lvalue_reference_t<T>> get(const nstd::Option<SomeType>& r)
{
    return std::get<T>(*r);
}
template <typename T, typename SomeType>
constexpr std::add_const_t<std::add_rvalue_reference_t<T>> get(const nstd::Option<SomeType>&& r)
{
    return std::get<T>(std::move(const_cast<nstd::Option<SomeType>&>(*r)));
}

template <std::size_t I, typename SomeType>
constexpr std::add_pointer_t<type_pack_element_t<I, nstd::Some<SomeType>, nstd::None>>
get_if(nstd::Option<SomeType>* r)
{
    return std::get<I>(&**r);
}
template <std::size_t I, typename SomeType>
constexpr std::add_const_t<
    std::add_pointer_t<type_pack_element_t<I, nstd::Some<SomeType>, nstd::None>>>
get_if(const nstd::Option<SomeType>* r)
{
    return std::get<I>(&**r);
}
template <typename T, typename SomeType>
constexpr std::add_pointer_t<T> get_if(nstd::Option<SomeType>* r)
{
    return std::get<T>(&**r);
}
template <typename T, typename SomeType>
constexpr std::add_const_t<std::add_pointer_t<T>> get_if(const nstd::Option<SomeType>* r)
{
    return std::get<T>(&**r);
}
} // namespace std

} // namespace nstd

#endif
