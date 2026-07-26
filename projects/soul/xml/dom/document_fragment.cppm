export module soul.xml.document_fragment;

import std;
import soul.xml.parent_node;
import soul.ast.source_pos;

export namespace soul::xml {

class DocumentFragment : public ParentNode
{
public:
    DocumentFragment(const soul::ast::SourcePos& sourcePos_);
};

DocumentFragment* MakeDocumentFragment();

} // namespace soul::xml
