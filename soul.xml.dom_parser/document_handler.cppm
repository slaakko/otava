// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.document_handler;

import std;
import soul.xml.document;
import soul.xml.element;
import soul.xml.parent_node;
import soul.xml.processor.content_handler;
import soul.xml.dom_parser;

export namespace soul::xml {

class DocumentHandlerData;

class DocumentHandler : public soul::xml::XmlContentHandler
{
public:
    DocumentHandler(soul::xml::ParsingFlags flags_);
    inline std::unique_ptr<soul::xml::Document> GetDocument() noexcept { return std::move(document); };
    void StartDocument(const soul::ast::SourcePos& sourcePos) override;
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
private:
    void AddTextContent();
    void AddTextContent(bool addSpace);
    soul::xml::ParsingFlags flags;
    std::unique_ptr<soul::xml::Document> document;
    soul::xml::ParentNode* currentParentNode;
    std::stack<soul::xml::ParentNode*> parentNodeStack;
    std::unique_ptr<soul::xml::Element> currentElement;
    std::stack<std::unique_ptr<soul::xml::Element>> elementStack;
    soul::ast::SourcePos textSourcePos;
    std::string textContent;
};

} // namespace soul::xml
