// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.type_symbol;

import std;
import otava.symbols.ast_node_io;
import otava.symbols.derivations;
import otava.symbols.container_symbol;
import otava.symbols.template_param_compare;
import otava.ast.node;
import otava.ast.statement;
import otava.intermediate.types;

export namespace otava::symbols {

class Emitter;
class TemplateParameterSymbol;

class TypeSymbol : public ContainerSymbol
{
public:
    TypeSymbol(Module* module_, SymbolId id_);
    TypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    virtual bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const { return true; }
    virtual TypeSymbol* GetBaseType(Context* context) { return this; }
    inline bool IsCompoundType() const noexcept { return Kind() == SymbolKind::compoundTypeSymbol; }
    bool IsAutoTypeSymbol() const noexcept;
    bool IsPointerType() const noexcept;
    bool IsArrayType() const noexcept;
    bool IsConstType() const noexcept;
    bool IsLValueRefType() const noexcept;
    bool IsRValueRefType() const noexcept;
    bool IsReferenceType() const noexcept;
    virtual TypeSymbol* PlainType(Context* context) { return this; }
    virtual TypeSymbol* FinalType(const soul::ast::FullSpan& fullSpan, Context* context) { return this; }
    virtual TypeSymbol* DirectType(Context* context) { return this; }
    TypeSymbol* AddConst(Context* context);
    TypeSymbol* RemoveConst(Context* context);
    TypeSymbol* AddPointer(Context* context);
    TypeSymbol* RemovePointer(Context* context);
    TypeSymbol* AddLValueRef(Context* context);
    TypeSymbol* RemoveLValueRef(Context* context);
    TypeSymbol* AddRValueRef(Context* context);
    TypeSymbol* RemoveRValueRef(Context* context);
    TypeSymbol* RemoveReference(Context* context);
    TypeSymbol* RemoveRefOrPtr(Context* context);
    virtual bool HasBaseClass(TypeSymbol* baseClass, int& distance, Context* context) const noexcept { return false; }
    virtual bool IsVoidType() const noexcept { return false; }
    virtual bool IsBoolType() const noexcept { return false; }
    virtual bool IsIntType() const noexcept { return false; }
    virtual bool IsUnsignedShortType() const noexcept { return false; }
    virtual bool IsDoubleType() const noexcept { return false; }
    virtual bool IsFloatType() const noexcept { return false; }
    virtual bool IsNullPtrType() const noexcept { return false; }
    virtual bool IsVoidPtrType() const noexcept { return false; }
    virtual bool IsIntegralType() const noexcept { return false; }
    virtual bool IsPolymorphic(Context* context) const noexcept { return false; }
    virtual bool IsFunctionType() const noexcept { return false; }
    virtual bool IsBasicStringCharType(Context* context) noexcept { return false; }
    virtual bool IsBasicStringChar16Type(Context* context) noexcept { return false; }
    virtual bool IsBasicStringChar32Type(Context* context) noexcept { return false; }
    bool IsConstCharPtrType(Context* context) 
    {
        return IsConstType() &&
            IsPointerType() &&
            PointerCount() == 1 &&
            (GetBaseType(context)->IsCharTypeSymbol() || GetBaseType(context)->IsChar8TypeSymbol());
    }
    bool IsConstChar16PtrType(Context* context) { return IsConstType() && IsPointerType() && PointerCount() == 1 && GetBaseType(context)->IsChar16TypeSymbol(); }
    inline bool IsConstChar32PtrType(Context* context) { return IsConstType() && IsPointerType() && PointerCount() == 1 && GetBaseType(context)->IsChar32TypeSymbol(); }
    inline bool IsFunctionPtrType(Context* context) { return IsPointerType() && PointerCount() == 1 && GetBaseType(context)->IsFunctionType(); }
    virtual int Rank(Context* context) { return -1; }
    virtual bool IsSignedIntegerType() const noexcept { return false; }
    virtual bool IsUnsignedIntegerType() const noexcept { return false; }
    virtual int PointerCount() const noexcept { return 0; }
    virtual bool HasForwardClassDeclarationSymbol(Context* context) const { return false; }
    virtual otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual Derivations GetDerivations() const noexcept { return Derivations::none; }
    virtual TypeSymbol* RemoveDerivations(Derivations sourceDerivations, Context* context);
    virtual TypeSymbol* Unify(TypeSymbol* argType, Context* context);
    virtual TypeSymbol* UnifyTemplateArgumentType(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, 
        TemplateParamEqual>& templateParameterMap, const soul::ast::FullSpan& fullSpan, Context* context);
};

class ClassGroupTypeSymbol : public TypeSymbol
{
public:
    ClassGroupTypeSymbol(Module* module_, SymbolId id_);
    ClassGroupTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsExportSymbol(Context* context) const noexcept override { return false; }
    inline void SetClassGroup(ClassGroupSymbol* classGroup_) noexcept { classGroup = classGroup_; }
    inline ClassGroupSymbol* GetClassGroup() const noexcept { return classGroup; }
private:
    ClassGroupSymbol* classGroup;
};

class AliasGroupTypeSymbol : public TypeSymbol
{
public:
    AliasGroupTypeSymbol(Module* module_, SymbolId id_);
    AliasGroupTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsExportSymbol(Context* context) const noexcept override { return false; }
    inline void SetAliasGroup(AliasGroupSymbol* aliasGroup_) noexcept { aliasGroup = aliasGroup_; }
    inline AliasGroupSymbol* GetAliasGroup() const noexcept { return aliasGroup; }
private:
    AliasGroupSymbol* aliasGroup;
};

class FunctionGroupTypeSymbol : public TypeSymbol
{
public:
    FunctionGroupTypeSymbol(Module* module_, SymbolId id_);
    FunctionGroupTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsExportSymbol(Context* context) const noexcept override { return false; }
    inline void SetFunctionGroup(FunctionGroupSymbol* functionGroup_) noexcept { functionGroup = functionGroup_; }
    inline FunctionGroupSymbol* GetFunctionGroup() const noexcept { return functionGroup; }
private:
    FunctionGroupSymbol* functionGroup;
};

class NestedTypeSymbol : public TypeSymbol
{
public:
    NestedTypeSymbol(Module* module_, SymbolId id_);
    NestedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
};

class DependentTypeSymbol : public TypeSymbol
{
public:
    DependentTypeSymbol(Module* module_, SymbolId id_);
    DependentTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    void ResetNode(otava::ast::Node* node_);
    inline otava::ast::Node* GetNode() const noexcept { return node.get(); }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    std::unique_ptr<otava::ast::Node> node;
    AstNodeHeader astNodeHeader;
};

TypeSymbol* ConvertRefToPtrType(TypeSymbol* type, Context* context);

std::unique_ptr<otava::ast::DeclarationStatementNode> DeclarationToAst(TypeSymbol* type, const std::string& variableName, otava::ast::Node* initializer,
    const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
