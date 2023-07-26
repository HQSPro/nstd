#ifndef __NSTD_RESULT_HPP__
#define __NSTD_RESULT_HPP__

#include <type_traits>
#include <variant>
#include <type_magic.hpp>

namespace nstd {

enum ResultStatus
{
    OK,
    REPORTED_ERR,
    UNREPORTED_ERR,
};

template <typename OkType>
class Ok
{
private:
    OkType value;

public:
    Ok(std::add_rvalue_reference_t<OkType> v) : value(std::forward<OkType>(v)) {}
    Ok(std::add_const_t<std::add_lvalue_reference_t<OkType>> v) : value(v) {}
    Ok(Ok&& ok) : value(std::move(ok.value)) {}
    Ok(const Ok& ok) : value(ok.value) {}
    std::add_const_t<std::add_lvalue_reference_t<OkType>> operator*() const { return value; }
    std::add_lvalue_reference_t<OkType> operator*() { return value; }
    std::add_const_t<std::add_pointer_t<OkType>> operator->() const { return &value; }
    std::add_pointer_t<OkType> operator->() { return &value; }
};
template <typename ErrType>
class Err
{
private:
    ErrType value;

public:
    Err(std::add_rvalue_reference_t<ErrType> v) : value(std::forward<ErrType>(v)) {}
    Err(std::add_const_t<std::add_lvalue_reference_t<ErrType>> v) : value(v) {}
    Err(Err&& err) : value(std::move(err.value)) {}
    Err(const Err& err) : value(err.value) {}
    std::add_const_t<std::add_lvalue_reference_t<ErrType>> operator*() const { return value; }
    std::add_lvalue_reference_t<ErrType> operator*() { return value; }
    std::add_const_t<std::add_pointer_t<ErrType>> operator->() const { return &value; }
    std::add_pointer_t<ErrType> operator->() { return &value; }
};

template <typename OkType, typename ErrType>
class Result
{
private:
    ResultStatus m_status;
    std::variant<Ok<OkType>, Err<ErrType>> m_result;

    Result(std::add_rvalue_reference_t<OkType> ok_value)
        : m_status(ResultStatus::OK),
          m_result(std::in_place_type_t<Ok<OkType>>{}, std::forward<OkType>(ok_value))
    {
    }
    Result(std::add_const_t<std::add_lvalue_reference_t<OkType>> ok_value)
        : m_status(ResultStatus::OK),
          m_result(std::in_place_type_t<Ok<OkType>>{}, std::forward<OkType>(ok_value))
    {
    }
    Result(ResultStatus s, std::add_rvalue_reference_t<ErrType> err_value)
        : m_status(s),
          m_result(std::in_place_type_t<Err<ErrType>>{}, std::forward<ErrType>(err_value))
    {
    }
    Result(ResultStatus s, std::add_const_t<std::add_lvalue_reference_t<ErrType>> err_value)
        : m_status(s),
          m_result(std::in_place_type_t<Err<ErrType>>{}, std::forward<ErrType>(err_value))
    {
    }

public:
    Result(const Result<OkType, ErrType>& res) : m_status(res.m_status), m_result(res.m_result) {}
    Result(Result<OkType, ErrType>&& res)
        : m_status(res.m_status), m_result(std::move(res.m_result))
    {
    }
    inline ResultStatus status() const noexcept { return m_status; }
    inline bool is_ok() const noexcept { return ResultStatus::OK == m_status; }
    inline bool is_err() const noexcept
    {
        return ResultStatus::REPORTED_ERR == m_status || ResultStatus::UNREPORTED_ERR == m_status;
    }
    inline bool is_reported_err() const noexcept { return ResultStatus::REPORTED_ERR == m_status; }
    inline bool is_unreported_err() const noexcept
    {
        return ResultStatus::UNREPORTED_ERR == m_status;
    }
    operator ResultStatus() const noexcept { return m_status; }
    std::add_const_t<std::add_lvalue_reference_t<std::variant<Ok<OkType>, Err<ErrType>>>>
    operator*() const
    {
        return m_result;
    }
    std::add_lvalue_reference_t<std::variant<Ok<OkType>, Err<ErrType>>> operator*()
    {
        return m_result;
    }
    std::add_const_t<std::add_pointer_t<std::variant<Ok<OkType>, Err<ErrType>>>> operator->() const
    {
        return &m_result;
    }
    std::add_pointer_t<std::variant<Ok<OkType>, Err<ErrType>>> operator->() { return &m_result; }
    inline static Result<OkType, ErrType> ok(std::remove_reference_t<OkType>&& value)
    {
        return Result(std::forward<OkType>(value));
    }
    inline static Result<OkType, ErrType>
    ok(std::add_const_t<std::remove_reference_t<OkType>>& value)
    {
        return Result(std::forward<OkType>(value));
    }
    inline static Result<OkType, ErrType> reported_err(std::remove_reference_t<ErrType>&& value)
    {
        return Result(ResultStatus::REPORTED_ERR, std::forward<ErrType>(value));
    }
    inline static Result<OkType, ErrType>
    reported_err(std::add_const_t<std::remove_reference_t<ErrType>>& value)
    {
        return Result(ResultStatus::REPORTED_ERR, std::forward<ErrType>(value));
    }
    inline static Result<OkType, ErrType> unreported_err(std::remove_reference_t<ErrType>&& value)
    {
        return Result(ResultStatus::UNREPORTED_ERR, std::forward<ErrType>(value));
    }
    inline static Result<OkType, ErrType>
    unreported_err(std::add_const_t<std::remove_reference_t<ErrType>>& value)
    {
        return Result(ResultStatus::UNREPORTED_ERR, std::forward<ErrType>(value));
    }
};

} // namespace nstd

