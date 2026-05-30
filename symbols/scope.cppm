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
class UsingDeclarationScope;
class UsingDirectiveScope;
class ClassTemplateSpecializationSymbol;

enum class ScopeKind : std::uint8_t
{
    none, namespaceScope, templateDeclarationScope, classScope, enumerationScope, functionScope, arrayScope, blockScope, usingDeclarationScope, 
    usingDirectiveScope, instantiationScope
};

enum class SymbolOffset : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(SymbolOffset symbolOffset) { return std::uint32_t(symbolOffset); }

enum class SymbolGroupKind : std::uint8_t
{
    none = 0,
    functionSymbolGroup = 1 << 0,
    typeSymbolGroup = 1 << 1,
    variableSymbolGroup = 1 << 2,
    enumConstantSymbolGroup = 1 << 3,
    conceptSymbolGroup = 1 << 4,
    blockSymbolGroup = 1 << 5,
    all = functionSymbolGroup | typeSymbolGroup | variableSymbolGroup | enumConstantSymbolGroup | conceptSymbolGroup | blockSymbolGroup
};

constexpr std::uint8_t ToUnderlying(SymbolGroupKind symbolGroupKind) { return std::uint8_t(symbolGroupKind); }

constexpr std::uint32_t symbolOffsetShift = 32 - 6;

constexpr std::uint32_t symbolOffsetMask = 0xFFFFFFFFu >> 6;

constexpr std::uint8_t symbolGroupMask = 0x3Fu;

constexpr SymbolOffset MakeSymbolOffset(SymbolGroupKind symbolGroupKind, StringOffset stringOffset)
{
    return SymbolOffset((std::uint32_t(ToUnderlying(symbolGroupKind)) << symbolOffsetShift) | (ToUnderlying(stringOffset) & symbolOffsetMask));
}

constexpr SymbolGroupKind GetSymbolGroupKind(SymbolOffset symbolOffset)
{
    return SymbolGroupKind((ToUnderlying(symbolOffset) >> symbolOffsetShift) & symbolGroupMask);
}

std::string SymbolGroupStr(SymbolGroupKind group);

constexpr SymbolGroupKind operator|(SymbolGroupKind left, SymbolGroupKind right) noexcept
{
    return SymbolGroupKind(std::uint8_t(left) | std::uint8_t(right));
}

constexpr SymbolGroupKind operator&(SymbolGroupKind left, SymbolGroupKind right) noexcept
{
    return SymbolGroupKind(std::uint8_t(left) & std::uint8_t(right));
}

constexpr SymbolGroupKind operator~(SymbolGroupKind kind) noexcept
{
    return SymbolGroupKind(~std::uint8_t(kind));
}

std::vector<SymbolGroupKind> SymbolGroupKindstoSymbolGroupKindVec(SymbolGroupKind symbolGroupKinds);

std::string ScopeKindStr(ScopeKind kind);

class Scope
{
public:
    Scope(Module* module_) noexcept;
    virtual ~Scope();
    inline Module* GetModule() const noexcept { return module; }
    inline ScopeKind Kind() const noexcept { return kind; }
    inline void SetKind(ScopeKind kind_) noexcept { kind = kind_; }
    void Install(Symbol* symbol);
    void Uninstall(Symbol* symbol);
    Symbol* Lookup(const std::string& id, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, const soul::ast::FullSpan& fullSpan,
        Context* context, LookupFlags flags) const;
    inline bool IsBlockScope() const noexcept { return kind == ScopeKind::blockScope; }
    inline bool IsClassScope() const noexcept { return kind == ScopeKind::classScope; }
    inline bool IsNamespaceScope() const noexcept { return kind == ScopeKind::namespaceScope; }
    inline bool IsTemplateDeclarationScope() const noexcept { return kind == ScopeKind::templateDeclarationScope; }
    virtual Scope* GroupScope() noexcept;
    virtual Scope* SymbolScope() noexcept;
    virtual std::string FullName() const = 0;
    virtual bool IsContainerScope() const noexcept { return false; }
    virtual Scope* GetClassScope() const noexcept { return nullptr; }
    virtual Scope* GetNamespaceScope() const noexcept { return nullptr; }
    virtual Symbol* GetSymbol() noexcept { return nullptr; }
    virtual ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(std::set<Scope*>& visited) const { return nullptr; }
    virtual void Lookup(const std::string& id, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) const;
    virtual void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual std::unique_ptr<Symbol> RemoveSymbol(Symbol* symbol);
    virtual std::vector<Scope*> ParentScopes() const { return std::vector<Scope*>(); }
    virtual void AddParentScope(Scope* parentScope_);
    virtual void PushParentScope(Scope* parentScope);
    virtual void PopParentScope();
    virtual bool HasParentScope(const Scope* parentScope) const noexcept { return false; }
    virtual void ClearParentScopes() {}
    virtual void AddBaseScope(Scope* baseScope, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual void AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual void AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual ClassGroupSymbol* GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual FunctionGroupSymbol* GetOrInsertFunctionGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual ConceptGroupSymbol* GetOrInsertConceptGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual VariableGroupSymbol* GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual AliasGroupSymbol* GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual EnumGroupSymbol* GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);
    //virtual void Import(Scope* that, Context* context);
private:
    Module* module;
    ScopeKind kind;
    std::unordered_map<SymbolOffset, SymbolId> symbolIdMap;
};

