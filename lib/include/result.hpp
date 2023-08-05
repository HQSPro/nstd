#ifndef __NSTD_RESULT_HPP__
#define __NSTD_RESULT_HPP__

#include <type_traits>
#include "variant"
#include "type_magic.hpp"

namespace nstd {

enum ResultStatus
{
    OK = 0,
    REPORTED_ERR,
    UNREPORTED_ERR,
};

template <typename OkType>
class Ok {
private:
    OkType value;

public:
    constexpr Ok(std::add_rvalue_reference_t<OkType> v) : value(std::forward<OkType>(v)) {}
    constexpr Ok(std::add_const_t<std::add_lvalue_reference_t<OkType>> v) : value(v) {}
    constexpr Ok(Ok&& ok) : value(std::move(ok.value)) {}
    constexpr Ok(const Ok& ok) : value(ok.value) {}
    inline constexpr std::add_const_t<std::add_lvalue_reference_t<OkType>> operator*() const { return value; }
    inline constexpr std::add_lvalue_reference_t<OkType> operator*() { return value; }
    inline constexpr std::add_const_t<std::add_pointer_t<OkType>> operator->() const { return &value; }
    inline constexpr std::add_pointer_t<OkType> operator->() { return &value; }
};
template <typename ErrType>
class Err {
private:
    ErrType value;

public:
    constexpr Err(std::add_rvalue_reference_t<ErrType> v) : value(std::forward<ErrType>(v)) {}
    constexpr Err(std::add_const_t<std::add_lvalue_reference_t<ErrType>> v) : value(v) {}
    constexpr Err(Err&& err) : value(std::move(err.value)) {}
    constexpr Err(const Err& err) : value(err.value) {}
    inline constexpr std::add_const_t<std::add_lvalue_reference_t<ErrType>> operator*() const { return value; }
    inline constexpr std::add_lvalue_reference_t<ErrType> operator*() { return value; }
    inline constexpr std::add_const_t<std::add_pointer_t<ErrType>> operator->() const { return &value; }
    inline constexpr std::add_pointer_t<ErrType> operator->() { return &value; }
};

template <typename OkType, typename ErrType>
class Result {
private:
    ResultStatus m_status;
    std::variant<Ok<OkType>, Err<ErrType>> m_result;

