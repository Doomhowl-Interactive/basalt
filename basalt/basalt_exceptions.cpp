#include <iostream>
#include "basalt_exceptions.hpp"

using namespace std;

BasaltException::BasaltException(const string& msg) : message(msg)
{
}

BasaltException::~BasaltException()
{
}

const char* BasaltException::what() const throw()
{
    cout << "BasaltException::what" << endl;
    return message.c_str();
}