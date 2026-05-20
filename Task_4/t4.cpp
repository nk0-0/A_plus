#include <iostream>
#include <cassert>

template <typename T>
class less_than_comparable {
public:
    bool operator>(const T& other) const {
        return other < static_cast<const T&>(*this);
    }
    bool operator<=(const T& other) const {
        return !(other < static_cast<const T&>(*this));
    }
    bool operator>=(const T& other) const {
        return !(static_cast<const T&>(*this) < other);
    }
    bool operator==(const T& other) const {
        return !(static_cast<const T&>(*this) < other) && !(other < static_cast<const T&>(*this));
    }
    bool operator!=(const T& other) const {
        return (static_cast<const T&>(*this) < other) || (other < static_cast<const T&>(*this));
    }
};

template <typename T>
class counter {
private:
    inline static size_t count_ = 0;  

public:
    counter() { ++count_; }                     
    counter(const counter&) { ++count_; }       
    counter(counter&&) noexcept { ++count_; }   
    ~counter() { --count_; }                  
 
    static size_t getCount() { return count_; }
};

class Number: public less_than_comparable<Number>, public counter<Number> {
public:
    Number(int value): m_value{value} {}

    int getM_value() const { return m_value; }

    bool operator<(const Number& other) const {
        return m_value < other.m_value;
    }

private:
    int m_value;
};

int main() {
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};

    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);

    std::cout << "Count: " << counter<Number>::getCount() << std::endl; 
    return 0;
}