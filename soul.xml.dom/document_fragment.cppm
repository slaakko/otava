// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.document_fragment;

import std;
import soul.xml.parent_node;

export namespace soul::xml {

class DocumentFragment : public ParentNode
{
public:
    DocumentFragment(const soul::ast::SourcePos& sourcePos_);
};

DocumentFragment* MakeDocumentFragment();

} // namespace soul::xml
