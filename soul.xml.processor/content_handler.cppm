// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.processor.content_handler;

import std;
import soul.ast.source_pos;
import soul.xml.processor.attribute;
import soul.lexer.base;

export namespace soul::xml {

class XmlContentHandler
{
public:
    XmlContentHandler();
    inline soul::lexer::LexerBase<char32_t>* GetLexer() noexcept { return lexer; }
    inline void SetLexer(soul::lexer::LexerBase<char32_t>* lexer_) noexcept { lexer = lexer_; }
    virtual ~XmlContentHandler();
    virtual void StartDocument(const soul::ast::SourcePos& sourcePos) {}
    virtual void EndDocument() {}
    virtual void XmlVersion(const std::string& xmlVersion) {}
    virtual void XmlEncoding(const std::string& xmlEncoding) {}
    virtual void Standalone(bool standalone) {}
    virtual void Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment) {}
    virtual void PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data) {}
    virtual void Text(const soul::ast::SourcePos& sourcePos, const std::string& text) {}
    virtual void CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata) {}
    virtual void StartElement(const soul::ast::SourcePos& sourcePos, const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName, 
        const soul::xml::processor::Attributes& attributes) {} 
    virtual void EndElement(const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName) {}
    virtual void SkippedEntity(const soul::ast::SourcePos& sourcePos, const std::string& entityName) {}
private:
    soul::lexer::LexerBase<char32_t>* lexer;
};

} // namespace soul::xml
