#ifndef __NSTD_RESULT_HPP__
#define __NSTD_RESULT_HPP__

#include "type_traits.hpp"
#include "utility.hpp"
#include "variant.hpp"
#include "option.hpp"

namespace nstd {

struct Display
{
    std::ostream& display(std::ostream&);
};

enum class ResultStatus
{
    OK = 0,
    ERR,
};

template <typename OkType>
class Ok {
private:
    OkType value;

public:
    template<typename... Args>
    Ok(Args&&... args): value(std::forward<Args>(args)...)
    {
    }
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
    template<typename... Args>
    Err(Args&&... args): value(std::forward<Args>(args)...)
    {
    }
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
public:
    Result(nstd::Ok<OkType>&& ok_value)
        : m_status(ResultStatus::OK), nstd::variant<Ok<OkType>, Err<ErrType>>(
                                          std::move(ok_value))
    {
    }
    Result(const nstd::Ok<OkType>& ok_value)
        : m_status(ResultStatus::OK), nstd::variant<Ok<OkType>, Err<ErrType>>(ok_value)
    {
    }
    Result(nstd::Err<ErrType>&& err_value)
        : m_status(ResultStatus::ERR), nstd::variant<Ok<OkType>, Err<ErrType>>(std::move(err_value))
    {
    }
    Result(const nstd::Err<ErrType>& err_value)
        : m_status(ResultStatus::ERR), nstd::variant<Ok<OkType>, Err<ErrType>>(
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
    template<typename... Args>
    inline static ok(Args&&... args)
    {
        return Result(Ok<OkType>(std::forward<Args>(args)...));
    }
    template<typename... Args>
    inline static err(Args&&... args)
    {
        return Result(Err<ErrType>(std::forward<Args>(args)...));
    }
};

template <typename E>
using ResultOmitOk = nstd::Result<nstd::monostate, E>;

struct Error
{
    virtual void context() = 0;
    virtual nstd::Option<std::shared_ptr<Error>> source() {return {};}
    virtual std::ostream& display(std::ostream&) = 0;
};

}  // namespace nstd

#endif
