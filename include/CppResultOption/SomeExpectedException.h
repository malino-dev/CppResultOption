//
// Created by user1 on 02/12/2024.
//

#ifndef SOME_EXPECTED_EXCEPTION_H
#define SOME_EXPECTED_EXCEPTION_H

#include <stdexcept>

namespace m24
{

class SomeExpectedException : public std::runtime_error
{
public:
    SomeExpectedException()
        : std::runtime_error("Some(_) expected, None found")
    {
    }

    SomeExpectedException(std::string const& message)
        : std::runtime_error(message)
    {
    }
};

} // namespace m24

#endif // SOME_EXPECTED_EXCEPTION_H
