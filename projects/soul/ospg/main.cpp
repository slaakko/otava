import soul.ast.spg;
import soul.spg.file_parsers;
import soul.lexer.file_map;
import soul.spg.parser_generator;
import std;
import util;

std::string Version()
{
    return "0.1.0";
}

void PrintHelp()
{
    std::cout << "Soul Parser Generator for Otava (ospg) version " << Version() << "\n";
    std::cout << "Usage: ospg [options] { FILE.spg }" << "\n";
    std::cout << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "\n";
    std::cout << "--no-debug-support | -n" << "\n";
    std::cout << "  Do not generate parser debugging code." << "\n";
    std::cout << "\n";
    std::cout << "--optimize | -o" << "\n";
    std::cout << "  Do switch optimization." << "\n";
    std::cout << "\n";
    std::cout << "--xml | -x" << "\n";
    std::cout << "  Generate XML info documents." << "\n";
    std::cout << "\n";
}

int Action(int argc, const char** argv)
{
    bool verbose = false;
    bool noDebugSupport = false;
    bool optimize = false;
    bool xml = false;
    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg.starts_with("--"))
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
            else if (arg == "--no-debug-support")
            {
                noDebugSupport = true;
            }
            else if (arg == "--optimize")
            {
                optimize = true;
            }
            else if (arg == "--xml")
            {
                xml = true;
            }
            else
            {
                throw std::runtime_error("unknown option '" + arg + "'");
            }
        }
        else if (arg.starts_with("-"))
        {
            std::string options = arg.substr(1);
            char unknownOpt = ' ';
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
                    case 'n':
                    {
                        noDebugSupport = true;
                        break;
                    }
                    case 'o':
                    {
                        optimize = true;
                        break;
                    }
                    case 'x':
                    {
                        xml = true;
                        break;
                    }
                    default:
                    {
                        unknownOpt = o;
                        break;
                    }
                }
                if (unknownOpt != ' ')
                {
                    throw std::runtime_error("unknown option '-" + std::string(1, unknownOpt) + "'");
                }
            }
        }
        else
        {
            files.push_back(util::GetFullPath(arg));
        }
    }
    soul::lexer::FileMap fileMap;
    for (const std::string& file : files)
    {
        std::unique_ptr<soul::ast::spg::SpgFile> spgFile = soul::spg::ParseSpgFile(file, fileMap, verbose);
        soul::spg::GenerateParsers(spgFile.get(), fileMap, verbose, noDebugSupport, optimize, xml, Version());
    }
    return 0;
}

int main(int argc, const char** argv)
{
    try
    {
        return Action(argc, argv);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
}
