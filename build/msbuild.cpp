// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef _WIN32
#include <Windows.h>
#endif

module otava.build.msbuild;

import otava.build.config;
import otava.symbols.exception;
import util.path;
import util.code_formatter;
import util.file_stream;
import util.unicode;

namespace otava::build {

#ifdef _WIN32

void MSBuild(const std::string& projectFilePath, const std::string& config, const std::set<std::string>& configurations)
{
    std::string vcvars64Path = GetVCVars64Path();
    std::string configFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::OtavaRoot(), "config"), "otava.config"));
    if (vcvars64Path.empty())
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("vcvars64.bat not configured, see configuration file '" + configFilePath + "'");
    }
    if (!util::FileExists(vcvars64Path))
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("'vcvars64.bat' not found from path '" + vcvars64Path + "', see configuration file '" + configFilePath + "'");
    }
    std::string Config = "Debug";
    if (configurations.find("release") != configurations.end())
    {
        Config = "Release";
    }
    std::string projectDir = util::Path::GetDirectoryName(projectFilePath);
    std::string buildBatPath = util::GetFullPath(util::Path::Combine(projectDir, "build.bat"));
    {
        std::ofstream buildBatFile(buildBatPath);
        if (!buildBatFile)
        {
            otava::symbols::SetExceptionThrown();
            throw std::runtime_error("could not create file '" + buildBatPath + "'");
        }
        util::CodeFormatter formatter(buildBatFile);
        formatter.WriteLine("@echo off");
        formatter.WriteLine("call \"" + vcvars64Path + "\"");
        formatter.WriteLine("msbuild -verbosity:minimal -t:Rebuild -p:Configuration=\"" + Config + "\" " + "\"" + projectFilePath + "\"" + " > \"" + projectDir +
            "/build.out.log\" 2> \"" + projectDir + "/build.error.log\"");
        formatter.WriteLine("exit %ERRORLEVEL%");
    }
    std::string commandLine = "cmd /C \"" + buildBatPath + "\"";
    STARTUPINFOA startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(processInfo));
    bool succeeded = CreateProcessA(NULL, (LPSTR)commandLine.c_str(), NULL, NULL, false, 0, NULL, NULL, &startupInfo, &processInfo);
    if (!succeeded)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("could not run build.bat");
    }
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    DWORD buildExitCode = 0;
    GetExitCodeProcess(processInfo.hProcess, &buildExitCode);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    if (util::FileExists(projectDir + "/build.out.log"))
    {
        std::string buildOutLog = util::ReadFile(projectDir + "/build.out.log");
        std::cout << buildOutLog << std::endl;
    }
    if (util::FileExists(projectDir + "/build.error.log"))
    {
        std::string buildErrorLog = util::ReadFile(projectDir + "/build.error.log");
        if (!buildErrorLog.empty())
        {
            std::cout << buildErrorLog << std::endl;
        }
    }
    if (buildExitCode != 0)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("msbuild failed with exit code " + std::to_string(buildExitCode));
    }
}

#endif

} // namespace otava::build