class ContainerScope : public Scope
{
public:
    ContainerScope(Module* module_) noexcept;
    //void Import(Scope* that, Context* context) override;
    std::vector<Scope*> ParentScopes() const override;
    void AddParentScope(Scope* parentScope) override;
    void PushParentScope(Scope* parentScope) override;
    void PopParentScope() override;
    void ClearParentScopes() override;
    bool IsContainerScope() const noexcept override { return true; }
    Scope* GetClassScope() const noexcept override;
    Scope* GetNamespaceScope() const noexcept override;
    void AddBaseScope(Scope* baseScope, const soul::ast::FullSpan& fullSpan, Context* context) override;
    Symbol* GetSymbol() noexcept override;
    ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(std::set<Scope*>& visited) const override;
    inline ContainerSymbol* GetContainerSymbol() const noexcept { return containerSymbol; }
    inline void SetContainerSymbol(ContainerSymbol* containerSymbol_) noexcept { containerSymbol = containerSymbol_; }
    void AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context) override;
    std::string FullName() const override;
    void Lookup(const std::string& id, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) const override;
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    std::unique_ptr<Symbol> RemoveSymbol(Symbol* symbol) override;
    ClassGroupSymbol* GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    FunctionGroupSymbol* GetOrInsertFunctionGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    ConceptGroupSymbol* GetOrInsertConceptGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    VariableGroupSymbol* GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    AliasGroupSymbol* GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    EnumGroupSymbol* GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool HasParentScope(const Scope* parentScope) const noexcept override;
private:
    std::vector<Scope*> parentScopes;
    std::vector<Scope*> baseScopes;
    Scope* usingDeclarationScope;
    ContainerSymbol* containerSymbol;
    std::vector<UsingDirectiveScope*> usingDirectiveScopes;
    std::vector<std::unique_ptr<Scope>> scopes;
    bool parentScopePushed;
};

class UsingDeclarationScope : public Scope
{
public:
    UsingDeclarationScope(Module* module_, ContainerScope* parentScope_) noexcept;
    std::string FullName() const override;
    void Lookup(const std::string& id, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) const override;
private:
    ContainerScope* parentScope;
};

class UsingDirectiveScope : public Scope
{
public:
    UsingDirectiveScope(Module* module_, NamespaceSymbol* ns_) noexcept;
    void Lookup(const std::string& id, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) const override;
    std::string FullName() const override;
    NamespaceSymbol* Ns() const { return ns; }
private:
    NamespaceSymbol* ns;
};

class InstantiationScope : public Scope
{
public:
    InstantiationScope(Module* module_, Scope* parentScope_) noexcept;
    std::string FullName() const override;
    Scope* GroupScope() noexcept override;
    Scope* SymbolScope() noexcept override;
    Scope* GetClassScope() const noexcept override;
    Scope* GetNamespaceScope() const noexcept override;
    ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(std::set<Scope*>& visited) const override;
    void Lookup(const std::string& id, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, LookupFlags flags,
        std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) const override;
    void PushParentScope(Scope* parentScope_) override;
    void PopParentScope() override;
    bool HasParentScope(const Scope* parentScope) const noexcept override;
private:
    std::vector<Scope*> parentScopes;
};

} // namespace otava::symbols
