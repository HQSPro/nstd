#ifndef __NSTD_RESULT_HPP__
#define __NSTD_RESULT_HPP__

#include <type_traits>
#include <assert.h>
#include <type_magic.hpp>

namespace nstd {

enum ResultStatus
{
    OK,
    REPORTED_ERR,
    UNREPORTED_ERR,
};
template <typename OkType, typename ErrType>
class Result
{
private:
    ResultStatus m_status;
    union ResultValue
    {
        OkType ok_value;
        ErrType err_value;
        ResultValue(OkType&& ok, std::true_type)
        {
            new(&ok_value) OkType(std::forward<OkType>(ok));
        }
        ResultValue(OkType& ok, std::true_type)
        {
            new(&ok_value) OkType(std::forward<OkType>(ok));
        }
        ResultValue(ErrType&& err, std::false_type)
        {
            new(&err_value) ErrType(std::forward<OkType>(err));
        }
        ResultValue(ErrType& err, std::false_type)
        {
            new(&err_value) ErrType(std::forward<OkType>(err));
        }
        ResultValue(const Result<OkType, ErrType>& oe)
        {
            switch(oe.m_status)
            {
            case ResultStatus::OK: new(&ok_value) OkType(oe.m_result.ok_value); break;
            case ResultStatus::REPORTED_ERR:
            case ResultStatus::UNREPORTED_ERR: new(&err_value) ErrType(oe.m_result.err_value); break;
            default: break;
            }
        }
        ResultValue(Result<OkType, ErrType>&& oe)
        {
            switch(oe.m_status)
            {
            case ResultStatus::OK: new(&ok_value) OkType(std::move(oe.m_result.ok_value)); break;
            case ResultStatus::REPORTED_ERR:
            case ResultStatus::UNREPORTED_ERR:
                new(&err_value) ErrType(std::move(oe.m_result.err_value));
                break;
            default: break;
            }
        }
        ~ResultValue() {}
    } m_result;
    Result(OkType&& ok_value) : m_result(std::forward<OkType>(ok_value), std::true_type())
    {
        m_status = ResultStatus::OK;
    }
    Result(OkType& ok_value) : m_result(std::forward<OkType>(ok_value), std::true_type())
    {
        m_status = ResultStatus::OK;
    }
    Result(ResultStatus s, ErrType&& err_value)
        : m_result(std::forward<ErrType>(err_value), std::false_type())
    {
        m_status = s;
    }
    Result(ResultStatus s, ErrType& err_value)
        : m_result(std::forward<ErrType>(err_value), std::false_type())
    {
        m_status = s;
    }

public:
    Result(const Result<OkType, ErrType>& res) : m_result(res) { m_status = res.m_status; }
    Result(Result<OkType, ErrType>&& res) : m_result(std::move(res)) { m_status = res.m_status; }
    inline ResultStatus status() { return m_status; }
    inline bool is_ok() const noexcept { return ResultStatus::OK == m_status; }
    inline bool is_err() const noexcept
    {
        return ResultStatus::REPORTED_ERR == m_status || ResultStatus::UNREPORTED_ERR == m_status;
    }
    inline bool is_reported_err() const noexcept { return ResultStatus::REPORTED_ERR == m_status; }
    inline bool is_unreported_err() const noexcept { return ResultStatus::UNREPORTED_ERR == m_status; }
    const typename std::add_lvalue_reference<OkType>::type get(nstd::Int<static_cast<int>(ResultStatus::OK)>) const
    {
        assert(ResultStatus::OK == m_status);
        return m_result.ok_value;
    }
    const typename std::add_lvalue_reference<ErrType>::type
    get(nstd::Int<static_cast<int>(ResultStatus::UNREPORTED_ERR)>) const
    {
        assert(ResultStatus::UNREPORTED_ERR == m_status);
        return m_result.err_value;
    }
    const typename std::add_lvalue_reference<ErrType>::type
    get(nstd::Int<static_cast<int>(ResultStatus::REPORTED_ERR)>) const
    {
        assert(ResultStatus::REPORTED_ERR == m_status);
        return m_result.err_value;
    }
    typename std::add_lvalue_reference<OkType>::type get(nstd::Int<static_cast<int>(ResultStatus::OK)>)
    {
        assert(ResultStatus::OK == m_status);
        return m_result.ok_value;
    }
    typename std::add_lvalue_reference<ErrType>::type
    get(nstd::Int<static_cast<int>(ResultStatus::UNREPORTED_ERR)>)
    {
        assert(ResultStatus::UNREPORTED_ERR == m_status);
        return m_result.err_value;
    }
    typename std::add_lvalue_reference<ErrType>::type
    get(nstd::Int<static_cast<int>(ResultStatus::REPORTED_ERR)>)
    {
        assert(ResultStatus::REPORTED_ERR == m_status);
        return m_result.err_value;
    }
    operator ResultStatus() const noexcept { return m_status; }
    static Result<OkType, ErrType> ok(OkType&& value)
    {
        return Result(std::forward<OkType>(value));
    }
    static Result<OkType, ErrType> ok(OkType& value)
    {
        return Result(std::forward<OkType>(value));
    }
    static Result<OkType, ErrType> reported_err(ErrType&& value)
    {
        return Result(ResultStatus::REPORTED_ERR, std::forward<ErrType>(value));
    }
    static Result<OkType, ErrType> reported_err(ErrType& value)
    {
        return Result(ResultStatus::REPORTED_ERR, std::forward<ErrType>(value));
    }
    static Result<OkType, ErrType> unreported_err(ErrType&& value)
    {
        return Result(ResultStatus::UNREPORTED_ERR, std::forward<ErrType>(value));
    }
    static Result<OkType, ErrType> unreported_err(ErrType& value)
    {
        return Result(ResultStatus::UNREPORTED_ERR, std::forward<ErrType>(value));
    }
    ~Result()
    {
        // The ~OkType() and ~ErrType() are ok while OkType or ErrType is scalar type.(I use
        // clang++ 15.0.2 for testing.)
        switch(m_status)
        {
        case ResultStatus::OK: m_result.ok_value.~OkType(); break;
        case ResultStatus::REPORTED_ERR:
        case ResultStatus::UNREPORTED_ERR: m_result.err_value.~ErrType(); break;
        default: break;
        }
    }
};

} // namespace nstd

#endif
