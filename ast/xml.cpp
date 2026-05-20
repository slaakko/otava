// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.xml;
import otava.ast.xml.generator.visitor;
import otava.ast.node;
import soul.xml.dom;
import util.code.formatter;

namespace otava::ast {

void WriteXml(Node* node, const std::string& filePath)
{
    XmlGeneratorVisitor generator;
    node->Accept(generator);
    std::unique_ptr<soul::xml::Element> element = generator.GetElement();
    soul::xml::Document doc;
    doc.AppendChild(element.release());
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    doc.Write(formatter);
}

} // namespace otava::ast
