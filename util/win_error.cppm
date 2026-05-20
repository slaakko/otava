// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.win_error;

import std;

export namespace util {

class WindowsException : public std::runtime_error
{
public:
    WindowsException(std::uint64_t errorCode_);
    inline std::uint64_t ErrorCode() const { return errorCode; }
    inline std::string ErrorMessage() const { return what(); }
private:
    std::uint64_t errorCode;
};

int ErrorNumber();

} // util
