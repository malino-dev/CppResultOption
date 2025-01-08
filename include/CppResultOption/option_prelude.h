//
// Created by user1 on 28/11/2024.
//

#ifndef OPTION_PRELUDE_H
#define OPTION_PRELUDE_H

#include "option.h"
#include "option_none.h"

namespace m24
{

template<typename T>
class Option;

} // namespace m24

namespace m24::Prelude
{

template<typename T>
Option<T> Some(T const& value)
{
    return Option<T>{value};
}

template<typename T>
Option<T> Some(T&& value)
    requires(std::is_rvalue_reference_v<T &&>)
{
    return Option<T>{std::move(value)};
}

inline OptionNone None;

template<typename T>
Option<T> NoneT()
{
    return Option<T>{};
}

} // namespace m24::Prelude

#endif // OPTION_PRELUDE_H
