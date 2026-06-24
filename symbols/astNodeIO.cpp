// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.ast_node_io;

import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.modules;
import otava.ast.writer;
import otava.ast.reader;
import util.utility;

namespace otava::symbols {

AstNodeHeader::AstNodeHeader() : hasNode(false), nodeFileOffset(FileOffset(0)), nodeLength(Length(0))
{
}

void AstNodeHeader::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(hasNode);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(nodeFileOffset));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(nodeLength));
}

void AstNodeHeader::Read(Reader& reader)
{
    hasNode = reader.CurrentReader().ReadBool();
    nodeFileOffset = FileOffset(reader.CurrentReader().ReadUInt());
    nodeLength = Length(reader.CurrentReader().ReadUInt());
}

void WriteNode(Writer& writer, otava::ast::Node* node, AstNodeHeader& header)
{
    FileOffset start = FileOffset(writer.Position());
    header.Write(writer);
    FileOffset valueOffset = FileOffset(writer.Position());
    if (node)
    {
        header.hasNode = true;
        otava::ast::Writer astWriter(&writer.GetBinaryStreamWriter());
        astWriter.Write(node);
    }
    else
    {
        header.hasNode = false;
    }
    FileOffset end = FileOffset(writer.Position());
    header.nodeFileOffset = valueOffset;
    header.nodeLength = Length(FileOffset(writer.Position()) - valueOffset);
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
}

std::unique_ptr<otava::ast::Node> ReadNode(Reader& reader, Module* module, AstNodeHeader& header)
{
    header.Read(reader);
    if (header.hasNode)
    {
        util::FileMapping* fileMapping = module->GetFileMapping();
        reader.PushCurrentReader(util::Advance(fileMapping->Start(), ToUnderlying(header.nodeFileOffset)), header.nodeLength);
        otava::ast::Reader astReader(&reader.CurrentReader());
        astReader.SetNodeMap(module->GetAstNodeMap());
        std::unique_ptr<otava::ast::Node> node;
        node.reset(astReader.ReadNode());
        reader.PopCurrentReader();
        return node;
    }
    else
    {
        return std::unique_ptr<otava::ast::Node>();
    }
}

} // namespace otava::symbols
