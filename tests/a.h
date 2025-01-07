//
// Created by user1 on 05/12/2024.
//

#ifndef A_H
#define A_H

class A
{
private:
    int _x;
public:
    explicit A(int x) : _x(x)
    {
        std::cout << "A::A(int x)" << std::endl;
    }

    A(A const& rhs) : _x(rhs._x)
    {
        std::cout << "A::A(A const& rhs)" << std::endl;
    }

    A(A&& rhs) : _x(rhs._x)
    {
        std::cout << "A::A(A&& rhs)" << std::endl;
        rhs._x = 0;
    }

    void Swap(A& rhs)
    {
        int temp = rhs._x;
        rhs._x = _x;
        _x = temp;
    }

    A& operator=(A rhs)
    {
        std::cout << "A::operator=(A const& rhs)" << std::endl;
        Swap(rhs);
        return *this;
    }

    int value() const { return _x; }
};

#endif //A_H
