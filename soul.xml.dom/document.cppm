// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.document;

import std;
import soul.xml.node;
import soul.xml.parent_node;
import soul.xml.element;

export namespace soul::xml {

class Document : public ParentNode
{
public:
    Document();
    Document(const soul::ast::SourcePos& sourcePos_);
    inline Element* DocumentElement() const noexcept { return documentElement; }
    inline bool XmlStandalone() const noexcept { return xmlStandalone; }
    inline void SetXmlStandalone(bool xmlStandalone_) noexcept { xmlStandalone = xmlStandalone_; }
    inline const std::string& XmlVersion() const noexcept { return xmlVersion; }
    inline void SetXmlVersion(const std::string& xmlVersion_) { xmlVersion = xmlVersion_; }
    inline const std::string& XmlEncoding() const noexcept { return xmlEncoding; }
    inline void SetXmlEncoding(const std::string& xmlEncoding_) { xmlEncoding = xmlEncoding_; }
    inline std::map<std::string, Element*>& Index() noexcept { return index; }
    Element* GetElementById(const std::string& elementId) noexcept;
    void AppendChild(Node* child) override;
    void InsertBefore(Node* newChild, Node* refChild) override;
    std::unique_ptr<Node> RemoveChild(Node* child) override;
    std::unique_ptr<Node> ReplaceChild(Node* newChild, Node* oldChild) override;
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    friend class ParentNode;
    void InvalidateIndex();
    void CheckValidInsert(Node* node);
    Element* documentElement;
    bool indexValid;
    bool xmlStandalone;
    std::string xmlVersion;
    std::string xmlEncoding;
    std::map<std::string, Element*> index;
};

std::unique_ptr<Document> MakeDocument();

} // namespace soul::xml
