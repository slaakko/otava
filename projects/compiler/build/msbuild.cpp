module otava.build.msbuild;

import otava.build.config;
import otava.symbols.exception;
import util.path;
import util.code_formatter;
import util.file_stream;
import util.process;

namespace otava::build {

void MSBuild(const std::string& projectFilePath, const std::string& config, const std::set<std::string>& configurations)
{
    std::string vcvars64Path = GetVCVars64Path();
    std::string configFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::SoulRoot(), "config"), "otava.config"));
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
    int exitCode = util::RunProcess(commandLine.c_str());
    if (util::FileExists(projectDir + "/build.out.log"))
    {
        std::string buildOutLog = util::ReadFile(projectDir + "/build.out.log");
        std::cout << buildOutLog << "\n";
    }
    if (util::FileExists(projectDir + "/build.error.log"))
    {
        std::string buildErrorLog = util::ReadFile(projectDir + "/build.error.log");
        if (!buildErrorLog.empty())
        {
            std::cout << buildErrorLog << "\n";
        }
    }
    if (exitCode != 0)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("msbuild failed with exit code " + std::to_string(exitCode));
    }
}

} // namespace otava::build
