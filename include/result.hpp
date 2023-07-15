#ifndef __NSTD_RESULT_HPP__
#define __NSTD_RESULT_HPP__

#include <type_traits>
#include <match.hpp>

namespace nstd {

    enum Status
    {
        OK,
        REPORTED_ERR,
        UNREPORTED_ERR,
    };
    template<typename OkType, typename ErrType>
    class Result: public ApplyMatchTrait<Result<OkType, ErrType>, Status>
    {
        private:
        Status m_status;
        union ResultValue
        {
            OkType ok_value;
            ErrType err_value;
            ResultValue(OkType&& ok, std::true_type)
            {
                new(&ok_value) OkType(std::forward<OkType>(ok));
            }
            ResultValue(ErrType&& err, std::false_type)
            {
                new(&err_value) ErrType(std::forward<OkType>(err));
            }
            ResultValue(const Result<OkType, ErrType>& oe)
            {
                switch (oe.m_status)
                {
                case Status::OK:
                    new(&ok_value) OkType(oe.m_result.ok_value);
                    break;
                case Status::REPORTED_ERR:
                case Status::UNREPORTED_ERR:
                    new(&err_value) ErrType(oe.m_result.err_value);
                    break;
                default:
                    break;
                }
            }
            ResultValue(Result<OkType, ErrType>&& oe)
            {
                switch (oe.m_status)
                {
                case Status::OK:
                    new(&ok_value) OkType(std::move(oe.m_result.ok_value));
                    break;
                case Status::REPORTED_ERR:
                case Status::UNREPORTED_ERR:
                    new(&err_value) ErrType(std::move(oe.m_result.err_value));
                    break;
                default:
                    break;
                }
            }
            ~ResultValue(){}
        } m_result;
        Result(OkType&& ok_value): m_result(std::forward<OkType>(ok_value), std::true_type())
        {
            m_status = Status::OK;
        }
        Result(Status s, ErrType&& err_value): m_result(std::forward<ErrType>(err_value), std::false_type())
        {
            m_status = s;
        }
        public:
        Result(const Result<OkType, ErrType>& res): m_result(res)
        {
            m_status = res.m_status;
        }
        Result(Result<OkType, ErrType>&& res): m_result(std::move(res))
        {
            m_status = res.m_status;
        }
        inline Status status()
        {
            return m_status;
        }
        inline bool is_ok()
        {
            return Status::OK == m_status;
        }
        inline bool is_err()
        {
            return Status::REPORTED_ERR == m_status || Status::UNREPORTED_ERR == m_status;
        }
        inline bool is_reported_err()
        {
            return Status::REPORTED_ERR == m_status;
        }
        inline bool is_unreported_err()
        {
            return Status::UNREPORTED_ERR == m_status;
        }

        operator bool()
        {
            switch (m_status)
            {
            case Status::OK:
                return false;
            case Status::REPORTED_ERR:
            case Status::UNREPORTED_ERR:
                return true;
            default:
                break;
            }
        }
        operator Status()
        {
            return m_status;
        }
        static Result<OkType, ErrType> ok(OkType&& value)
        {
            return Result(std::forward<OkType>(value));
        }
        static Result<OkType, ErrType> reported_err(ErrType&& value)
        {
            return Result(Status::REPORTED_ERR, std::forward<ErrType>(value));
        }
        static Result<OkType, ErrType> unreported_err(ErrType&& value)
        {
            return Result(Status::UNREPORTED_ERR, std::forward<ErrType>(value));
        }
        ~Result()
        {
            // The ~OkType() and ~ErrType() are ok while OkType or ErrType is scalar type.(I use clang++ 15.0.2 for testing.)
            switch (m_status)
            {
            case Status::OK:
                m_result.ok_value.~OkType();
                break;
            case Status::REPORTED_ERR:
            case Status::UNREPORTED_ERR:
                m_result.err_value.~ErrType();
            default:
                break;
            }
        }
    };
}

#endif
