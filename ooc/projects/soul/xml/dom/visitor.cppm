export module soul.xml.visitor;

import std;
import soul.xml.document;
import soul.xml.element;
import soul.xml.text;
import soul.xml.cdata_section;
import soul.xml.comment;
import soul.xml.entity_reference;
import soul.xml.processing_instruction;

export namespace soul::xml {

class Visitor
{
public:
    virtual ~Visitor();
    virtual void BeginVisit(Document& document) {}
    virtual void EndVisit(Document& document) {}
    virtual void BeginVisit(Element& element) {}
    virtual void EndVisit(Element& element) {}
    virtual void Visit(Text& text) {}
    virtual void Visit(CDataSection& cdataSection) {}
    virtual void Visit(Comment& comment) {}
    virtual void Visit(EntityReference& entityReference) {}
    virtual void Visit(ProcessingInstruction& processingInstruction) {}
};

} // namespace soul::xml
