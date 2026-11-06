// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

import std;
import otava.profiler.run;
import util.text_util;

std::string Version()
{
    return "0.3.3";
}

constexpr int percentField = 0;
constexpr int timeField = 1;
constexpr int countField = 2;
constexpr int workField = 3;
constexpr int idField = 4;
constexpr int nameField = 5;

void PrintHelp()
{
    std::cout << "otava profiler version " << Version() << "\n";
    std::cout << "usage: op [options] { key=value }" << "\n";
    std::cout << "options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "keys and values:" << "\n";
    std::cout << "file=FILE" << "\n";
    std::cout << "  Read profile from file FILE." << "\n";
    std::cout << "  If file not given, read profile from the last profile.XXX.txt file in the current directory." << "\n";
    std::cout << "sort=FIELD[:asc|:desc] where FIELD is percent, time, count, work, id or name" << "\n";
    std::cout << "  Sort profile with respect to FIELD value either ascending or descending order." << "\n";
    std::cout << "  If sort not given, sort profile with respect to time field in descending order." << "\n";
    std::cout << "filter=PATTERN" << "\n";
    std::cout << "  Filter profile with respect to name pattern PATTERN, where PATTERN can contain wild cards * and ?." << "\n";
    std::cout << "  Filter is applied to the name field of the profiled functions." << "\n";
    std::cout << "  If filter not given, print all elements." << "\n";
    std::cout << "count=COUNT" << "\n";
    std::cout << "  Print at most COUNT elements." << "\n";
    std::cout << "  If count not given, print all elements after applying possible filter." << "\n";
}

int main(int argc, const char** argv)
{
    try
    {
        std::map<std::string, std::string> commandMap;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--help")
                {
                    PrintHelp();
                    return 0;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with('-'))
            {
                std::string options = arg.substr(1);
                for (char c : options)
                {
                    switch (c)
                    {
                    case 'h':
                    {
                        PrintHelp();
                        return 0;
                    }
                    default:
                    {
                        throw std::runtime_error("unknown option '-" + arg + "'");
                    }
                    }
                }
            }
            else if (arg.find('=') != std::string::npos)
            {
                std::vector<std::string> components = util::Split(arg, '=');
                if (components.size() == 2)
                {
                    commandMap[components[0]] = components[1];
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else
            {
                throw std::runtime_error("unknown option '" + arg + "'");
            }
        }
        otava::profiler::run(commandMap);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}