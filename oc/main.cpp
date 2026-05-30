// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

import std;
import otava.build.parser;
import otava.build.build;
import otava.build.config;
import otava.build_project;
import otava.build_solution;
import otava.parser.recorded_parse;
import otava.parser.expr;
import otava.parser.stmt;
import otava.parser.decl_specifier_seq;
import otava.symbols.expr_parser;
import otava.symbols.stmt_parser;
import otava.symbols.decl_specifier_seq_parser;
import otava.symbols.exception;
import soul.lexer.file_map;
import util.init_done;
import util.path;
import util.text_util;

std::string Version()
{
    return "0.1.0";
}

void PrintHelp()
{
    std::cout << "usage: oc [options] { FILE.project | FILE.solution }" << "\n";
    std::cout << "options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--config=(debug|release|trace|CONFIG) | -c=(debug|release|trace|CONFIG)" << "\n";
    std::cout << "  Set configuration to build to 'debug', 'release', 'trace' or user defined configuration CONFIG." << "\n";
    std::cout << "  Default configuration is 'debug'." << "\n";
    std::cout << "--opt=OPTIMIZATION_LEVEL | -O=OPTIMIZATION_LEVEL" << "\n";
    std::cout << "  Set release mode optimization level to OPTIMIZATION_LEVEL (0-3)" << "\n";
    std::cout << "--define=SYMBOL | -d=SYMBOL" << "\n";
    std::cout << "  Define build symbol SYMBOL." << "\n";
    std::cout << "--rebuild | -r" << "\n";
    std::cout << "  Rebuild project or solution." << "\n";
    std::cout << "--all | -a" << "\n";
    std::cout << "  Build all dependent projects." << "\n";
    std::cout << "--multithreaded | -m" << "\n";
    std::cout << "  Build using all cores." << "\n";
    std::cout << "--debug | -b" << "\n";
    std::cout << "  Set compile debugging on." << "\n";
    std::cout << "--out=FILENAME | -o=FILENAME" << "\n";
    std::cout << "  Write compile debug output to file FILENAME." << "\n";
    std::cout << "--opt-fn=FN | -f=FN" << "\n";
    std::cout << "  Write optimization stages for function FN to --ort-out file." << "\n";
}

