// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.container_symbol;

import std;
import otava.symbols.id;
import otava.symbols.scope;
import otava.symbols.symbol;
import soul.ast.span;

export namespace otava::symbols {

class BlockSymbol;

struct ContainerSymbolHeader
{
    ContainerSymbolHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    Cardinality memberCount;
    FileOffset bodyOffset;
    Length bodyLength;
    FileOffset scopeOffset;
    Length scopeLength;
};

class ContainerSymbol : public Symbol
{
public:
    ContainerSymbol(Module* module_, SymbolId id_);
    ContainerSymbol(Module* module_, SymbolId id_, const std::string& name_);
    Scope* GetScope() override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    Cardinality MemberCount() const noexcept;
    inline FileOffset ScopeOffset() const noexcept { return header.scopeOffset; }
    inline Length ScopeLength() const noexcept { return header.scopeLength; }
    Symbol* GetMember(Index index, Context* context);
    virtual void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context);
    bool IsContainerSymbol() const noexcept override { return true; }
    inline const std::vector<BlockSymbol*>& Blocks() const noexcept { return blocks; }
private:
    ContainerSymbolHeader header;
    std::vector<SymbolId> memberIds;
    std::vector<std::unique_ptr<Symbol>> symbols;
    ContainerScope scope;
    std::vector<BlockSymbol*> blocks;
    bool bodyRead;
    void ReadBody();
};

} // namespace otava::symbols