namespace std {
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_lvalue_reference_t<type_pack_element_t<I, nstd::Ok<OkType>, nstd::Err<ErrType>>>
get(nstd::Result<OkType, ErrType>& r)
{
    return std::get<I>(*r);
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_rvalue_reference_t<type_pack_element_t<I, nstd::Ok<OkType>, nstd::Err<ErrType>>>
get(nstd::Result<OkType, ErrType>&& r)
{
    return std::get<I>(std::move(*r));
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_const_t<
    std::add_lvalue_reference_t<type_pack_element_t<I, nstd::Ok<OkType>, nstd::Err<ErrType>>>>
get(const nstd::Result<OkType, ErrType>& r)
{
    return std::get<I>(*r);
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_const_t<
    std::add_rvalue_reference_t<type_pack_element_t<I, nstd::Ok<OkType>, nstd::Err<ErrType>>>>
get(const nstd::Result<OkType, ErrType>&& r)
{
    return std::get<I>(std::move(const_cast<nstd::Result<OkType, ErrType>&>(*r)));
}

template <typename T, typename OkType, typename ErrType>
constexpr std::add_lvalue_reference_t<T> get(nstd::Result<OkType, ErrType>& r)
{
    return std::get<T>(*r);
}
template <typename T, typename OkType, typename ErrType>
constexpr std::add_rvalue_reference_t<T> get(nstd::Result<OkType, ErrType>&& r)
{
    return std::get<T>(std::move(*r));
}
template <typename T, typename OkType, typename ErrType>
constexpr std::add_const_t<std::add_lvalue_reference_t<T>>
get(const nstd::Result<OkType, ErrType>& r)
{
    return std::get<T>(*r);
}
template <typename T, typename OkType, typename ErrType>
constexpr std::add_const_t<std::add_rvalue_reference_t<T>>
get(const nstd::Result<OkType, ErrType>&& r)
{
    return std::get<T>(std::move(const_cast<nstd::Result<OkType, ErrType>&>(*r)));
}

template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_pointer_t<type_pack_element_t<I, nstd::Ok<OkType>, nstd::Err<ErrType>>>
get_if(nstd::Result<OkType, ErrType>* r)
{
    return std::get<I>(&**r);
}
template <std::size_t I, typename OkType, typename ErrType>
constexpr std::add_const_t<
    std::add_pointer_t<type_pack_element_t<I, nstd::Ok<OkType>, nstd::Err<ErrType>>>>
get_if(const nstd::Result<OkType, ErrType>* r)
{
    return std::get<I>(&**r);
}
template <typename T, typename OkType, typename ErrType>
constexpr std::add_pointer_t<T> get_if(nstd::Result<OkType, ErrType>* r)
{
    return std::get<T>(&**r);
}
template <typename T, typename OkType, typename ErrType>
constexpr std::add_const_t<std::add_pointer_t<T>> get_if(const nstd::Result<OkType, ErrType>* r)
{
    return std::get<T>(&**r);
}
} // namespace std

#endif
