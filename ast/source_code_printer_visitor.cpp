// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.source.code.printer.visitor;

import util.unicode;
import otava.ast.node;
import otava.ast.util;

namespace otava::ast {

SourceCodePrinterVisitor::SourceCodePrinterVisitor(std::ostream& stream, const std::vector<int>& lineStartIndeces_) : 
    formatter(stream), line(1), col(1), lineStartIndeces(lineStartIndeces_)
{
}

void SourceCodePrinterVisitor::Move(const soul::ast::Span& span)
{
    soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, lineStartIndeces);
    while (lineColLen.line > line)
    {
        formatter.WriteLine();
        ++line;
        col = 1;
    }
    if (lineColLen.col > col)
    {
        int n = lineColLen.col - col;
        formatter.Write(std::string(n, ' '));
        col += n;
    }
}

void SourceCodePrinterVisitor::Move(const std::u32string& str)
{
    for (char32_t c : str)
    {
        if (c == '\n')
        {
            ++line;
            col = 1;
        }
        else if (c == '\t')
        {
            col += 4;
        }
        else
        {
            ++col;
        }
    }
}

void SourceCodePrinterVisitor::Write(const std::string& str)
{
    formatter.Write(str);
    Move(util::ToUtf32(str));
}

void SourceCodePrinterVisitor::Write(const std::u32string& str)
{
    formatter.Write(util::ToUtf8(str));
    Move(str);
}

void SourceCodePrinterVisitor::BeginVisit(Node& node)
{
    Move(node.GetSpan());
}

void SourceCodePrinterVisitor::VisitIdentifier(const std::u32string& id, const soul::ast::Span& span)
{
    Move(span);
    Write(ToUniversalId(id));
}

void SourceCodePrinterVisitor::VisitKeyword(const std::string& keyword, const soul::ast::Span& span)
{
    Move(span);
    Write(keyword);
}

void SourceCodePrinterVisitor::VisitOperator(const std::string& symbol, const soul::ast::Span& span)
{
    Move(span);
    Write(symbol);
}

void SourceCodePrinterVisitor::VisitToken(const std::u32string& tokenStr, const soul::ast::Span& span)
{
    Move(span);
    Write(tokenStr);
}

void SourceCodePrinterVisitor::VisitLiteral(const std::u32string& rep, const soul::ast::Span& span)
{
    Move(span);
    Write(rep);
}

void SourceCodePrinterVisitor::VisitHeaderName(const std::u32string& rep, const soul::ast::Span& span)
{
    Move(span);
    Write(rep);
}

} // namespace otava::ast
