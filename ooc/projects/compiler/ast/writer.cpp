// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.writer;

namespace otava::ast {

Writer::Writer(util::BinaryStreamWriter* writerPtr_) : writerPtr(writerPtr_)
{
}

void Writer::Write(NodeKind nodeKind)
{
    writerPtr->Write(static_cast<std::uint16_t>(nodeKind));
}

void Writer::Write(const std::string& str)
{
    writerPtr->Write(str);
}

void Writer::Write(bool value)
{
    writerPtr->Write(value);
}

void Writer::Write(Node* node)
{
    if (!node)
    {
        Write(NodeKind::nullNode);
    }
    else
    {
        Write(node->Kind());
        node->Write(*this);
    }
}

} // namespace otava::ast
