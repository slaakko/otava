import std;
import soul.xml.dom;
import soul.xml.xpath;
import soul.lexer;
import soul.xml.processor;
import soul.xml.dom_parser;
import soul.ast;
import util;

std::string Version()
{
    return "0.1.0";
}

void PrintHelp()
{
    std::cout << "XPATH tester version " << Version() << "\n";
    std::cout << "Executes an XPATH expression against an XML file and prints the results." << "\n";
    std::cout << "Usage: xpath_test [options] <xpath_expr> <xml_file_name>" << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
}

int Action(int argc, const char** argv)
{
    bool verbose = false;
    std::string xpathExpr;
    std::string xmlFileName;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg.starts_with("--"))
        {
            if (arg == "--verbose")
            {
                verbose = true;
            }
            else if (arg == "--help")
            {
                PrintHelp();
                return 1;
            }
            else
            {
                throw std::runtime_error("unknown option '" + arg + "'");
            }
        }
        else if (arg.starts_with("-"))
        {
            std::string options = arg.substr(1);
            char opt = ' ';
            bool unknownOpt = false;
            for (char o : options)
            {
                switch (o)
                {
                    case 'v':
                    {
                        verbose = true;
                        break;
                    }
                    case 'h':
                    {
                        PrintHelp();
                        return 1;
                    }
                    default:
                    {
                        unknownOpt = true;
                        opt = o;
                        break;
                    }
                }
            }
            if (unknownOpt)
            {
                throw std::runtime_error("unknown option '-" + std::string(1, opt) + "'");
            }
        }
        else
        {
            if (xpathExpr.empty())
            {
                xpathExpr = arg;
            }
            else if (xmlFileName.empty())
            {
                xmlFileName = arg;
            }
        }
    }
    if (xpathExpr.empty())
    {
        throw std::runtime_error("no XPATH expression given");
    }
    if (xmlFileName.empty())
    {
        throw std::runtime_error("no XML file name given");
    }
    if (verbose)
    {
        std::cout << "> evaluating XPATH expression '" << xpathExpr << "' against XML file '" << xmlFileName << "':" << "\n";
    }
    std::unique_ptr<soul::xml::Document> infoDoc = soul::xml::xpath::EvaluateToXMLInfoDocument(xpathExpr, xmlFileName);
    util::CodeFormatter formatter(std::cout);
    formatter.SetIndentSize(2);
    infoDoc->Write(formatter);
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
    return 0;
}
