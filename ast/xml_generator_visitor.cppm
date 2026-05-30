// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.xml.generator.visitor;

import std;
import otava.ast.visitor;
import soul.xml.element;

export namespace otava::ast {

class XmlGeneratorVisitor : public DefaultVisitor
{
public:
    XmlGeneratorVisitor();
    inline std::unique_ptr<soul::xml::Element> GetElement() { return std::move(element); }
    void BeginVisit(Node& node) override;
    void EndVisit(Node& node) override;
    void VisitIdentifier(const std::string& id, const soul::ast::Span& span) override;
    void VisitKeyword(const std::string& keyword, const soul::ast::Span& span) override;
    void VisitOperator(const std::string& symbol, const soul::ast::Span& span) override;
    void VisitToken(const std::string& tokenStr, const soul::ast::Span& span) override;
    void VisitLiteral(const std::string& rep, const soul::ast::Span& span) override;
    void VisitHeaderName(const std::string& rep, const soul::ast::Span& span) override;
    void AddAttribute(const std::string& name, const std::string& value);
    void AddElement(soul::xml::Element* child);
private:
    std::unique_ptr<soul::xml::Element> element;
    std::stack<std::unique_ptr<soul::xml::Element>> stack;
};

} // namespace otava::ast
