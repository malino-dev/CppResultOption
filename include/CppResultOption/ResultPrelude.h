//
// Created by user1 on 28/11/2024.
//

#ifndef RESULT_PRELUDE_H
#define RESULT_PRELUDE_H

#include "ResultErr.h"
#include "ResultOk.h"

namespace m24::Prelude
{

#pragma region Simple Ok
template<typename T>
ResultOk<T> Ok(T const& value)
{
    return ResultOk<T>{value};
}

template<typename T>
ResultOk<T> Ok(T&& value)
    requires(std::is_rvalue_reference_v<T &&>)
{
    return ResultOk<T>{std::move(value)};
}
#pragma endregion

#pragma region Fully templated Ok
template<typename T, typename E>
Result<T, E> Ok(T const& value)
{
    return Result<T, E>{OkTag, value};
}

template<typename T, typename E>
Result<T, E> Ok(T&& value)
    requires(std::is_rvalue_reference_v<T &&>)
{
    return Result<T, E>{OkTag, std::move(value)};
}
#pragma endregion

#pragma region Simple Err
template<typename E>
ResultErr<E> Err(E const& value)
{
    return ResultErr<E>{value};
}

template<typename E>
ResultErr<E> Err(E&& value)
    requires(std::is_rvalue_reference_v<E &&>)
{
    return ResultErr<E>{std::move(value)};
}
#pragma endregion

#pragma region Fully templated Err
template<typename T, typename E>
Result<T, E> Err(E const& value)
{
    return Result<T, E>{ErrTag, value};
}

template<typename T, typename E>
Result<T, E> Err(E&& value)
    requires(std::is_rvalue_reference_v<E &&>)
{
    return Result<T, E>{ErrTag, std::forward<T>(value)};
}
#pragma endregion

} // namespace m24::Prelude

#endif // RESULT_PRELUDE_H
