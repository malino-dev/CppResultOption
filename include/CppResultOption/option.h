//
// Created by user1 on 27/11/2024.
//

#ifndef OPTION_H
#define OPTION_H

#include "option_matcher.h"
#include "option_prelude.h"
#include "result.h"
#include "some_expected_exception.h"

#include <cassert>
#include <optional>

namespace m24
{

template<typename T, typename R, typename OkFunctor>
class OptionMatcher;

template<typename T>
class Option;

namespace internal
{
    template<typename T>
    class OptionBase
    {
    private:
        std::optional<T> _value;

    public:
#pragma region Constructors
        explicit OptionBase(T const& value)
            : _value(value)
        {
        }

        explicit OptionBase(T&& value)
            requires(std::is_rvalue_reference_v<T &&>)
            : _value(std::move(value))
        {
        }

        explicit OptionBase()
            : _value(std::nullopt)
        {
        }

        // TODO: assignment operator (maybe copy-and-swap)

        virtual ~OptionBase() = default;
#pragma endregion

#pragma region And
        template<typename R>
        Option<R> And(Option<R> const& other) const noexcept
        {
            if (IsNone()) return Prelude::None;

            return other;
        }

        template<typename R, typename Functor>
        Option<R> AndThen(Functor&& other) const noexcept
        {
            if (IsNone()) return Prelude::None;

            return other(UnwrapUnchecked());
        }
#pragma endregion

#pragma region Expect
        T const& Expect(std::string const& message) const
        {
            if (IsNone()) throw SomeExpectedException(message);

            return UnwrapUnchecked();
        }
#pragma endregion

#pragma region Filter
        template<typename Predicate>
        Option<T> Filter(Predicate predicate) const noexcept
        {
            // None  -> None
            // Ok(_) -> predicate(_) ? Ok(_) : None

            if (IsNone()) return Prelude::None;
            if (predicate(UnwrapUnchecked())) return Prelude::Some(UnwrapUnchecked());

            return Prelude::None;
        }
#pragma endregion

#pragma region Inspect
        template<typename Action>
        void Inspect(Action action) const noexcept
        {
            if (IsNone()) return;

            action(UnwrapUnchecked());
        }
#pragma endregion

#pragma region IsNone
        [[nodiscard]] bool IsNone() const noexcept
        {
            const bool result = !_value.has_value();
            assert(result == !IsSome());
            return result;
        }

        template<typename Predicate>
        [[nodiscard]] bool IsNoneOr(Predicate predicate) const noexcept
        {
            if (IsNone()) return true;

            return predicate(UnwrapUnchecked());
        }
#pragma endregion

#pragma region IsSome
        [[nodiscard]] bool IsSome() const noexcept
        {
            return _value.has_value();
        }

        template<typename Predicate>
        [[nodiscard]] bool IsSomeAnd(Predicate functor) const noexcept
        {
            if (IsNone()) return false;

            return functor(UnwrapUnchecked());
        }
#pragma endregion

#pragma region Map
        template<typename R, typename Functor>
        Option<R> Map(Functor&& functor) const noexcept
        {
            if (IsNone()) return Prelude::None;

            return Prelude::Some(functor(UnwrapUnchecked()));
        }

        template<typename R, typename Functor>
        Option<R> MapOr(R const& defaultValue, Functor&& functor) const noexcept
        {
            if (IsNone()) return Prelude::Some(defaultValue);

            return Prelude::Some(functor(UnwrapUnchecked()));
        }

        template<typename R, typename NoneFunctor, typename SomeFunctor>
        Option<R> MapOrElse(NoneFunctor&& defaultValue, SomeFunctor&& functor) const noexcept
        {
            if (IsNone()) return Prelude::Some(defaultValue());

            return Prelude::Some(functor(UnwrapUnchecked()));
        }
#pragma endregion

#pragma region Match
        template<typename R, typename OkFunctor, typename NoneFunctor>
        R Match(OkFunctor&& mapOk, NoneFunctor&& mapNone) const noexcept
        {
            if (IsNone()) return mapNone();

            return mapOk(UnwrapUnchecked());
        }

        template<typename R, typename SomeFunctor>
        OptionMatcher<T, R, SomeFunctor> Some(SomeFunctor&& mapSome) const noexcept
        {
            return OptionMatcher<T, R, SomeFunctor>(*this, mapSome);
        }
#pragma endregion

#pragma region OkOr
        template<typename E>
        Result<T, E> OkOr(T const& rhs) const noexcept
        {
            if (IsSome()) return Prelude::Ok(UnwrapUnchecked());

            return Prelude::Err(rhs);
        }

