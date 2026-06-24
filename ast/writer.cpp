// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.writer;

namespace otava::ast {

/*
Writer::Writer(const std::string& fileName) : 
    fileStream(new util::FileStream(fileName, util::OpenMode::binary | util::OpenMode::write)),
    bufferedStream(new util::BufferedStream(*fileStream)), 
    binaryStreamWriter(new util::BinaryStreamWriter(*bufferedStream)),
    writerPtr(binaryStreamWriter.get())
{
}
*/

Writer::Writer(util::BinaryStreamWriter* writerPtr_) : writerPtr(writerPtr_)
{
}

/*
void Writer::Write(const soul::ast::Span& span)
{
    if (span.IsValid())
    {
        writerPtr->WriteULEB128UInt(span.len);
        writerPtr->WriteULEB128UInt(span.pos);
    }
    else
    {
        writerPtr->WriteULEB128UInt(0);
    }
}
*/

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
        //Write(node->GetSpan());
        node->Write(*this);
    }
}

} // namespace otava::ast
