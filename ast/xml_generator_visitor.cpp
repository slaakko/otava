// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.xml.generator.visitor;

import otava.ast.node;
import soul.ast.source.pos;
import util.unicode;

namespace otava::ast {

XmlGeneratorVisitor::XmlGeneratorVisitor()
{
}

void XmlGeneratorVisitor::BeginVisit(Node& node)
{
    stack.push(std::move(element));
    element.reset(soul::xml::MakeElement(NodeKindStr(node.Kind())));
    AddAttribute("type", NodeTypeStr(node.Type()));
    AddAttribute("span", soul::ast::ToString(node.GetSpan()));
    int n = node.Count();
    if (n != 0)
    {
        AddAttribute("count", std::to_string(n));
    }
}

void XmlGeneratorVisitor::EndVisit(Node& node)
{
    std::unique_ptr<soul::xml::Element> parent = std::move(stack.top());
    stack.pop();
    if (parent)
    {
        parent->AppendChild(element.release());
        element.reset(parent.release());
    }
}

void XmlGeneratorVisitor::VisitIdentifier(const std::u32string& id, const soul::ast::Span& span)
{
    std::unique_ptr<soul::xml::Element> idElement(soul::xml::MakeElement("identifier"));
    idElement->SetAttribute("value", util::ToUtf8(id));
    idElement->SetAttribute("span", soul::ast::ToString(span));
    AddElement(idElement.release());
}

void XmlGeneratorVisitor::VisitKeyword(const std::string& keyword, const soul::ast::Span& span)
{
    std::unique_ptr<soul::xml::Element> kwElement(soul::xml::MakeElement("keyword"));
    kwElement->SetAttribute("value", keyword);
    kwElement->SetAttribute("span", soul::ast::ToString(span));
    AddElement(kwElement.release());
}

void XmlGeneratorVisitor::VisitOperator(const std::string& symbol, const soul::ast::Span& span)
{
    std::unique_ptr<soul::xml::Element> opElement(soul::xml::MakeElement("operator"));
    opElement->SetAttribute("value", symbol);
    opElement->SetAttribute("span", soul::ast::ToString(span));
    AddElement(opElement.release());
}

void XmlGeneratorVisitor::VisitToken(const std::u32string& tokenStr, const soul::ast::Span& span)
{
    std::unique_ptr<soul::xml::Element> tokenElement(soul::xml::MakeElement("token"));
    tokenElement->SetAttribute("value", util::ToUtf8(tokenStr));
    tokenElement->SetAttribute("span", soul::ast::ToString(span));
    AddElement(tokenElement.release());
}

void XmlGeneratorVisitor::VisitLiteral(const std::u32string& rep, const soul::ast::Span& span)
{
    std::unique_ptr<soul::xml::Element> literalElement(soul::xml::MakeElement("literal"));
    literalElement->SetAttribute("value", util::ToUtf8(rep));
    literalElement->SetAttribute("span", soul::ast::ToString(span));
    AddElement(literalElement.release());
}

void XmlGeneratorVisitor::VisitHeaderName(const std::u32string& rep, const soul::ast::Span& span)
{
    std::unique_ptr<soul::xml::Element> headerElement(soul::xml::MakeElement("header"));
    headerElement->SetAttribute("value", util::ToUtf8(rep));
    headerElement->SetAttribute("span", soul::ast::ToString(span));
    AddElement(headerElement.release());
}

void XmlGeneratorVisitor::AddAttribute(const std::string& name, const std::string& value)
{
    element->SetAttribute(name, value);
}

void XmlGeneratorVisitor::AddElement(soul::xml::Element* child)
{
    element->AppendChild(child);
}

} // namespace otava::ast
