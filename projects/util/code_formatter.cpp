module util.code_formatter;

import util.unicode;

namespace util {

void CodeFormatter::Write(const std::string& text)
{
    if (atBeginningOfLine)
    {
        if (indent > 0)
        {
            stream << std::string(indentSize * indent, ' ');
            atBeginningOfLine = false;
        }
    }
    if (logging && contentCount > 0)
    {
        stream << "length=" << std::to_string(text.length());
    }
    else
    {
        stream << text;
    }
}

void CodeFormatter::WriteLine(const std::string& text)
{
    Write(text);
    NewLine();
}

void CodeFormatter::NewLine()
{
    stream << "\n";
    atBeginningOfLine = true;
    ++line;
}

void CodeFormatter::Flush()
{
    stream.flush();
}

CodeFormatter& operator<<(CodeFormatter& f, StandardEndLine manip)
{
    f.WriteLine();
    f.Flush();
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, const std::string& s)
{
    f.Write(s);
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, const char* s)
{
    f.Write(s);
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, char c)
{
    f.Write(std::string(1, c));
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, bool b)
{
    if (b)
    {
        f.Write("true");
    }
    else
    {
        f.Write("false");
    }
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, int x)
{
    f.Write(std::to_string(x));
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, double x)
{
    f.Write(std::to_string(x));
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, std::int64_t x)
{
    f.Write(std::to_string(x));
    return f;
}

CodeFormatter& operator<<(CodeFormatter& f, std::uint64_t x)
{
    f.Write(std::to_string(x));
    return f;
}

} // namespace util
