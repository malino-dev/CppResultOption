//
// Created by user1 on 28/11/2024.
//

#ifndef RESULT_OK_H
#define RESULT_OK_H

#include "result_tags.h"

namespace m24
{

template<typename T, typename E>
class Result;

template<typename T>
class ResultOk
{
private:
    T _value;

public:
    explicit ResultOk(T const& value)
        : _value(value)
    {
    }

    explicit ResultOk(T&& value)
        requires(std::is_rvalue_reference_v<T &&>)
        : _value(std::move(value))
    {
    }

    template<typename E>
    operator Result<T, E>()
    {
        return Result<T, E>(OkTag, std::move(_value));
    }
};

} // namespace m24

#endif // RESULT_OK_H
