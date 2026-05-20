// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef _WIN32
#include <Windows.h>
#endif

module util.win_error;

import util.unicode;

namespace util {

std::string WindowsErrorMessage(std::uint64_t errorCode)
{
#ifdef _WIN32
    char16_t buf[4096];
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, LANG_SYSTEM_DEFAULT, (LPWSTR)buf, 4096, nullptr);
    return ToUtf8(buf);
#else
    return std::to_string(errorCode);
#endif
}

WindowsException::WindowsException(std::uint64_t errorCode_) : std::runtime_error(WindowsErrorMessage(errorCode_)), errorCode(errorCode_)
{
}

int ErrorNumber()
{
    return errno;
}

} // util
