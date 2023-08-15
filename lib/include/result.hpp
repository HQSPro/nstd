#ifndef __NSTD_RESULT_HPP__
#define __NSTD_RESULT_HPP__

#include "type_traits.hpp"
#include "utility.hpp"
#include "variant.hpp"
#include "option.hpp"

namespace nstd {

enum ResultStatus
{
    OK = 0,
    ERR,
};

template <typename OkType>
class Ok {
private:
    OkType value;

public:
    constexpr Ok(nstd::add_rvalue_reference_t<OkType> v) : value(std::move(v)) {}
    constexpr Ok(nstd::add_const_t<nstd::add_lvalue_reference_t<OkType>> v) : value(v) {}
    constexpr Ok(Ok&& ok) : value(std::move(ok.value)) {}
    constexpr Ok(const Ok& ok) : value(ok.value) {}
    inline constexpr nstd::add_const_t<nstd::add_lvalue_reference_t<OkType>>
    operator*() const noexcept
    {
        return value;
    }
    inline constexpr nstd::add_lvalue_reference_t<OkType> operator*() noexcept { return value; }
    inline constexpr nstd::add_const_t<nstd::add_pointer_t<OkType>> operator->() const noexcept
    {
        return &value;
    }
    inline constexpr nstd::add_pointer_t<OkType> operator->() noexcept { return &value; }
};

template <typename ErrType>
class Err {
private:
    ErrType value;

public:
    constexpr Err(nstd::add_rvalue_reference_t<ErrType> v) : value(nstd::move(v)) {}
    constexpr Err(nstd::add_const_t<nstd::add_lvalue_reference_t<ErrType>> v) : value(v) {}
    constexpr Err(Err&& err) : value(std::move(err.value)) {}
    constexpr Err(const Err& err) : value(err.value) {}
    inline constexpr nstd::add_const_t<nstd::add_lvalue_reference_t<ErrType>>
    operator*() const noexcept
    {
        return value;
    }
    inline constexpr nstd::add_lvalue_reference_t<ErrType> operator*() noexcept { return value; }
    inline constexpr nstd::add_const_t<nstd::add_pointer_t<ErrType>> operator->() const noexcept
    {
        return &value;
    }
    inline constexpr nstd::add_pointer_t<ErrType> operator->() noexcept { return &value; }
};

template <typename OkType, typename ErrType>
class Result : nstd::variant<Ok<OkType>, Err<ErrType>> {
private:
    ResultStatus m_status;

    constexpr Result(nstd::add_rvalue_reference_t<OkType> ok_value)
        : m_status(ResultStatus::OK), nstd::variant<Ok<OkType>, Err<ErrType>>(
                                          nstd::in_place_type<Ok<OkType>>, std::move(ok_value))
    {
    }
    constexpr Result(nstd::add_const_t<nstd::add_lvalue_reference_t<OkType>> ok_value)
        : m_status(ResultStatus::OK), nstd::variant<Ok<OkType>, Err<ErrType>>(
                                          nstd::in_place_type<Ok<OkType>>, ok_value)
    {
    }
    constexpr Result(ResultStatus s, nstd::add_rvalue_reference_t<ErrType> err_value)
        : m_status(s), nstd::variant<Ok<OkType>, Err<ErrType>>(nstd::in_place_type<Err<ErrType>>,
                                                               std::move(err_value))
    {
    }
    constexpr Result(ResultStatus s,
                     nstd::add_const_t<nstd::add_lvalue_reference_t<ErrType>> err_value)
        : m_status(s), nstd::variant<Ok<OkType>, Err<ErrType>>(nstd::in_place_type<Err<ErrType>>,
                                                               err_value)
    {
    }

public:
    constexpr Result(const Result<OkType, ErrType>& res)
        : m_status(res.m_status), nstd::variant<Ok<OkType>, Err<ErrType>>(res)
    {
    }
    constexpr Result(Result<OkType, ErrType>&& res)
        : m_status(res.m_status), nstd::variant<Ok<OkType>, Err<ErrType>>(std::move(res))
    {
    }
    inline constexpr ResultStatus status() const noexcept { return m_status; }
    inline constexpr bool is_ok() const noexcept { return ResultStatus::OK == m_status; }
    inline constexpr bool is_err() const noexcept { return ResultStatus::ERR == m_status; }
    inline constexpr operator ResultStatus() const noexcept { return m_status; }
    inline constexpr static Result<OkType, ErrType> ok(nstd::add_rvalue_reference_t<OkType> value)
    {
        return Result(std::forward<OkType>(value));
    }
    inline constexpr static Result<OkType, ErrType>
    ok(nstd::add_const_t<nstd::add_lvalue_reference_t<OkType>> value)
    {
        return Result(std::forward<OkType>(value));
    }
    inline constexpr static Result<OkType, ErrType> err(nstd::add_rvalue_reference_t<ErrType> value)
    {
        return Result(ResultStatus::ERR, std::forward<ErrType>(value));
    }
    inline constexpr static Result<OkType, ErrType>
    err(nstd::add_const_t<nstd::add_lvalue_reference_t<ErrType>> value)
    {
        return Result(ResultStatus::ERR, std::forward<ErrType>(value));
    }
};

template <typename E>
using ResultOmitOk = nstd::Result<nstd::monostate, E>;

}  // namespace nstd

#endif
