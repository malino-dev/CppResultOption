//
// Created by user1 on 28/11/2024.
//

#ifndef ERR_EXPECTED_EXCEPTION_H
#define ERR_EXPECTED_EXCEPTION_H

#include <stdexcept>

namespace m24
{

class ErrExpectedException : public std::runtime_error
{
public:
    ErrExpectedException()
        : std::runtime_error("Err(_) expected, Ok(_) found")
    {
    }

    ErrExpectedException(std::string const& message)
        : std::runtime_error(message)
    {
    }
};

} // namespace m24

#endif // OK_EXPECTED_EXCEPTION_H
