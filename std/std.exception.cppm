export module std.exception;

import std.type.fundamental;
import std.basic_string;
import std.cow_string;

export namespace std {

class exception
{
public:
    exception() noexcept;
    exception(const exception& that) noexcept;
    exception& operator=(const exception& that) noexcept;
    virtual ~exception();
    virtual const char* what() const noexcept;
};

class logic_error : public exception
{
public:
    logic_error(const string& what_arg);
    logic_error(const char* what_arg);
    logic_error(const logic_error& that) noexcept;
    logic_error& operator=(const logic_error& that) noexcept;
    const char* what() const noexcept override;
private:
    cow_string message;
};

class invalid_argument : public logic_error
{
public:
    invalid_argument(const string& what_arg);
    invalid_argument(const char* what_arg);
    invalid_argument(const invalid_argument& that) noexcept;
};

class domain_error : public logic_error
{
public:
    domain_error(const string& what_arg);
    domain_error(const char* what_arg);
    domain_error(const domain_error& that) noexcept;
};

class length_error : public logic_error
{
public:
    length_error(const string& what_arg);
    length_error(const char* what_arg);
    length_error(const length_error& that) noexcept;
};

class out_of_range : public logic_error
{
public:
    out_of_range(const string& what_arg);
    out_of_range(const char* what_arg);
    out_of_range(const out_of_range& that) noexcept;
};

class runtime_error : public exception
{
public:
    runtime_error(const string& what_arg);
    runtime_error(const char* what_arg);
    runtime_error(const runtime_error& that) noexcept;
    runtime_error& operator=(const runtime_error& that) noexcept;
    const char* what() const noexcept override;
private:
    cow_string message;
};

class bad_alloc : public exception
{
public:
    bad_alloc();
    bad_alloc(const bad_alloc& that) noexcept;
    bad_alloc& operator=(const bad_alloc& that) noexcept;
    const char* what() const noexcept override;
};

} // namespace std
