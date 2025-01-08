//
// Created by user1 on 28/11/2024.
//

#ifndef RESULT2_H
#define RESULT2_H

#include <cassert>
#include <optional>
#include <string>

#include "ok_expected_exception.h"
#include "err_expected_exception.h"

#include "option.h"
#include "option_prelude.h"
#include "result_prelude.h"
#include "result_tags.h"

namespace m24
{

namespace internal
{
    template<typename T, typename E>
    class ResultBase
    {
    private:
        std::optional<T> _okValue;
        std::optional<E> _errValue;

    public:
#pragma region Constructors
        ResultBase(ResultOkTag const&, T const& value)
            : _okValue(value),
              _errValue(std::nullopt)
        {
        }

        ResultBase(ResultOkTag const&, T&& value)
            requires(std::is_rvalue_reference_v<T &&>)
            : _okValue(std::move(value)),
              _errValue(std::nullopt)
        {
        }

        ResultBase(ResultErrTag const&, E const& value)
            : _okValue(std::nullopt),
              _errValue(value)
        {
        }

        ResultBase(ResultErrTag const&, E&& value)
            requires(std::is_rvalue_reference_v<E &&>)
            : _okValue(std::nullopt),
              _errValue(std::move(value))
        {
        }
#pragma endregion

#pragma region And
        template<typename R>
        Result<R, E> And(Result<R, E> const& other) const
        {
            // Ok(_)  -> other
            // Err(_) -> Err(_)

            if (IsOk()) return other;

            return Prelude::Err(UnwrapErrUnchecked());
        }

        template<typename R, typename Functor>
        Result<R, E> AndThen(Functor&& mapOk) const
        {
            // Ok(_)  -> mapOk(_)
            // Err(_) -> Err(_)

            if (IsErr()) return Prelude::Err(UnwrapErrUnchecked());

            return mapOk(UnwrapUnchecked());
        }
#pragma endregion

#pragma region Err
        [[nodiscard]] bool IsErr() const
        {
            // Ok(_)  -> false
            // Err(_) -> true

            bool const result = _errValue.has_value();
            assert(result == !IsOk());
            return result;
        }

        template<typename Predicate>
        bool IsErrAnd(Predicate&& predicate) const
        {
            // Ok(_)  -> false
            // Err(_) -> predicate(_)

            if (IsOk()) return false;

            return predicate(UnwrapErrUnchecked());
        }

        Option<E> Err() const
        {
            // Ok(_)  -> None
            // Err(_) -> Some(_)

            if (IsOk()) return Prelude::None;

            return Prelude::Some(UnwrapErrUnchecked());
        }
#pragma endregion

#pragma region Expect
        T Expect(std::string const& message) const
        {
            if (IsErr()) throw OkExpectedException(message);

            return UnwrapUnchecked();
        }

        E ExpectErr(std::string const& message) const
        {
            if (IsOk()) throw ErrExpectedException(message);

            return UnwrapErrUnchecked();
        }
#pragma endregion

#pragma region Inspect
        template<typename Action>
        void Inspect(Action&& ok) const
        {
            if (IsErr()) return;

            ok(UnwrapUnchecked());
        }

        template<typename Action>
        void InspectErr(Action&& err) const
        {
            if (IsOk()) return;

            err(UnwrapErrUnchecked());
        }

        template<typename OkAction, typename ErrAction>
        void InspectAll(OkAction&& mapOk, ErrAction&& mapErr) const
        {
            if (IsOk())
                mapOk(UnwrapUnchecked());
            else
                mapErr(UnwrapErrUnchecked());
        }
#pragma endregion

#pragma region Map
        template<typename R, typename Functor>
        Result<R, E> Map(Functor&& mapOk) const
        {
            // Ok(_)  -> Ok(mapOk(_))
            // Err(_) -> Err(_)

            if (IsErr()) return Prelude::Err(UnwrapErrUnchecked());

            return Prelude::Ok(mapOk(UnwrapUnchecked()));
        }

