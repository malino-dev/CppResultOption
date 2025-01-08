//
// Created by user1 on 03/12/2024.
//

#ifndef OPTION_MATCHER_H
#define OPTION_MATCHER_H

namespace m24
{

template<typename T>
class Option;

template<typename T, typename R, typename SomeFunctor>
class OptionMatcher
{
private:
    Option<T> const& _option;
    SomeFunctor const& _mapSome;

public:
    OptionMatcher(Option<T> const& option, SomeFunctor const& mapSome)
        : _option(option),
          _mapSome(mapSome)
    {
    }

    template<typename NoneFunctor>
    R None(NoneFunctor mapNone)
    {
        if (_option.IsSome()) return _mapSome(_option.UnwrapUnchecked());

        return mapNone();
    }
};

} // namespace m24

#endif // OPTION_MATCHER_H
