// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.scope;

import std;
import soul.ast.span;
import otava.symbols.lookup;
import otava.symbols.id;

export namespace otava::symbols {

class Symbol;
class Module;
class Context;
class NamespaceSymbol;
class ContainerSymbol;
class ClassGroupSymbol;
class FunctionGroupSymbol;
class ConceptGroupSymbol;
class VariableGroupSymbol;
class AliasGroupSymbol;
class EnumGroupSymbol;
class TemplateParamGroupSymbol;
class UsingDeclarationScope;
class UsingDirectiveScope;
class ClassTemplateSpecializationSymbol;
class Writer;
class Reader;

enum class ScopeKind : std::uint8_t
{
    none, namespaceScope, templateDeclarationScope, classScope, enumerationScope, functionScope, arrayScope, blockScope, usingDeclarationScope, 
    usingDirectiveScope, instantiationScope
};

constexpr std::uint8_t ToUnderlying(ScopeKind scopeKind) { return std::uint8_t(scopeKind); }

enum class SymbolOffset : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(SymbolOffset symbolOffset) { return std::uint32_t(symbolOffset); }

enum class SymbolGroupKind : std::uint8_t
{
    none = 0,
    functionSymbolGroup = 1 << 0,
    aliasSymbolGroup = 1 << 1,
    classSymbolGroup = 1 << 2,
    enumSymbolGroup = 1 << 3,
    templateParamSymbolGroup = 1 << 4,
    variableSymbolGroup = 1 << 5,
    namespaceSymbolGroup = 1 << 6,
    all = functionSymbolGroup | aliasSymbolGroup | classSymbolGroup | enumSymbolGroup | templateParamSymbolGroup | variableSymbolGroup | namespaceSymbolGroup
};

constexpr std::uint8_t ToUnderlying(SymbolGroupKind symbolGroupKind) { return std::uint8_t(symbolGroupKind); }

constexpr std::uint32_t symbolOffsetShift = 32 - 7;

constexpr std::uint32_t symbolOffsetMask = 0xFFFFFFFFu >> 7;

constexpr std::uint8_t symbolGroupMask = 0x7Fu;

constexpr SymbolOffset MakeSymbolOffset(SymbolGroupKind symbolGroupKind, StringOffset stringOffset)
{
    return SymbolOffset((std::uint32_t(ToUnderlying(symbolGroupKind)) << symbolOffsetShift) | (ToUnderlying(stringOffset) & symbolOffsetMask));
}

constexpr SymbolGroupKind GetSymbolGroupKind(SymbolOffset symbolOffset)
{
    return SymbolGroupKind((ToUnderlying(symbolOffset) >> symbolOffsetShift) & symbolGroupMask);
}

constexpr StringOffset GetStringOffset(SymbolOffset symbolOffset)
{
    return StringOffset(ToUnderlying(symbolOffset) & symbolOffsetMask);
}

std::string SymbolGroupStr(SymbolGroupKind group);

constexpr SymbolGroupKind operator|(SymbolGroupKind left, SymbolGroupKind right) noexcept
{
    return SymbolGroupKind(ToUnderlying(left) | ToUnderlying(right));
}

constexpr SymbolGroupKind operator&(SymbolGroupKind left, SymbolGroupKind right) noexcept
{
    return SymbolGroupKind(ToUnderlying(left) & ToUnderlying(right));
}

constexpr SymbolGroupKind operator~(SymbolGroupKind kind) noexcept
{
    return SymbolGroupKind(~ToUnderlying(kind));
}

std::vector<SymbolGroupKind> SymbolGroupKindstoSymbolGroupKindVec(SymbolGroupKind symbolGroupKinds);

std::string ScopeKindStr(ScopeKind kind);

void SetCompileEnded();
void ResetCompileEnded();

class Scope
{
public:
    Scope(Module* module_) noexcept;
    virtual ~Scope();
    inline Module* GetModule() const noexcept { return module; }
    inline void ResetModule() { module = nullptr; }
    inline ScopeKind Kind() const noexcept { return kind; }
    inline void SetKind(ScopeKind kind_) noexcept { kind = kind_; }
    inline bool IsReadOnly() const noexcept { return readOnly; }
    inline bool IsGlobal() const noexcept { return global; }
    inline void SetGlobal() noexcept { global = true; }
    void Install(Symbol* symbol, Context* context);
    void Uninstall(Symbol* symbol);
    void ImportModuleScopes(Context* context);
    virtual void Import(Scope* that, Context* context);
    Symbol* Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, const soul::ast::FullSpan& fullSpan,
        Context* context, LookupFlags flags);
    inline bool IsBlockScope() const noexcept { return kind == ScopeKind::blockScope; }
    inline bool IsClassScope() const noexcept { return kind == ScopeKind::classScope; }
    inline bool IsNamespaceScope() const noexcept { return kind == ScopeKind::namespaceScope; }
    inline bool IsTemplateDeclarationScope() const noexcept { return kind == ScopeKind::templateDeclarationScope; }
    virtual Scope* GroupScope(Context* context) noexcept;
    virtual Scope* SymbolScope(Context* context) noexcept;
    virtual std::string FullName(Context* context) const = 0;
    virtual bool IsContainerScope() const noexcept { return false; }
    virtual Scope* GetClassScope(Context* context) const noexcept { return nullptr; }
    virtual Scope* GetNamespaceScope(Context* context) const noexcept { return nullptr; }
    virtual Symbol* GetSymbol() noexcept { return nullptr; }
    virtual ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(std::set<Scope*>& visited) const { return nullptr; }
    virtual void Lookup(const std::string& name, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context);
    virtual void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual std::unique_ptr<Symbol> RemoveSymbol(Symbol* symbol);
    virtual std::vector<Scope*> ParentScopes(Context* context) { return std::vector<Scope*>(); }
    virtual void AddParentScope(Scope* parentScope_);
    virtual void RemoveParentScope(Scope* parentScope);
    virtual void PushParentScope(Scope* parentScope);
    virtual void PopParentScope();
    virtual bool HasParentScope(const Scope* parentScope) const noexcept { return false; }
    virtual void ClearParentScopes() {}
    virtual void AddBaseScope(Scope* baseScope, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual void AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual void AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual NamespaceSymbol* GetOrInsertNamespace(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual ClassGroupSymbol* GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual FunctionGroupSymbol* GetOrInsertFunctionGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual VariableGroupSymbol* GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual AliasGroupSymbol* GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual EnumGroupSymbol* GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual TemplateParamGroupSymbol* GetOrInsertTemplateParamGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    void Read();
    inline const std::unordered_map<SymbolOffset, SymbolId>& SymbolIdMap() const noexcept { return symbolIdMap; }
    virtual void Write(Writer& writer);
    virtual void Read(Reader& reader);
    bool Imported(Scope* scope) noexcept;
    void AddImported(Scope* scope);
    void AddContainerScope(Scope* containerScope);
    void RemoveContainerScope(Scope* containerScope);
private:
    Module* module;
    bool readOnly;
    ScopeKind kind;
    std::unordered_map<SymbolOffset, SymbolId> symbolIdMap;
    std::unordered_map<SymbolOffset, ModuleSymbolId> moduleSymbolIdMap;
    bool read;
    bool global;
    bool imported;
    bool destructing;
    std::set<Scope*> importSet;
    std::vector<Scope*> containerScopes;
};

class ContainerScope : public Scope
{
public:
    ContainerScope(Module* module_) noexcept;
    ~ContainerScope();
    void Import(Scope* that, Context* context) override;
    std::vector<Scope*> ParentScopes(Context* context) override;
    std::vector<Scope*> BaseScopes(Context* context);
    void AddParentScope(Scope* parentScope) override;
    void RemoveParentScope(Scope* parentScope) override;
    void PushParentScope(Scope* parentScope) override;
    void PopParentScope() override;
    void ClearParentScopes() override;
    bool IsContainerScope() const noexcept override { return true; }
    Scope* GetClassScope(Context* context) const noexcept override;
    Scope* GetNamespaceScope(Context* context) const noexcept override;
    void AddBaseScope(Scope* baseScope, const soul::ast::FullSpan& fullSpan, Context* context) override;
    Symbol* GetSymbol() noexcept override;
    ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(std::set<Scope*>& visited) const override;
    inline ContainerSymbol* GetContainerSymbol() const noexcept { return containerSymbol; }
    inline void SetContainerSymbol(ContainerSymbol* containerSymbol_) noexcept { containerSymbol = containerSymbol_; }
    void AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context) override;
    std::string FullName(Context* context) const override;
    void Lookup(const std::string& name, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) override;
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    std::unique_ptr<Symbol> RemoveSymbol(Symbol* symbol) override;
    NamespaceSymbol* GetOrInsertNamespace(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    ClassGroupSymbol* GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    FunctionGroupSymbol* GetOrInsertFunctionGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    VariableGroupSymbol* GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    AliasGroupSymbol* GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    EnumGroupSymbol* GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    TemplateParamGroupSymbol* GetOrInsertTemplateParamGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    bool HasParentScope(const Scope* parentScope) const noexcept override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    std::vector<Scope*> parentScopes;
    std::vector<Scope*> baseScopes;
    std::vector<Scope*> pscopes;
    UsingDeclarationScope* usingDeclarationScope;
    ContainerSymbol* containerSymbol;
    std::vector<UsingDirectiveScope*> usingDirectiveScopes;
    std::vector<std::unique_ptr<Scope>> scopes;
    bool parentScopePushed;
    bool destructing;
};

class UsingDeclarationScope : public Scope
{
public:
    UsingDeclarationScope(Module* module_, ContainerScope* parentScope_) noexcept;
    std::string FullName(Context* context) const override;
    void Import(UsingDeclarationScope* that, Context* context);
    void Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) override;
private:
    ContainerScope* parentScope;

};

class UsingDirectiveScope : public Scope
{
public:
    UsingDirectiveScope(Module* module_) noexcept;
    UsingDirectiveScope(Module* module_, NamespaceSymbol* ns_) noexcept;
    void Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) override;
    std::string FullName(Context* context) const override;
    NamespaceSymbol* Ns() const { return ns; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    NamespaceSymbol* ns;
    SymbolId nsId;
};

class InstantiationScope : public Scope
{
public:
    InstantiationScope(Module* module_, Scope* parentScope_) noexcept;
    std::string FullName(Context* context) const override;
    Scope* GroupScope(Context* context) noexcept override;
    Scope* SymbolScope(Context* context) noexcept override;
    Scope* GetClassScope(Context* context) const noexcept override;
    Scope* GetNamespaceScope(Context* context) const noexcept override;
    ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(std::set<Scope*>& visited) const override;
    void Lookup(const std::string& id, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) override;
    void PushParentScope(Scope* parentScope_) override;
    void PopParentScope() override;
    bool HasParentScope(const Scope* parentScope) const noexcept override;
private:
    std::vector<Scope*> parentScopes;
};

Scope* MakeScope(Module* module, ScopeKind scopeKind, ContainerScope* parentScope);

} // namespace otava::symbols
