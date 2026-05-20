// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef _WIN32
#include <Windows.h>
#endif

module util.process;

namespace util {

int RunProcess(const char* commandLine)
{
    STARTUPINFOA startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(processInfo));
    bool succeeded = CreateProcessA(NULL, (LPSTR)commandLine, NULL, NULL, false, 0, NULL, NULL, &startupInfo, &processInfo);
    if (!succeeded)
    {
        throw std::runtime_error("could not run process '" + std::string(commandLine) + "'");
    }
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeProcess(processInfo.hProcess, &exitCode);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    return exitCode;
}

} // namespace util
