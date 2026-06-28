// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.class_templates;

import otava.symbols.classes;
import otava.ast.node;
import std;

export namespace otava::symbols {

class CompoundTypeSymbol;
class TypeSymbol;

class ClassTemplateSpecializationSymbol : public ClassTypeSymbol
{
public:
    ClassTemplateSpecializationSymbol(Module* module__, SymbolId id_);
    ClassTemplateSpecializationSymbol(Module* module__, SymbolId id_, const std::string& name_);
    inline bool Instantiated() const noexcept { return instantiated; }
    inline void SetInstantiated() noexcept { instantiated = true; }
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const override;
    ClassTypeSymbol* ClassTemplate(Context* context) const;
    std::string SimpleName(Context* context) override { return ClassTemplate(context)->SimpleName(context); }
    std::string GroupName(Context* context) override;
    std::string FullName(Context* context) const override;
    std::string IrName(Context* context) const override;
    void SetClassTemplate(ClassTypeSymbol* classTemplate_, Context* context) noexcept;
    const std::vector<Symbol*>& TemplateArguments(Context* context) const;
    void AddTemplateArgument(Symbol* templateArgument);
    void AddInstantiatedVirtualFunctionSpecialization(FunctionSymbol* specialization);
    TypeSymbol* UnifyTemplateArgumentType(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, 
        TemplateParamEqual>& templateParameterMap, const soul::ast::FullSpan& fullSpan, Context* context) noexcept override;
    bool IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const override;
    FunctionSymbol* GetMatchingVirtualFunctionSpecialization(FunctionSymbol* newcomer, Context* context) const noexcept;
    bool ContainsVirtualFunctionSpecialization(FunctionSymbol* specialization) const noexcept;
    FunctionSymbol* Destructor(Context* context);
    TypeSymbol* FinalType(const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline void SetDestructor(FunctionSymbol* destructor_) noexcept { destructor = destructor_; }
    inline bool InstantiatingDestructor() const noexcept { return instantiatingDestructor; }
    inline void SetInstantiatingDestructor(bool instantiating) noexcept { instantiatingDestructor = instantiating; }
    bool HasForwardClassDeclarationSymbol(Context* context) const override;
    void Write(Writer& writer);
    void Read(Reader& reader);
private:
    mutable ClassTypeSymbol* classTemplate;
    mutable std::vector<Symbol*> templateArguments;
    SymbolId classTemplateId;
    std::vector<SymbolId> templateArgumentIds;
    mutable bool templateArgumentsSet;
    bool instantiated;
    std::vector<FunctionSymbol*> instantiatedVirtualFunctionSpecializations;
    std::vector<SymbolId> instantiatedVirtualFunctionSpecializationIds;
    FunctionSymbol* destructor;
    SymbolId destructorId;
    bool instantiatingDestructor;
};

struct MemFnKey
{
    MemFnKey();
    MemFnKey(FunctionSymbol* memFn, const std::vector<TypeSymbol*> templateArguments);
    SymbolId memFnId;
    std::vector<SymbolId> templateArgumentIds;
};

struct MemFnKeyEqual
{
    bool operator()(const MemFnKey& left, const MemFnKey& right) const noexcept;
};

struct MemFnKeyHash
{
    size_t operator()(const MemFnKey& key) const noexcept;
};

class ClassTemplateRepository
{
public:
    ClassTemplateRepository();
    FunctionDefinitionSymbol* GetFunctionDefinition(const MemFnKey& key, Context* context) const noexcept;
    void AddFunctionDefinition(const MemFnKey& key, FunctionDefinitionSymbol* functionDefinitionSymbol);
private:
    std::unordered_map<MemFnKey, SymbolId, MemFnKeyHash, MemFnKeyEqual> memFnKeyMap;
};

ClassTemplateSpecializationSymbol* InstantiateClassTemplate(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArgs,
    const soul::ast::FullSpan& fullSpan, Context* context);
ClassTemplateSpecializationSymbol* InstantiateClassTemplate(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArgs,
    const soul::ast::FullSpan& fullSpan, Context* context, bool createNew);
FunctionSymbol* InstantiateMemFnOfClassTemplate(FunctionSymbol* memFn,
    ClassTemplateSpecializationSymbol* classTemplateSpecialization, 
    const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>& templateParameterMap,
    const soul::ast::FullSpan& fullSpan, Context* context);
CompoundTypeSymbol* GetCompoundSpecializationArgType(TypeSymbol* specialization, Index index, Context* context) noexcept;
ClassTemplateSpecializationSymbol* GetClassTemplateSpecializationArgType(TypeSymbol* specialization, Index index, Context* context) noexcept;
void InstantiateDestructor(ClassTemplateSpecializationSymbol* specialization, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbol
