//
// Created by user1 on 28/11/2024.
//

#ifndef RESULT_ERR_H
#define RESULT_ERR_H

#include "ResultTags.h"

namespace m24
{

template<typename T, typename E>
class Result;

template<typename E>
class ResultErr
{
private:
    E _value;

public:
    explicit ResultErr(E const& value)
        : _value(value)
    {
    }

    explicit ResultErr(E&& value)
        requires(std::is_rvalue_reference_v<E &&>)
        : _value(value)
    {
    }

    template<typename T>
    operator Result<T, E>()
    {
        return Result<T, E>(ErrTag, std::move(_value));
    }
};

} // namespace m24


#endif // RESULT_ERR_H
