module std.exception;

import std.eh;
import std.rt;

namespace std {

exception::exception() 
{
}

exception::exception(const exception& that) 
{
}

exception::~exception()
{
}

exception& exception::operator=(const exception& that) 
{
    return *this;
}

const char* exception::what() const
{
    return "std::exception";
}

logic_error::logic_error(const string& what_arg) : exception(), message(what_arg)
{
}

logic_error::logic_error(const char* what_arg) : exception(), message(what_arg)
{
}
    
logic_error::logic_error(const logic_error& that) : exception(that), message(that.message)
{
}

logic_error& logic_error::operator=(const logic_error& that) : exception(that)
{
    try
    {
        message = that.message;
    }
    catch (...)
    {
    }
    return *this;
}

const char* logic_error::what() const
{
    return message.c_str();
}

invalid_argument::invalid_argument(const string& what_arg) : logic_error(what_arg)
{
}

invalid_argument::invalid_argument(const char* what_arg) : logic_error(what_arg)
{
}

invalid_argument::invalid_argument(const invalid_argument& that) : logic_error(that)
{
}

domain_error::domain_error(const string& what_arg) : logic_error(what_arg)
{
}

domain_error::domain_error(const char* what_arg) : logic_error(what_arg)
{
}

domain_error::domain_error(const domain_error& that) : logic_error(that)
{
}

length_error::length_error(const string& what_arg) : logic_error(what_arg)
{
}

length_error::length_error(const char* what_arg) : logic_error(what_arg)
{
}

length_error::length_error(const length_error& that) : logic_error(that)
{
}

out_of_range::out_of_range(const string& what_arg) : logic_error(what_arg)
{
}

out_of_range::out_of_range(const char* what_arg) : logic_error(what_arg)
{
}

out_of_range::out_of_range(const out_of_range& that) : logic_error(that)
{
}

runtime_error::runtime_error(const string& what_arg) : exception(), message(what_arg)
{
}

runtime_error::runtime_error(const char* what_arg) : exception(), message(what_arg)
{
}

runtime_error::runtime_error(const runtime_error& that) : exception(that), message(that.message)
{
}

runtime_error& runtime_error::operator=(const runtime_error& that)
{
    try
    {
        message = that.message;
    }
    catch (...)
    {
    }
    return *this;
}

const char* runtime_error::what() const
{
    return message.c_str();
}

bad_alloc::bad_alloc() : exception()
{
}

bad_alloc::bad_alloc(const bad_alloc& that) : exception(that)
{
}

bad_alloc& bad_alloc::operator=(const bad_alloc& that) 
{
    return *this;
}

const char* bad_alloc::what() const
{
    return "std::bad_alloc";
}

} // namespace std
