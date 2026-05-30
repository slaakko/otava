// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.source.code.printer.visitor;

import std;
import otava.ast.visitor;
import util.code_formatter;

export namespace otava::ast {

class SourceCodePrinterVisitor : public DefaultVisitor
{
public:
    SourceCodePrinterVisitor(std::ostream& stream, const std::vector<int>& lineStartIndeces_);
    void BeginVisit(Node& node) override;
    void VisitIdentifier(const std::string& id, const soul::ast::Span& span) override;
    void VisitKeyword(const std::string& keyword, const soul::ast::Span& span) override;
    void VisitOperator(const std::string& symbol, const soul::ast::Span& span) override;
    void VisitToken(const std::string& tokenStr, const soul::ast::Span& span) override;
    void VisitLiteral(const std::string& rep, const soul::ast::Span& span) override;
    void VisitHeaderName(const std::string& rep, const soul::ast::Span& span) override;
private:
    void Move(const soul::ast::Span& span);
    void Move(const std::string& str);
    void Write(const std::string& str);
    util::CodeFormatter formatter;
    int line;
    int col;
    std::vector<int> lineStartIndeces;
};

} // namespace otava::ast
