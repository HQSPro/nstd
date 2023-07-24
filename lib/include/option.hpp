#ifndef __NSTD_OPTION_HPP__
#define __NSTD_OPTION_HPP__

#include <type_traits>
#include <assert.h>
#include <memory>
#include <variant>
#include <type_magic.hpp>

namespace nstd {

enum OptionStatus
{
    SOME,
    NONE,
};
template <typename SomeType>
class Option
{
private:
    struct Placeholder{};
    OptionStatus m_status;
    std::variant<Placeholder, SomeType> value;
public:
    Option(SomeType&& some) : m_status(OptionStatus::SOME), value(std::forward<SomeType>(some)){}
    Option(SomeType& some) : m_status(OptionStatus::SOME), value(std::forward<SomeType>(some)){}
    Option(): m_status(OptionStatus::NONE), value() {}
    Option(const Option<SomeType>& option) : m_status(option.m_status)
    {
        if(OptionStatus::SOME == m_status)
        {
            value = option.value;
        }
        else
        {
            value = std::variant<Placeholder, SomeType>{};
        }
    }
    Option(Option<SomeType>&& option) : m_result(option.m_status)
    {
        if(OptionStatus::SOME == m_status)
        {
            value = std::move(option.value);
        }
        else
        {
            value = std::variant<Placeholder, SomeType>{};
        }
    }
    inline OptionStatus status() const { return m_status; }
    inline bool is_some() const noexcept { return OptionStatus::SOME == m_status; }
    inline bool is_none() const noexcept
    {
        return OptionStatus::NONE == m_status;
    }
    std::add_const_t<std::add_lvalue_reference_t<SomeType>> operator*() const
    {
        assert(OptionStatus::SOME == m_status);
        return std::get<SomeType>(value);
    }
    std::add_lvalue_reference_t<SomeType>  operator*()
    {
        assert(OptionStatus::SOME == m_status);
        return std::get<SomeType>(value);
    }
    std::add_const_t<std::add_pointer_t<SomeType>> operator->() const
    {
        assert(OptionStatus::SOME == m_status);
        return value.get_if<SomeType>();
    }
    std::add_pointer_t<SomeType> operator->()
    {
        assert(OptionStatus::SOME == m_status);
        return value.get_if<SomeType>();
    }
};

} // namespace nstd

#endif
