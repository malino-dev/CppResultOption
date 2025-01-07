//
// Created by user1 on 28/11/2024.
//

#ifndef OK_EXPECTED_EXCEPTION_H
#define OK_EXPECTED_EXCEPTION_H

#include <stdexcept>

namespace m24
{

class OkExpectedException : public std::runtime_error
{
public:
    OkExpectedException()
        : std::runtime_error("Ok(_) expected, Err(_) found")
    {
    }

    OkExpectedException(std::string const& message)
        : std::runtime_error(message)
    {
    }
};

} // namespace m24

#endif // OK_EXPECTED_EXCEPTION_H
