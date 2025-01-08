//
// Created by user1 on 28/11/2024.
//

#ifndef OPTION_NONE_H
#define OPTION_NONE_H

namespace m24
{

template<typename T>
class Option;

}

namespace m24::Prelude
{

class OptionNone
{
public:
    template<typename T>
    operator Option<T>()
    {
        return Option<T>();
    }
};

} // namespace m24::Prelude

#endif // OPTION_NONE_H