    constexpr Result(std::add_rvalue_reference_t<OkType> ok_value)
        : m_status(ResultStatus::OK), m_result(std::in_place_type_t<Ok<OkType>>{}, std::forward<OkType>(ok_value))
    {
    }
    constexpr Result(std::add_const_t<std::add_lvalue_reference_t<OkType>> ok_value)
        : m_status(ResultStatus::OK), m_result(std::in_place_type_t<Ok<OkType>>{}, std::forward<OkType>(ok_value))
    {
    }
    constexpr Result(ResultStatus s, std::add_rvalue_reference_t<ErrType> err_value)
        : m_status(s), m_result(std::in_place_type_t<Err<ErrType>>{}, std::forward<ErrType>(err_value))
    {
    }
    constexpr Result(ResultStatus s, std::add_const_t<std::add_lvalue_reference_t<ErrType>> err_value)
        : m_status(s), m_result(std::in_place_type_t<Err<ErrType>>{}, std::forward<ErrType>(err_value))
    {
    }

public:
    constexpr Result(const Result<OkType, ErrType>& res) : m_status(res.m_status), m_result(res.m_result) {}
    constexpr Result(Result<OkType, ErrType>&& res) : m_status(res.m_status), m_result(std::move(res.m_result)) {}
    inline constexpr ResultStatus status() const noexcept { return m_status; }
    inline constexpr bool is_ok() const noexcept { return ResultStatus::OK == m_status; }
    inline constexpr bool is_err() const noexcept { return ResultStatus::REPORTED_ERR == m_status || ResultStatus::UNREPORTED_ERR == m_status; }
    inline constexpr bool is_reported_err() const noexcept { return ResultStatus::REPORTED_ERR == m_status; }
    inline constexpr bool is_unreported_err() const noexcept { return ResultStatus::UNREPORTED_ERR == m_status; }
    inline constexpr operator ResultStatus() const noexcept { return m_status; }
    inline constexpr std::add_const_t<std::add_lvalue_reference_t<std::variant<Ok<OkType>, Err<ErrType>>>> operator*() const { return m_result; }
    inline constexpr std::add_lvalue_reference_t<std::variant<Ok<OkType>, Err<ErrType>>> operator*() { return m_result; }
    inline constexpr std::add_const_t<std::add_pointer_t<std::variant<Ok<OkType>, Err<ErrType>>>> operator->() const { return &m_result; }
    inline constexpr std::add_pointer_t<std::variant<Ok<OkType>, Err<ErrType>>> operator->() { return &m_result; }
    inline constexpr static Result<OkType, ErrType> ok(std::remove_reference_t<OkType>&& value) { return Result(std::forward<OkType>(value)); }
    inline constexpr static Result<OkType, ErrType> ok(std::add_const_t<std::remove_reference_t<OkType>>& value) { return Result(std::forward<OkType>(value)); }
    inline constexpr static Result<OkType, ErrType> reported_err(std::remove_reference_t<ErrType>&& value)
    {
        return Result(ResultStatus::REPORTED_ERR, std::forward<ErrType>(value));
    }
    inline constexpr static Result<OkType, ErrType> reported_err(std::add_const_t<std::remove_reference_t<ErrType>>& value)
    {
        return Result(ResultStatus::REPORTED_ERR, std::forward<ErrType>(value));
    }
    inline constexpr static Result<OkType, ErrType> unreported_err(std::remove_reference_t<ErrType>&& value)
    {
        return Result(ResultStatus::UNREPORTED_ERR, std::forward<ErrType>(value));
    }
    inline constexpr static Result<OkType, ErrType> unreported_err(std::add_const_t<std::remove_reference_t<ErrType>>& value)
    {
        return Result(ResultStatus::UNREPORTED_ERR, std::forward<ErrType>(value));
    }
};

}  // namespace nstd

namespace std {
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_lvalue_reference_t<nstd::type_pack_element_t<I, OkType, ErrType, ErrType>> get(nstd::Result<OkType, ErrType>& r)
{
    if constexpr(I == 2) { return *std::get<I - 1>(*r); }
    else { return *std::get<I>(*r); }
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_rvalue_reference_t<nstd::type_pack_element_t<I, OkType, ErrType, ErrType>> get(nstd::Result<OkType, ErrType>&& r)
{
    if constexpr(I == 2) { return std::move(*std::get<I - 1>(std::move(*r))); }
    else { return std::move(*std::get<I>(std::move(*r))); }
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_const_t<std::add_lvalue_reference_t<nstd::type_pack_element_t<I, OkType, ErrType, ErrType>>> get(const nstd::Result<OkType, ErrType>& r)
{
    if constexpr(I == 2) { return *std::get<I - 1>(*r); }
    else { return *std::get<I>(*r); }
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_const_t<std::add_rvalue_reference_t<nstd::type_pack_element_t<I, OkType, ErrType, ErrType>>> get(const nstd::Result<OkType, ErrType>&& r)
{
    if constexpr(I == 2) { return std::move(*std::get<I - 1>(std::move(*r))); }
    else { return std::move(*std::get<I>(std::move(*r))); }
}

template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_pointer_t<nstd::type_pack_element_t<I, OkType, ErrType, ErrType>> get_if(nstd::Result<OkType, ErrType>* r)
{
    if constexpr(I == 2) { return &**std::get_if<I - 1>(&**r); }
    else { return &**std::get_if<I>(&**r); }
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_const_t<std::add_pointer_t<nstd::type_pack_element_t<I, OkType, ErrType, ErrType>>> get_if(const nstd::Result<OkType, ErrType>* r)
{
    if constexpr(I == 2) { return &**std::get_if<I - 1>(&**r); }
    else { return &**std::get_if<I>(&**r); }
}

}  // namespace std

#endif