        /**
         * @param defaultValue Default value if result is an error.
         * @param mapOk Mapper function to call which has the signature ``R mapOk(T okValue)``.
         * @return Returns mapOk(Unwrap()) if result is T, otherwise defaultValue
         */
        template<typename R, typename Functor>
        R MapOr(R defaultValue, Functor&& mapOk) const
        {
            if (IsErr()) return std::move(defaultValue);

            return mapOk(UnwrapUnchecked());
        }

        template<typename R, typename ErrFunctor, typename OkFunctor>
        R MapOrElse(ErrFunctor&& mapErr, OkFunctor&& mapOk) const
        {
            if (IsErr()) return mapErr(UnwrapErrUnchecked());

            return mapOk(UnwrapUnchecked());
        }

        template<typename R, typename Functor>
        Result<T, R> MapErr(Functor&& mapErr) const
        {
            // Ok(_)  -> Ok(_)
            // Err(_) -> Err(mapErr(_))

            if (IsOk()) return Prelude::Ok(UnwrapUnchecked());

            return Prelude::Err(mapErr(UnwrapErrUnchecked()));
        }
#pragma endregion

#pragma region Match
        template<typename R, typename OkFunctor, typename ErrFunctor>
        R Match(OkFunctor&& mapOk, ErrFunctor&& mapErr) const
        {
            if (IsOk()) return mapOk(UnwrapUnchecked());
            return mapErr(UnwrapErrUnchecked());
        }
#pragma endregion

#pragma region Ok
        [[nodiscard]] bool IsOk() const
        {
            // Ok(_)  -> true
            // Err(_) -> false

            return _okValue.has_value();
        }

        template<typename Predicate>
        bool IsOkAnd(Predicate&& predicate) const
        {
            // Ok(_)  -> predicate(_)
            // Err(_) -> false

            if (IsErr()) return false;

            return predicate(UnwrapUnchecked());
        }

        Option<T> Ok() const
        {
            // Ok(_)  -> Some(_)
            // Err(_) -> None

            if (IsErr()) return Prelude::None;

            return Prelude::Some(UnwrapUnchecked());
        }
#pragma endregion

#pragma region Or
        template<typename R>
        ResultBase<T, R> Or(ResultBase<T, R> const& other) const
        {
            // Ok(_)  -> Ok(_)
            // Err(_) -> other

            if (IsErr()) return other;

            return Prelude::Ok(UnwrapUnchecked());
        }

        template<typename R, typename Functor>
        ResultBase<T, R> OrElse(Functor&& mapErr) const
        {
            // Ok(_)  -> Ok(_)
            // Err(_) -> mapErr(_)

            if (IsOk()) return Prelude::Ok(UnwrapUnchecked());

            return Prelude::Err(mapErr(UnwrapErrUnchecked()));
        }
#pragma endregion

#pragma region Unwrap
        T const& Unwrap() const&
        {
            if (IsErr()) throw OkExpectedException();

            return UnwrapUnchecked();
        }

        T& Unwrap() &
        {
            if (IsErr()) throw OkExpectedException();

            return UnwrapUnchecked();
        }

        T Unwrap() &&
        {
            if (IsErr()) throw OkExpectedException();

            return std::move(*_okValue);
        }

    private:
        T const& UnwrapUnchecked() const&
        {
            return *_okValue;
        }

        T& UnwrapUnchecked()
        {
            return *_okValue;
        }

    public:
        T const& UnwrapOr(T const& defaultValue) const&
        {
            if (IsErr()) return defaultValue;

            return UnwrapUnchecked();
        }

        T UnwrapOr(T const& defaultValue) &&
        {
            if (IsErr()) return defaultValue;

            return std::move(*_okValue);
        }

        template<typename Functor>
        T const& UnwrapOrElse(Functor&& defaultValue) const&
        {
            if (IsErr()) return defaultValue();

            return UnwrapUnchecked();
        }

        template<typename Functor>
        T UnwrapOrElse(Functor&& defaultValue) &&
        {
            if (IsErr()) return defaultValue();

            return std::move(*_okValue);
        }

        E const& UnwrapErr() const&
        {
            if (IsOk()) throw ErrExpectedException();

            return UnwrapErrUnchecked();
        }

        E UnwrapErr() &&
        {
            if (IsOk()) throw ErrExpectedException();

            return std::move(*_errValue);
        }

