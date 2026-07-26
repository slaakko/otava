module soul.xml.index;

import soul.xml.element;
import soul.xml.visitor;

namespace soul::xml {

class BuildIndexVisitor : public Visitor
{
public:
    BuildIndexVisitor(std::map<std::string, Element*>& index_);
    void BeginVisit(Element& element) override;
private:
    std::map<std::string, Element*>& index;
};

BuildIndexVisitor::BuildIndexVisitor(std::map<std::string, Element*>& index_) : index(index_)
{
}

void BuildIndexVisitor::BeginVisit(Element& element)
{
    std::string id = element.GetAttribute("id");
    if (!id.empty())
    {
        index[id] = &element;
    }
}

void BuildIndex(Document* document)
{
    BuildIndexVisitor visitor(document->Index());
    document->Accept(visitor);
}

} // namespace soul::xml
