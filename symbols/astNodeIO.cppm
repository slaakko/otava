// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.ast_node_io;

import std;
import otava.ast.node;
import otava.symbols.id;

export namespace otava::symbols {

class Writer;
class Reader;
class Module;

struct AstNodeHeader
{
    AstNodeHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    bool hasNode;
    FileOffset nodeFileOffset;
    Length nodeLength;
};

void WriteNode(Writer& writer, otava::ast::Node* node, AstNodeHeader& header);
std::unique_ptr<otava::ast::Node> ReadNode(Reader& reader, Module* module, AstNodeHeader& header);

} // namespace otava::symbols
