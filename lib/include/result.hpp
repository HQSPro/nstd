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
    Ok(nstd::add_rvalue_reference_t<OkType> v) : value(std::move(v)) {}
    Ok(nstd::add_const_t<nstd::add_lvalue_reference_t<OkType>> v) : value(v) {}
    Ok(Ok<OkType>&& ok) : value(std::move(ok.value)) {}
    Ok(const Ok<OkType>& ok) : value(ok.value) {}
    Ok<OkType>& operator=(Ok<OkType>&&)      = default;
    Ok<OkType>& operator=(const Ok<OkType>&) = default;
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
    Err(nstd::add_rvalue_reference_t<ErrType> v) : value(nstd::move(v)) {}
    Err(nstd::add_const_t<nstd::add_lvalue_reference_t<ErrType>> v) : value(v) {}
    Err(Err<ErrType>&& err) : value(std::move(err.value)) {}
    Err(const Err<ErrType>& err) : value(err.value) {}
    Err<ErrType>& operator=(Err<ErrType>&&)      = default;
    Err<ErrType>& operator=(const Err<ErrType>&) = default;
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

    Result(nstd::add_rvalue_reference_t<OkType> ok_value)
        : m_status(ResultStatus::OK), nstd::variant<Ok<OkType>, Err<ErrType>>(
                                          nstd::in_place_type<Ok<OkType>>, std::move(ok_value))
    {
    }
    Result(nstd::add_const_t<nstd::add_lvalue_reference_t<OkType>> ok_value)
        : m_status(ResultStatus::OK), nstd::variant<Ok<OkType>, Err<ErrType>>(
                                          nstd::in_place_type<Ok<OkType>>, ok_value)
    {
    }
    Result(ResultStatus s, nstd::add_rvalue_reference_t<ErrType> err_value)
        : m_status(s), nstd::variant<Ok<OkType>, Err<ErrType>>(nstd::in_place_type<Err<ErrType>>,
                                                               std::move(err_value))
    {
    }
    Result(ResultStatus s, nstd::add_const_t<nstd::add_lvalue_reference_t<ErrType>> err_value)
        : m_status(s), nstd::variant<Ok<OkType>, Err<ErrType>>(nstd::in_place_type<Err<ErrType>>,
                                                               err_value)
    {
    }

public:
    Result(const Result<OkType, ErrType>& res)
        : m_status(res.m_status), nstd::variant<Ok<OkType>, Err<ErrType>>(res)
    {
    }
    Result(Result<OkType, ErrType>&& res)
        : m_status(res.m_status), nstd::variant<Ok<OkType>, Err<ErrType>>(std::move(res))
    {
    }
    Result<OkType, ErrType>& operator=(Result<OkType, ErrType>&&)      = default;
    Result<OkType, ErrType>& operator=(const Result<OkType, ErrType>&) = default;
    inline constexpr ResultStatus status() const noexcept { return m_status; }
    inline constexpr bool is_ok() const noexcept { return ResultStatus::OK == m_status; }
    inline constexpr bool is_err() const noexcept { return ResultStatus::ERR == m_status; }
    inline constexpr operator ResultStatus() const noexcept { return m_status; }
    inline static Result<OkType, ErrType> ok(nstd::add_rvalue_reference_t<OkType> value)
    {
        return Result(std::forward<OkType>(value));
    }
    inline static Result<OkType, ErrType>
    ok(nstd::add_const_t<nstd::add_lvalue_reference_t<OkType>> value)
    {
        return Result(std::forward<OkType>(value));
    }
    inline static Result<OkType, ErrType> err(nstd::add_rvalue_reference_t<ErrType> value)
    {
        return Result(ResultStatus::ERR, std::forward<ErrType>(value));
    }
    inline static Result<OkType, ErrType>
    err(nstd::add_const_t<nstd::add_lvalue_reference_t<ErrType>> value)
    {
        return Result(ResultStatus::ERR, std::forward<ErrType>(value));
    }
};

template <typename E>
using ResultOmitOk = nstd::Result<nstd::monostate, E>;

}  // namespace nstd

#endif
