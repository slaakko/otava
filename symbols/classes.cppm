// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.classes;

import std;
import otava.symbols.scope;
import otava.symbols.type_symbol;
import otava.symbols.function_kind;
import otava.ast.node;
import otava.ast.statement;
import otava.ast.classes;
import otava.ast.function;
import soul.ast.span;

export namespace otava::symbols {

const std::int32_t undefinedIndex = 0;
const std::int32_t defaultCtorIndex = -1;
const std::int32_t copyCtorIndex = -2;
const std::int32_t moveCtorIndex = -3;
const std::int32_t copyAssignmentIndex = -4;
const std::int32_t moveAssignmentIndex = -5;
const std::int32_t destructorIndex = -6;

const std::int32_t vtabClassIdElementCount = 1;

class FunctionSymbol;
class FunctionDefinitionSymbol;
class ClassGroupSymbol;
class Emitter;
class VariableSymbol;

std::int32_t GetSpecialFunctionIndex(SpecialFunctionKind specialFunctionKind) noexcept;

using RecordedParseCompoundStatementFn = void (*)(otava::ast::CompoundStatementNode* compoundStatementNode, Context* context);

void SetRecordedParseCompoundStatementFn(RecordedParseCompoundStatementFn fn) noexcept;

using RecordedParseCtorInitializerFn = void (*)(otava::ast::ConstructorInitializerNode* ctorInitializerNode, Context* context);

void SetRecordedParseCtorInitializerFn(RecordedParseCtorInitializerFn fn) noexcept;

class TemplateDeclarationSymbol;
class TemplateParameterSymbol;
class BoundCompileUnitNode;
class BoundFunctionNode;
class BoundExpressionNode;
class BoundConstructTemporaryNode;
class BoundConstructExpressionNode;
class BoundFunctionCallNode;

enum class ClassKind : std::uint8_t
{
    class_, struct_, union_
};

constexpr std::uint8_t ToUnderlying(ClassKind classKind)
{
    return std::uint8_t(classKind);
}

enum class ClassTypeSymbolFlags : std::uint8_t
{
    none = 0, objectLayoutComputed = 1 << 0, hasUserDefinedDestructor = 1 << 1, hasUserDefinedConstructor = 1 << 2, vtabInitialized = 1 << 3, vtabGenerated = 1 << 4,
    resolved = 1 << 5
};

constexpr std::uint8_t ToUnderlying(ClassTypeSymbolFlags flags)
{
    return std::uint8_t(flags);
}

constexpr ClassTypeSymbolFlags operator|(ClassTypeSymbolFlags left, ClassTypeSymbolFlags right) noexcept
{
    return static_cast<ClassTypeSymbolFlags>(ToUnderlying(left) | ToUnderlying(right));
}

constexpr ClassTypeSymbolFlags operator&(ClassTypeSymbolFlags left, ClassTypeSymbolFlags right) noexcept
{
    return static_cast<ClassTypeSymbolFlags>(ToUnderlying(left) & ToUnderlying(right));
}

constexpr ClassTypeSymbolFlags operator~(ClassTypeSymbolFlags flags) noexcept
{
    return static_cast<ClassTypeSymbolFlags>(~ToUnderlying(flags));
}

class ClassTypeSymbol : public TypeSymbol
{
public:
    ClassTypeSymbol(Module* module__, SymbolId id_);
    ClassTypeSymbol(Module* module__, SymbolId id_, const std::string& name_);
    ~ClassTypeSymbol();
    inline void SetClassKind(ClassKind classKind_) noexcept { classKind = classKind_; }
    inline ClassKind GetClassKind() const noexcept { return classKind; }
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    TemplateDeclarationSymbol* ParentTemplateDeclaration(Context* context) const noexcept;
    bool IsTemplate(Context* context) const noexcept;
    inline bool GetFlag(ClassTypeSymbolFlags flag) const noexcept { return (flags & flag) != ClassTypeSymbolFlags::none; }
    inline void SetFlag(ClassTypeSymbolFlags flag) noexcept { flags = flags | flag; }
    inline void ResetFlag(ClassTypeSymbolFlags flag) noexcept { flags = flags & ~flag; }
    inline bool ObjectLayoutComputed() const noexcept { return GetFlag(ClassTypeSymbolFlags::objectLayoutComputed); }
    inline void SetObjectLayoutComputed() noexcept { SetFlag(ClassTypeSymbolFlags::objectLayoutComputed); }
    inline void ResetObjectLayoutComputed() noexcept { ResetFlag(ClassTypeSymbolFlags::objectLayoutComputed); }
    inline bool HasUserDefinedDestructor() const noexcept { return GetFlag(ClassTypeSymbolFlags::hasUserDefinedDestructor); }
    inline void SetHasUserDefinedDestructor() noexcept { SetFlag(ClassTypeSymbolFlags::hasUserDefinedDestructor); }
    inline bool HasUserDefinedConstructor() const noexcept { return GetFlag(ClassTypeSymbolFlags::hasUserDefinedConstructor); }
    inline void SetHasUserDefinedConstructor() noexcept { SetFlag(ClassTypeSymbolFlags::hasUserDefinedConstructor); }
    inline bool VTabInitialized() const noexcept { return GetFlag(ClassTypeSymbolFlags::vtabInitialized); }
    inline void SetVTabInitialized() noexcept { SetFlag(ClassTypeSymbolFlags::vtabInitialized); }
    Cardinality Arity(Context* context) noexcept;
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const override;
    TypeSymbol* Specialization(Context* context);
    void SetSpecialization(TypeSymbol* specialization_, Context* context) noexcept;
    inline int Level() const noexcept { return level; }
    inline void SetLevel(int level_) noexcept { level = level_; }
    std::int32_t NextFunctionIndex() noexcept;
    FunctionSymbol* GetFunctionByIndex(std::int32_t functionIndex) const noexcept;
    void MapFunction(FunctionSymbol* function);
    void UnmapFunction(FunctionSymbol* function);
    void SetMemFnDefSymbol(FunctionDefinitionSymbol* memFnDefSymbol);
    FunctionDefinitionSymbol* GetMemFnDefSymbol(int32_t defIndex) const noexcept;
    inline const std::map<std::int32_t, FunctionDefinitionSymbol*>& MemFnDefSymbolMap() const noexcept { return memFnDefSymbolMap; }
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    std::string IrName(Context* context) const override;
    void ComputeVTabName(Context* context);
    virtual std::string GroupName(Context* context);
    virtual ClassGroupSymbol* Group(Context* context) const;
    inline void SetGroup(ClassGroupSymbol* group_) noexcept { group = group_; }
    bool IsPolymorphic(Context* context) const noexcept override;
    void MakeVTab(Context* context, const soul::ast::FullSpan& fullSpan);
    void InitVTab(std::vector<FunctionSymbol*>& vtab, Context* context, const soul::ast::FullSpan& fullSpan, bool clear);
    const std::vector<FunctionSymbol*>& VTab() const noexcept { return vtab; }
    std::vector<ClassTypeSymbol*> VPtrHolderClasses() const;
    otava::intermediate::Value* GetVTabVariable(Emitter& emitter, Context* context);
    inline std::int32_t VPtrIndex() const noexcept { return vptrIndex; }
    inline void SetVPtrIndex(std::int32_t vptrIndex_) noexcept { vptrIndex = vptrIndex_; }
    inline std::int32_t DeltaIndex() const noexcept { return deltaIndex; }
    inline void SetDeltaIndex(std::int32_t deltaIndex_) noexcept { deltaIndex = deltaIndex_; }
    inline void SetNextMemFnDefIndex(int32_t defIndex) noexcept { nextMemFnDefIndex = std::max(defIndex, nextMemFnDefIndex); }
    inline int32_t NextMemFnDefIndex() const noexcept { return nextMemFnDefIndex; }
    otava::intermediate::Type* VPtrType(Emitter& emitter) const noexcept;
    void AddBaseClass(ClassTypeSymbol* baseClass, const soul::ast::FullSpan& fullSpan, Context* context);
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    const std::vector<ClassTypeSymbol*>& BaseClasses(Context* context) const;
    const std::vector<VariableSymbol*>& MemberVariables(Context* context) const;
    const std::vector<VariableSymbol*>& StaticMemberVariables(Context* context) const;
    const std::vector<FunctionSymbol*>& MemberFunctions(Context* context) const;
    bool HasBaseClass(TypeSymbol* baseClass, int& distance, Context* context) const noexcept override;
    bool HasPolymorphicBaseClass(Context* context) const noexcept;
    bool IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const override;
    inline const std::vector<TypeSymbol*>& ObjectLayout() const noexcept { return objectLayout; }
    void MakeObjectLayout(const soul::ast::FullSpan& fullSpan, Context* context);
    FunctionSymbol* CopyCtor() const noexcept { return copyCtor; }
    void GenerateCopyCtor(const soul::ast::FullSpan& fullSpan, Context* context);
    void ResetCopyCtor() noexcept { copyCtor = nullptr; }
    Cardinality TotalMemberCount() const noexcept;
    FunctionSymbol* GetConversionFunction(TypeSymbol* type, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::pair<bool, std::int64_t> Delta(ClassTypeSymbol* base, Emitter& emitter, Context* context) noexcept;
    std::string VTabName(Context* context) const;
private:
    ClassTypeSymbolFlags flags;
    ClassKind classKind;
    mutable std::vector<ClassTypeSymbol*> baseClasses;
    std::vector<SymbolId> baseClassIds;
    mutable std::vector<VariableSymbol*> memberVariables;
    std::vector<SymbolId> memberVariableIds;
    mutable std::vector<VariableSymbol*> staticMemberVariables;
    std::vector<SymbolId> staticMemberVariableIds;
    mutable std::vector<FunctionSymbol*> memberFunctions;
    std::vector<SymbolId> memberFunctionIds;
    mutable std::vector<TypeSymbol*> objectLayout;
    std::vector<SymbolId> objectLayoutIds;
    mutable std::vector<FunctionSymbol*> conversionFunctions;
    std::vector<SymbolId> conversionFunctionIds;
    std::int32_t level;
    SymbolId groupId;
    mutable ClassGroupSymbol* group;
    mutable std::vector<FunctionSymbol*> vtab;
    std::vector<SymbolId> vtabIds;
    std::int32_t vptrIndex;
    std::int32_t deltaIndex;
    std::int32_t currentFunctionIndex;
    std::map<std::int32_t, FunctionSymbol*> functionIndexMap;
    std::map<std::int32_t, FunctionDefinitionSymbol*> memFnDefSymbolMap;
    StringOffset vtabNameOffset;
    mutable TypeSymbol* specialization;
    SymbolId specializationId;
    int32_t nextMemFnDefIndex;
    FunctionSymbol* copyCtor;
    std::vector<std::unique_ptr<Symbol>> tempVars;
    mutable bool contentFetched;
    bool destructing;
    void GetContent(Context* context) const;
};

class ForwardClassDeclarationSymbol : public TypeSymbol
{
public:
    ForwardClassDeclarationSymbol(Module* module__, SymbolId id_);
    ForwardClassDeclarationSymbol(Module* module__, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const override;
    TemplateDeclarationSymbol* ParentTemplateDeclaration(Context* context) const noexcept;
    Cardinality Arity(Context* context) noexcept;
    ClassGroupSymbol* Group(Context* context) const;
    inline void SetGroup(ClassGroupSymbol* group_) noexcept { group = group_; }
    inline void SetClassKind(ClassKind classKind_) noexcept { classKind = classKind_; }
    inline ClassKind GetClassKind() const noexcept { return classKind; }
    TypeSymbol* FinalType(const soul::ast::FullSpan& fullSpan, Context* context) override;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline void SetClassTypeSymbol(ClassTypeSymbol* classTypeSymbol_) { classTypeSymbol = classTypeSymbol_; }
    ClassTypeSymbol* GetClassTypeSymbol(Context* context) const;
    TypeSymbol* Specialization(Context* context);
    void SetSpecialization(TypeSymbol* specialization_, Context* context) noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    mutable ClassTypeSymbol* classTypeSymbol;
    SymbolId classTypeSymbolId;
    ClassKind classKind;
    TypeSymbol* specialization;
    SymbolId specializationId;
    mutable ClassGroupSymbol* group;
    SymbolId groupId;
};

class ClassParsingMap
{
public:
    ClassParsingMap();
    inline const std::vector<FunctionSymbol*>& Functions() const noexcept { return fns; }
    otava::ast::FunctionDefinitionNode* GetFunctionDefnitionNode(FunctionSymbol* fn)  const noexcept;
    void MapFunctionDefinitionNode(FunctionSymbol* fn, otava::ast::FunctionDefinitionNode* node);
private:
    std::vector<FunctionSymbol*> fns;
    std::map<FunctionSymbol*, otava::ast::FunctionDefinitionNode*> map;
};

void BeginClass(otava::ast::Node* node, Context* context);
void EndClass(otava::ast::Node* node, Context* context);
void ProcessElaboratedClassDeclaration(otava::ast::Node* node, otava::symbols::Context* context);
void SetCurrentAccess(otava::ast::Node* node, otava::symbols::Context* context);
void GetClassAttributes(otava::ast::Node* node, std::string& name, otava::symbols::ClassKind& kind, TypeSymbol*& specialization, Context* context);
std::vector<ClassTypeSymbol*> ResolveBaseClasses(otava::ast::Node* node, Context* context);
void ParseInlineMemberFunctions(otava::ast::Node* classSpecifierNode, ClassTypeSymbol* classTypeSymbol, Context* context);
void ParseInlineMemberFunction(Context* context, FunctionSymbol* memfn);
Symbol* GenerateDestructor(ClassTypeSymbol* classTypeSymbol, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
void GenerateDestructors(BoundCompileUnitNode* compileUnit, otava::symbols::Context* context);
BoundFunctionCallNode* MakeDestructorCall(ClassTypeSymbol* cls, BoundExpressionNode* arg, FunctionDefinitionSymbol* destructor,
    const soul::ast::FullSpan& fullSpan, Context* context);
void CheckGenerateTemporaryDestructorCall(BoundConstructTemporaryNode* constructTemporary, BoundExpressionNode* arg, Context* context);
void ThrowMemberDeclarationParsingError(const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
void ThrowStatementParsingError(const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);

struct ClassLess
{
    bool operator()(ClassTypeSymbol* left, ClassTypeSymbol* right) const noexcept;
};

std::pair<bool, std::int64_t> Delta(ClassTypeSymbol* left, ClassTypeSymbol* right, Emitter& emitter, Context* context) noexcept;

} // namespace otava::symbol
