#pragma once

#include <exception>

namespace bacchus {

class Error: public std::exception {
    const char* msg;
public:
    Error(const char* msg): msg(msg) {}
    virtual ~Error() {}
    virtual const char* what() const noexcept override { return msg; }
};

class MathError: public Error {
public:
    MathError(const char* msg): Error(msg) {}
};

class LogicError: public Error {
public:
    LogicError(const char* msg): Error(msg) {}
};

} // namespace bacchus
