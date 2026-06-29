module soul.ast.span;

namespace soul::ast {

void Span::Union(const Span& that) noexcept
{
    int end = that.pos + that.len;
    len = end - pos;
}

LineColLen SpanToLineColLen(const Span& span, const std::vector<int>& lineStarts) noexcept
{
    if (!span.IsValid() || lineStarts.empty())
    {
        return LineColLen();
    }
    else
    {
        auto it = std::lower_bound(lineStarts.begin(), lineStarts.end(), span.pos);
        int lineStart = 0;
        int line = 0;
        if (it != lineStarts.end())
        {
            if (it != lineStarts.begin() && *it > span.pos)
            {
                --it;
            }
            line = static_cast<int>(it - lineStarts.begin());
            lineStart = lineStarts[line];
        }
        else
        {
            line = int(lineStarts.size());
            lineStart = lineStarts[line - 1];
        }
        int col = span.pos - lineStart + 1;
        int len = span.len;
        if (line == 0)
        {
            ++line;
        }
        return LineColLen(line, col, len);
    }
}

int LineColLenToPos(const LineColLen& lineColLen, const std::vector<int>& lineStarts) noexcept
{
    if (!lineColLen.IsValid() || lineStarts.empty() || lineColLen.line >= lineStarts.size())
    {
        return -1;
    }
    int pos = lineStarts[lineColLen.line] + lineColLen.col - 1;
    return pos;
}

bool operator==(const LineColLen& left, const LineColLen& right) noexcept
{
    return left.line == right.line && left.col == right.col && left.len == right.len;
}

bool operator<(const LineColLen& left, const LineColLen& right) noexcept
{
    if (left.line < right.line) return true;
    if (left.line > right.line) return false;
    if (left.col < right.col) return true;
    if (left.col > right.col) return false;
    return left.len < right.len;
}

std::string ToString(const Span& span)
{
    std::string s("(");
    if (span.IsValid())
    {
        s.append(std::to_string(span.pos)).append(", ").append(std::to_string(span.len));
    }
    s.append(")");
    return s;
}

} // namespace soul::ast
