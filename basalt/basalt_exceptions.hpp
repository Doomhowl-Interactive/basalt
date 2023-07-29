#pragma once

#include <string>
#include <exception>

class BasaltException : public std::exception {
   public:
    BasaltException(const std::string& msg);
    ~BasaltException();

    virtual const char* what() const throw();

    const std::string message;
};