    private:
        E const& UnwrapErrUnchecked() const
        {
            return *_errValue;
        }
#pragma endregion

#pragma region Operators
        T const& operator*() const
        {
            return Unwrap();
        }

        T const* operator->() const
        {
            return &Unwrap();
        }

        explicit operator T() const
        {
            return Unwrap();
        }

        operator bool() const noexcept
        {
            return IsOk();
        }

        Result<T, E> operator&(Result<T, E> const& other) const noexcept
        {
            return And(other);
        }

        Result<T, E> operator|(Result<T, E> const& other) const noexcept
        {
            return Or(other);
        }

        bool operator==(Result<T, E> const& other) const noexcept
        {
            if (IsOk() && other.IsErr() || IsErr() && other.IsOk()) return false;
            if (IsOk()) return Unwrap() == other.Unwrap();
            return UnwrapErr() == other.UnwrapErr();
        }

        bool operator!=(Result<T, E> const& other) const noexcept
        {
            if (IsOk() && other.IsErr() || IsErr() && other.IsOk()) return true;
            if (IsOk()) return Unwrap() != other.Unwrap();
            return UnwrapErr() != other.UnwrapErr();
        }
#pragma endregion

#pragma region std::cout.operator<<
        friend std::ostream& operator<<(std::ostream& os, ResultBase const& result)
        {
            if (result.IsOk())
            {
                os << "Ok(" << result.UnwrapUnchecked() << ")";
            }
            else
            {
                os << "Err(" << result.UnwrapErrUnchecked() << ")";
            }

            return os;
        }
#pragma endregion
    };
} // namespace internal

template<typename T, typename E = std::runtime_error>
class Result final : public internal::ResultBase<T, E>
{
#pragma region Constructors
public:
    Result(ResultOkTag const& tag, T const& value)
        : internal::ResultBase<T, E>(tag, value)
    {
    }

    Result(ResultOkTag const& tag, T&& value)
        requires(std::is_rvalue_reference_v<T &&>)
        : internal::ResultBase<T, E>(tag, std::move(value))
    {
    }

    Result(ResultErrTag const& tag, E const& err)
        : internal::ResultBase<T, E>(tag, err)
    {
    }

    Result(ResultErrTag const& tag, E&& err)
        requires(std::is_rvalue_reference_v<E &&>)
        : internal::ResultBase<T, E>(tag, std::move(err))
    {
    }
#pragma endregion
};

template<typename T, typename E>
class Result<Option<T>, E> final : public internal::ResultBase<Option<T>, E>
{
public:
#pragma region Constructors
    Result(ResultOkTag tag, T value)
        : internal::ResultBase<Option<T>, E>(tag, value)
    {
    }

    Result(ResultErrTag tag, E err)
        : internal::ResultBase<Option<T>, E>(tag, err)
    {
    }
#pragma endregion

    Option<Result<T, E>> Transpose()
    {
        // Ok(None)    -> None
        // Ok(Some(_)) -> Some(Ok(_))
        // Err(_)      -> Some(Err(_))

        if (this->IsErr()) return Prelude::Some(Prelude::Err(this->UnwrapErrUnchecked()));

        Option<T> ok = this->UnwrapUnchecked();
        if (ok.IsNone()) return Prelude::None;
        return Prelude::Some(Prelude::Ok(ok.UnwrapUnchecked()));
    }
};

template<typename T, typename E>
class Result<Result<T, E>, E> final : public internal::ResultBase<Result<T, E>, E>
{
public:
#pragma region Constructors
    Result(ResultOkTag tag, Result<T, E> value)
        : internal::ResultBase<Result<T, E>, E>(tag, value)
    {
    }

    Result(ResultErrTag tag, E err)
        : internal::ResultBase<Result<T, E>, E>(tag, err)
    {
    }
#pragma endregion

    Result<T, E> Flatten()
    {
        // Ok(Ok(_))  -> Ok(_)
        // Ok(Err(_)) -> Err(_)
        // Err(_)     -> Err(_)

        if (this->IsErr()) return Prelude::Err(this->UnwrapErrUnchecked());

        Result<T, E> ok = this->UnwrapUnchecked();
        return ok;
    }
};

} // namespace m24

#endif // RESULT2_H
