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

const std::int32_t vtabClassIdElementCount = 2;
const std::int32_t vtabFunctionSectionOffset = 2;

class FunctionSymbol;
class FunctionDefinitionSymbol;
class ClassGroupSymbol;
class Emitter;

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

enum class ClassTypeSymbolFlags : std::uint8_t
{
    none = 0, objectLayoutComputed = 1 << 0, hasUserDefinedDestructor = 1 << 1, hasUserDefinedConstructor = 1 << 2, vtabInitialized = 1 << 3, vtabGenerated = 1 << 4,
    resolved = 1 << 5
};

constexpr ClassTypeSymbolFlags operator|(ClassTypeSymbolFlags left, ClassTypeSymbolFlags right) noexcept
{
    return static_cast<ClassTypeSymbolFlags>(std::uint8_t(left) | std::uint8_t(right));
}

constexpr ClassTypeSymbolFlags operator&(ClassTypeSymbolFlags left, ClassTypeSymbolFlags right) noexcept
{
    return static_cast<ClassTypeSymbolFlags>(std::uint8_t(left) & std::uint8_t(right));
}

constexpr ClassTypeSymbolFlags operator~(ClassTypeSymbolFlags flags) noexcept
{
    return static_cast<ClassTypeSymbolFlags>(~std::uint8_t(flags));
}

class ClassTypeSymbol : public TypeSymbol
{
public:
    ClassTypeSymbol(Module* module__, SymbolId id_);
    ClassTypeSymbol(Module* module__, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    TemplateDeclarationSymbol* ParentTemplateDeclaration(Context* context) const noexcept;
    bool IsTemplate(Context* context) const noexcept;
    inline bool GetFlag(ClassTypeSymbolFlags flag) const noexcept { return (flags & flag) != ClassTypeSymbolFlags::none; }
    inline void SetFlag(ClassTypeSymbolFlags flag) noexcept { flags = flags | flag; }
    inline void ResetFlag(ClassTypeSymbolFlags flag) noexcept { flags = flags & ~flag; }
    inline bool HasUserDefinedDestructor() const noexcept { return GetFlag(ClassTypeSymbolFlags::hasUserDefinedDestructor); }
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept override;
    inline int Level() const noexcept { return level; }
    inline void SetLevel(int level_) noexcept { level = level_; }
    std::int32_t NextFunctionIndex() noexcept;
    FunctionSymbol* GetFunctionByIndex(std::int32_t functionIndex) const noexcept;
    void MapFunction(FunctionSymbol* function);
    void SetMemFnDefSymbol(FunctionDefinitionSymbol* memFnDefSymbol);
    FunctionDefinitionSymbol* GetMemFnDefSymbol(int32_t defIndex) const noexcept;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    virtual ClassGroupSymbol* Group(Context* context) const;
    inline void SetGroup(ClassGroupSymbol* group_) noexcept { group = group_; }
    void MakeVTab(Context* context, const soul::ast::FullSpan& fullSpan);
    void InitVTab(std::vector<FunctionSymbol*>& vtab, Context* context, const soul::ast::FullSpan& fullSpan, bool clear);
    std::vector<ClassTypeSymbol*> VPtrHolderClasses() const;
    inline std::int32_t VPtrIndex() const noexcept { return vptrIndex; }
    inline void SetVPtrIndex(std::int32_t vptrIndex_) noexcept { vptrIndex = vptrIndex_; }
    inline std::int32_t DeltaIndex() const noexcept { return deltaIndex; }
    inline void SetDeltaIndex(std::int32_t deltaIndex_) noexcept { deltaIndex = deltaIndex_; }
    otava::intermediate::Type* VPtrType(Emitter& emitter) const noexcept;
private:
    ClassTypeSymbolFlags flags;
    std::int32_t level;
    SymbolId groupId;
    mutable ClassGroupSymbol* group;
    std::vector<FunctionSymbol*> vtab;
    std::int32_t vtabSize;
    std::int32_t vptrIndex;
    std::int32_t deltaIndex;
    std::string vtabName;
};

class ForwardClassDeclarationSymbol : public TypeSymbol
{
public:
    ForwardClassDeclarationSymbol(Module* module__, SymbolId id_);
    ForwardClassDeclarationSymbol(Module* module__, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept override;
    TypeSymbol* FinalType(const soul::ast::FullSpan& fullSpan, Context* context) override;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    ClassTypeSymbol* classTypeSymbol;
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