int main(int argc, const char** argv)
{
    try
    {
        std::cout << "****************************************" << "\n";
        std::cout << "* Otava C++ Compiler version " << Version() << "\n";
        std::cout << "****************************************" << "\n";
        util::Init();
        otava::parser::recorded::parse::Init();
        otava::symbols::SetExprParser(otava::parser::ParseExpression);
        otava::symbols::SetStmtParser(otava::parser::ParseStatement);
        otava::symbols::SetDeclarationSpecifierSequenceParser(otava::parser::ParseDeclarationSpecifierSequence);
        soul::lexer::FileMap fileMap;
        std::vector<std::string> files;
        bool verbose = false;
        bool rebuild = false;
        bool debug = false;
        std::string config = "debug";
        bool multithreaded = false;
        bool debugParse = false;
        bool setSeed = false;
        bool xml = false;
        bool symbolXml = false;
        bool all = false;
        int optLevel = -1;
        std::ofstream outFileStream;
        std::ostream* outFile = nullptr;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--config")
                        {
                            config = components[1];
                        }
                        else if (components[0] == "--opt")
                        {
                            optLevel = std::stoi(components[1]);
                        }
                        else if (components[0] == "--define")
                        {
                            otava::build::DefineSymbol(components[1]);
                        }
                        else if (components[0] == "--out")
                        {
                            outFileStream.open(components[1]);
                            outFile = &outFileStream;
                        }
                        else if (components[0] == "--opt-fn")
                        {
                            //otava::optimizer::SetOptFn(components[1]);
                        }
                        else
                        {
                            otava::symbols::SetExceptionThrown();
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                    else
                    {
                        otava::symbols::SetExceptionThrown();
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
                else
                {
                    if (arg == "--help")
                    {
                        PrintHelp();
                        return 1;
                    }
                    else if (arg == "--verbose")
                    {
                        verbose = true;
                    }
                    else if (arg == "--rebuild")
                    {
                        rebuild = true;
                    }
                    else if (arg == "--multithreaded")
                    {
                        multithreaded = true;
                    }
                    else if (arg == "--debug-parse")
                    {
                        debugParse = true;
                    }
                    else if (arg == "--seed")
                    {
                        setSeed = true;
                    }
                    else if (arg == "--xml")
                    {
                        xml = true;
                    }
                    else if (arg == "--symbol-xml")
                    {
                        symbolXml = true;
                    }
                    else if (arg == "--all")
                    {
                        all = true;
                    }
                    else if (arg == "--debug")
                    {
                        debug = true;
                    }
                    else
                    {
                        otava::symbols::SetExceptionThrown();
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
            }
            else if (arg.starts_with("-"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "-c")
                        {
                            config = components[1];
                        }
                        else if (components[0] == "-O")
                        {
                            optLevel = std::stoi(components[1]);
                        }
                        else if (components[0] == "-d")
                        {
                            otava::build::DefineSymbol(components[1]);
                        }
                        else if (components[0] == "-o")
                        {
                            outFileStream.open(components[1]);
                            outFile = &outFileStream;
                        }
                        else if (components[0] == "-f")
                        {
                            //otava::optimizer::SetOptFn(components[1]);
                        }
                        else
                        {
                            otava::symbols::SetExceptionThrown();
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                    else
                    {
                        otava::symbols::SetExceptionThrown();
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
                else
                {
                    std::string options = arg.substr(1);
                    for (char o : options)
                    {
                        switch (o)
                        {
                        case 'h':
                        {
                            PrintHelp();
                            return 1;
                        }
                        case 'v':
                        {
                            verbose = true;
                            break;
                        }
                        case 'r':
                        {
                            rebuild = true;
                            break;
                        }
                        case 'a':
                        {
                            all = true;
                            break;
                        }
                        case 'm':
                        {
                            multithreaded = true;
                            break;
                        }
                        case 'p':
                        {
                            debugParse = true;
                            break;
                        }
                        case 'x':
                        {
                            xml = true;
                            break;
                        }
                        case 's':
                        {
                            setSeed = true;
                            break;
                        }
                        case 'y':
                        {
                            symbolXml = true;
                            break;
                        }
                        case 'b':
                        {
                            debug = true;
                            break;
                        }
                        default:
                        {
                            otava::symbols::SetExceptionThrown();
                            throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                        }
                        }
                    }
                }
            }
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        if (files.empty())
        {
            otava::symbols::SetExceptionThrown();
            throw std::runtime_error("no files given");
        }
        if (optLevel != -1)
        {
            //otava::optimizer::Optimizer::Instance().SetOptimizations(std::to_string(optLevel));
        }
        for (const auto& file : files)
        {
            if (verbose)
            {
                std::cout << "> " << file << std::endl;
            }
            if (file.ends_with(".project"))
            {
                std::unique_ptr<otava::build::Project> project = otava::build::ParseProjectFile(file);
                project->SetFileMap(&fileMap);
                otava::build::BuildFlags buildFlags = otava::build::BuildFlags::none;
                if (verbose)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::verbose;
                }
                if (rebuild)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::rebuild;
                }
                if (all)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::all;
                }
                if (multithreaded)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::multithreadedBuild;
                }
                if (debugParse)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::debugParse;
                }
                if (xml)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::xml;
                }
                if (symbolXml)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::symbolXml;
                }
                if (setSeed)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::seed;
                }
                if (debug)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::debug;
                }
                std::set<otava::build::Project*, otava::build::ProjectLess> projectSet;
                otava::build::Build(fileMap, project.get(), config, optLevel, buildFlags, projectSet, outFile);
            }
            else if (file.ends_with(".solution"))
            {
                std::unique_ptr<otava::build::Solution> solution = otava::build::ParseSolutionFile(file);
                otava::build::BuildFlags buildFlags = otava::build::BuildFlags::none;
                if (verbose)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::verbose;
                }
                if (rebuild)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::rebuild;
                }
                if (all)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::all;
                }
                if (multithreaded)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::multithreadedBuild;
                }
                if (xml)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::xml;
                }
                if (symbolXml)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::symbolXml;
                }
                if (setSeed)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::seed;
                }
                if (debug)
                {
                    buildFlags = buildFlags | otava::build::BuildFlags::debug;
                }
                std::set<otava::build::Project*, otava::build::ProjectLess> projectSet;
                otava::build::Build(fileMap, solution.get(), config, optLevel, buildFlags, projectSet, outFile);
            }
            else
            {
                otava::symbols::SetExceptionThrown();
                throw std::runtime_error("file '" + file + "' has invalid extension: not .project or .solution");
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    util::Done();
}