        template<typename E, typename Functor>
        Result<T, E> OkOrElse(Functor&& mapErr) const noexcept
        {
            if (IsSome()) return Prelude::Ok(UnwrapUnchecked());

            return Prelude::Err(mapErr());
        }
#pragma endregion

#pragma region Or
        Option<T> Or(Option<T> const& rhs) const noexcept
        {
            if (IsSome()) return Prelude::Some(UnwrapUnchecked());

            return rhs;
        }

        template<typename Functor>
        Option<T> OrElse(Functor&& rhs) const noexcept
        {
            if (IsSome()) return Prelude::Some(UnwrapUnchecked());

            return rhs;
        }
#pragma endregion

#pragma region Replace
        Option<T> Replace(T const& value) noexcept
        {
            if (IsNone())
            {
                _value.emplace(value);
                return Prelude::None;
            }

            T oldValue = std::move(UnwrapUnchecked());
            _value.emplace(value);
            return Prelude::Some(std::move(oldValue));
        }
#pragma endregion

#pragma region Take
        Option<T> Take() noexcept
        {
            if (IsNone()) return Prelude::None;

            Option<T> result = Prelude::Some(std::move(UnwrapUnchecked()));
            _value = std::nullopt;

            return result;
        }

        template<typename Predicate>
        Option<T> TakeIf(Predicate&& predicate) noexcept
        {
            if (IsNone()) return Prelude::None;

            T& value = UnwrapUnchecked();
            if (predicate(value)) return Take();

            return Prelude::None;
        }
#pragma endregion

#pragma region Unwrap
        T const& Unwrap() const
        {
            if (IsNone()) throw SomeExpectedException();

            return UnwrapUnchecked();
        }
#pragma endregion

#pragma region UnwrapOr
        T const& UnwrapOr(T const& defaultValue) const noexcept
        {
            if (IsNone()) return defaultValue;

            return UnwrapUnchecked();
        }
#pragma endregion

#pragma region UnwrapUnchecked (Private)
    private:
        T const& UnwrapUnchecked() const noexcept
        {
            return *_value;
        }

        T& UnwrapUnchecked() noexcept
        {
            return *_value;
        }
#pragma endregion

#pragma region Xor
    public:
        Option<T> Xor(Option<T> const& other) const noexcept
        {
            if (IsNone()) return other;
            if (other.IsSome()) return Prelude::None;
            return Prelude::Some(UnwrapUnchecked());
        }
#pragma endregion

#pragma region Zip
        template<typename U>
        Option<std::pair<T, U>> Zip(Option<U> const& other) const noexcept
        {
            if (IsNone() || other.IsNone()) return Prelude::None;

            T const& value1 = UnwrapUnchecked();
            U const& value2 = other.UnwrapUnchecked();

            return Prelude::Some(std::make_pair(value1, value2));
        }

        template<typename R, typename U, typename Functor>
        Option<R> ZipWith(Option<U> const& other, Functor f) const noexcept
        {
            if (IsNone() || other.IsNone()) return Prelude::None;

            T const& value1 = UnwrapUnchecked();
            U const& value2 = other.UnwrapUnchecked();
            R zipped = f(value1, value2);

            return Prelude::Some(std::move(zipped));
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
            return IsSome();
        }

        Option<T> operator&(Option<T> const& other) const noexcept
        {
            return And(other);
        }

        Option<T> operator|(Option<T> const& other) const noexcept
        {
            return Or(other);
        }

        Option<T> operator^(Option<T> const& other) const noexcept
        {
            return Xor(other);
        }

        bool operator==(Option<T> const& other) const noexcept
        {
            if (IsNone() && other.IsNone()) return true;
            if (IsNone() || other.IsNone()) return false;
            return UnwrapUnchecked() == other.UnwrapUnchecked();
        }

        bool operator==(Prelude::OptionNone const& other) const noexcept
        {
            return IsNone();
        }

        bool operator!=(Option<T> const& other) const noexcept
        {
            if (IsNone() && other.IsNone()) return false;
            if (IsNone() || other.IsNone()) return true;
            return UnwrapUnchecked() != other.UnwrapUnchecked();
        }

        bool operator!=(Prelude::OptionNone const& other) const noexcept
        {
            return IsSome();
        }
#pragma endregion
    };
}

template<typename T>
class Option final : public internal::OptionBase<T>
{
public:
    explicit Option(T const& value)
        : internal::OptionBase<T>(value)
    {
    }

    explicit Option(T&& value)
        requires(std::is_rvalue_reference_v<T &&>)
        : internal::OptionBase<T>(std::move(value))
    {
    }

    explicit Option()
        : internal::OptionBase<T>()
    {
    }
};

// TODO: implement Option<Option<T>>

} // namespace m24

#endif // OPTION_H
