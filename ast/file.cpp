// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.file;

import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

File::File() : filePath(), content()
{
}

File::File(const std::string& filePath_, Node* content_) : filePath(filePath_), content(content_)
{
}

void File::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(filePath);
    writer.Write(content.get());
}

void File::Read(Reader& reader)
{
    filePath = reader.GetBinaryStreamReader().ReadUtf8String();
    content.reset(reader.ReadNode());
}

} // namespace otava::ast
