import std;
import util;
import soul.xml.dom;
import soul.xml.document;
import soul.xml.parser;
import soul.xml.content_handler;
import soul.xml.dom_parser;
import soul.lexer;
import soul.ast.source_pos;
import soul.xml.processor;

std::string Version()
{
    return "0.1.0";
}

void PrintHelp()
{
    std::cout << "XML parser tester version " << Version() << "\n";
    std::cout << "Usage: xml_parser_test [options] { FILE.xml }" << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--sax | -s" << "\n";
    std::cout << "  Use SAX-like content handler." << "\n";
    std::cout << "--empty | -e" << "\n";
    std::cout << "  Empty content handler." << "\n";
}

class PrintContentHandler : public soul::xml::XmlContentHandler
{
public:
    void StartDocument(const soul::ast::SourcePos& sourcePos) override;
    void EndDocument() override;
    void XmlVersion(const std::string& xmlVersion) override;
    void XmlEncoding(const std::string& xmlEncoding) override;
    void Standalone(bool standalone) override;
    void Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment) override;
    void PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data) override;
    void Text(const soul::ast::SourcePos& sourcePos, const std::string& text) override;
    void CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata) override;
    void StartElement(const soul::ast::SourcePos& sourcePos, const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName,
        const soul::xml::processor::Attributes& attributes) override;
    void EndElement(const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName) override;
    void SkippedEntity(const soul::ast::SourcePos& sourcePos, const std::string& entityName) override;
};

void PrintContentHandler::StartDocument(const soul::ast::SourcePos& sourcePos)
{
    std::cout << "START_DOCUMENT(line=" << std::to_string(sourcePos.line) << ")" << "\n";
}

void PrintContentHandler::EndDocument()
{
    std::cout << "END_DOCUMENT" << "\n";
}

void PrintContentHandler::XmlVersion(const std::string& xmlVersion)
{
    std::cout << "XML_VERSION(version='" << xmlVersion << "')" << "\n";
}

void PrintContentHandler::XmlEncoding(const std::string& xmlEncoding)
{
    std::cout << "XML_ENCODING(encoding='" << xmlEncoding << "')" << "\n";
}

void PrintContentHandler::Standalone(bool standalone)
{
    std::cout << "XML_STANDALONE(standalone=" << (standalone ? "'yes'" : "'no'") << "')" << "\n";
}

void PrintContentHandler::Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment)
{
    std::cout << "COMMENT(comment='" << util::StringStr(comment) << "', line=" << std::to_string(sourcePos.line) << ")" << "\n";
}

void PrintContentHandler::PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data)
{
    std::cout << "PI(target='" << target << "', data='" << data + "', line=" << std::to_string(sourcePos.line) << ")" << "\n";
}

void PrintContentHandler::Text(const soul::ast::SourcePos& sourcePos, const std::string& text)
{
    std::cout << "TEXT(text='" << util::StringStr(text) + "', line=" << std::to_string(sourcePos.line) << ")" << "\n";
}

void PrintContentHandler::CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata)
{
    std::cout << "CDATA_SECTION(cdata='" << util::StringStr(cdata) << "', line=" << std::to_string(sourcePos.line) << ")" << "\n";
}

void PrintContentHandler::StartElement(const soul::ast::SourcePos& sourcePos, const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName,
    const soul::xml::processor::Attributes& attributes)
{
    std::cout << "START_ELEMENT(namespaceURI='" << namespaceUri << "', localName='" << localName << "', qualifiedName='" << qualifiedName << "', attributes=" <<
        attributes.ToString() << ", line=" << std::to_string(sourcePos.line) << ")" << "\n";
}

void PrintContentHandler::EndElement(const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName)
{
    std::cout << "END_ELEMENT(namespaceURI='" << namespaceUri << "', localName='" << localName << "', qualifiedName='" << qualifiedName << "')" << "\n";
}

void PrintContentHandler::SkippedEntity(const soul::ast::SourcePos& sourcePos, const std::string& entityName)
{
    std::cout << "SKIPPED_ENTITY(entityName='" << entityName << "', line=" << std::to_string(sourcePos.line) << ")" << "\n";
}

int Action(int argc, const char** argv)
{
    bool verbose = false;
    bool lexer = false;
    bool lexerless = false;
    bool sax = false;
    bool empty = false;
    bool warnings = false;
    std::vector<std::string> fileNames;
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
            else if (arg == "--sax")
            {
                sax = true;
            }
            else if (arg == "--empty")
            {
                empty = true;
            }
            else if (arg == "--warnings")
            {
                warnings = true;
            }
            else
            {
                throw std::runtime_error("unknown option '" + arg + "'");
            }
        }
        else if (arg.starts_with("-"))
        {
            std::string options = arg.substr(1);
            bool unknown_opt = false;
            char opt = ' ';
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
                    case 's':
                    {
                        sax = true;
                        break;
                    }
                    case 'e':
                    {
                        empty = true;
                        break;
                    }
                    case 'w':
                    {
                        warnings = true;
                        break;
                    }
                    default:
                    {
                        unknown_opt = true;
                        opt = o;
                        break;
                    }
                }
            }
            if (unknown_opt)
            {
                throw std::runtime_error("unknown option '-" + std::string(1, opt) + "'");
            }
        }
        else
        {
            fileNames.push_back(util::GetFullPath(arg));
        }
    }
    if (fileNames.empty())
    {
        throw std::runtime_error("no files given");
    }
    for (const auto& fileName : fileNames)
    {
        if (verbose)
        {
            std::cout << "> " << fileName << "\n";
        }
        if (sax)
        {
            soul::xml::XmlContentHandler emptyHandler;
            soul::xml::XmlContentHandler* contentHandler = &emptyHandler;
            PrintContentHandler printContentHandler;
            if (!empty)
            {
                contentHandler = &printContentHandler;
            }
            soul::xml::parser::ParseXmlFile(fileName, contentHandler);
        }
        else
        {
            soul::xml::ParsingFlags flags = soul::xml::ParsingFlags::none;
            if (warnings)
            {
                flags = flags | soul::xml::ParsingFlags::generateWarnings;
            }
            std::unique_ptr<soul::xml::Document> document = soul::xml::ParseXmlFile(fileName, flags);
            if (!empty)
            {
                util::CodeFormatter formatter(std::cout);
                formatter.SetIndentSize(2);
                document->Write(formatter);
            }
        }
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
