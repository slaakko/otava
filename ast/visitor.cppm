// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.visitor;

import std;
import soul.ast.span;

export namespace otava::ast {

// Base classes;
class Node;
class CompoundNode;
class UnaryNode;
class BinaryNode;
class SequenceNode;
class ListNode;
// Attribute:
class AttributeSpecifierSequenceNode;
class AttributeSpecifierNode;
class AttributeUsingPrefixNode;
class AttributeNode;
class AttributeScopedTokenNode;
class AttributeArgumentsNode;
class BalancedTokenSequenceNode;
class TokenNode;
class AlignmentSpecifierNode;
// Class:
class ClassSpecifierNode;
class ClassHeadNode;
class BaseClauseNode;
class BaseSpecifierListNode;
class BaseSpecifierNode;
class BeginAccessGroupNode;
class MemberDeclarationNode;
class MemberDeclaratorListNode;
class ConstructorNode;
class ConstructorInitializerNode;
class MemberInitializerListNode;
class MemberInitializerNode;
class VirtSpecifierSequenceNode;
class ClassNode;
class StructNode;
class UnionNode;
class PublicNode;
class ProtectedNode;
class PrivateNode;
class VirtualNode;
class OverrideNode;
class FinalNode;
class PureSpecifierNode;
// Concept:
class ConceptDefinitionNode;
class RequiresExprNode;
class RequirementBodyNode;
class SimpleRequirementNode;
class TypeRequirementNode;
class CompoundRequirementNode;
class ReturnTypeRequirementNode;
class NestedRequirementNode;
class TypeConstraintNode;
class RequiresClauseNode;
// Declaration:
class DeclarationSequenceNode;
class SimpleDeclarationNode;
class AsmDeclarationNode;
class AsmNode;
class LinkageSpecificationNode;
class NamespaceDefinitionNode;
class NamespaceBodyNode;
class NamespaceAliasDefinitionNode;
class UsingDeclarationNode;
class UsingNode;
class UsingDeclaratorListNode;
class UsingEnumDeclarationNode;
class UsingDirectiveNode;
class NamespaceNode;
class StaticAssertDeclarationNode;
class StaticAssertNode;
class AliasDeclarationNode;
class EmptyDeclarationNode;
class AttributeDeclarationNode;
class InitDeclaratorListNode;
class InitDeclaratorNode;
class TrailingFunctionDeclaratorNode;
class ParenthesizedDeclaratorNode;
class AbstractDeclaratorNode;
class DeclSpecifierSequenceNode;
class FriendNode;
class TypedefNode;
class ConstExprNode;
class ConstEvalNode;
class ConstInitNode;
class InlineNode;
class StaticNode;
class ThreadLocalNode;
class ExternNode;
class MutableNode;
class ExplicitNode;
class ConditionalExplicitNode;
class QualifiedPtrNode;
class PtrOperatorNode;
class PtrDeclaratorNode;
class PackDeclaratorIdNode;
class ArrayDeclaratorNode;
class FunctionDeclaratorNode;
class PrefixNode;
class TrailingQualifiersNode;
class TrailingAttributesNode;
class NoexceptSpecifierNode;
class ThrowSpecifierNode;
class ThrowExprNode;
// Enum:
class EnumSpecifierNode;
class EnumHeadNode;
class EnumBaseNode;
class EnumClassNode;
class EnumStructNode;
class EnumNode;
class EnumeratorDefinitionNode;
class EnumeratorNode;
class ElaboratedEnumSpecifierNode;
class OpaqueEnumDeclarationNode;
// Expression:
class BinaryExprNode;
class UnaryExprNode;
class ExpressionListNode;
class AssignmentInitNode;
class YieldExprNode;
class ConditionalExprNode;
class AssignNode;
class PlusAssignNode;
class MinusAssignNode;
class MulAssignNode;
class DivAssignNode;
class ModAssignNode;
class XorAssignNode;
class AndAssignNode;
class OrAssignNode;
class ShiftLeftAssignNode;
class ShiftRightAssignNode;
class DisjunctionNode;
class ConjunctionNode;
class InclusiveOrNode;
class ExclusiveOrNode;
class AndNode;
class EqualNode;
class NotEqualNode;
class LessNode;
class GreaterNode;
class LessOrEqualNode;
class GreaterOrEqualNode;
class CompareNode;
class ShiftLeftNode;
class ShiftRightNode;
class PlusNode;
class MinusNode;
class MulNode;
class DivNode;
class ModNode;
class DotStarNode;
class ArrowStarNode;
class CastExprNode;
class DerefNode;
class AddrOfNode;
class NotNode;
class ComplementNode;
class PrefixIncNode;
class PrefixDecNode;
class AwaitExprNode;
class SizeOfTypeExprNode;
class SizeOfPackExprNode;
class SizeOfUnaryExprNode;
class AlignOfExprNode;
class NoexceptExprNode;
class OpNewCall;
class NewExprNode;
class NewPlacementNode;
class ParenNewTypeIdNode;
class NewTypeIdNode;
class ArrayDeletePtrNode;
class DeletePtrNode;
class SubscriptExprNode;
class InvokeExprNode;
class PairNode;
class DotNode;
class ArrowNode;
class MemberExprNode;
class PostfixIncExprNode;
class PostfixDecExprNode;
class TypeIdExprNode;
class DynamicCastNode;
class StaticCastNode;
class ReinterpretCastNode;
class ConstCastNode;
class CppCastExprNode;
class ThisNode;
class ParenthesizedExprNode;
class FoldExprNode;
class NewDeclaratorNode;
class ArrayNewDeclaratorNode;
class NewInitializerNode;
class BracedInitListNode;
class DesignatedInitializerNode;
class DesignatorNode;
// Function:
class FunctionDefinitionNode;
class FunctionBodyNode;
class DefaultNode;
class DeleteNode;
class DefaultedOrDeletedFunctionNode;
class NoDeclSpecFunctionDeclarationNode;
class OperatorNode;
class NewArrayOpNode;
class NewOpNode;
class DeleteArrayOpNode;
class DeleteOpNode;
class CoAwaitOpNode;
class InvokeOpNode;
class SubscriptOpNode;
class OperatorFunctionIdNode;
class ConversionFunctionIdNode;
class ConversionTypeIdNode;
class ConversionDeclaratorNode;
class DestructorIdNode;
class ParameterNode;
class ParameterListNode;
class NoexceptNode;
class FunctionTryBlockNode;
// Identifier:
class IdentifierNode;
class UnnamedNode;
class ColonColonNode;
class NestedNameSpecifierNode;
class QualifiedIdNode;
class IdentifierListNode;
class ModuleNameNode;
// Lambda:
class LambdaExpressionNode;
class LambdaIntroducerNode;
class LambdaCaptureNode;
class DefaultRefCaptureNode;
class DefaultCopyCaptureNode;
class ByRefCaptureNode;
class SimpleCaptureNode;
class CurrentObjectCopyCapture;
class CurrentObjectByRefCapture;
class InitCaptureNode;
class LambdaDeclaratorNode;
class LambdaSpecifiersNode;
class LambdaTemplateParamsNode;
// Literal:
class IntegerLiteralNode;
class FloatingLiteralNode;
class CharacterLiteralNode;
class StringLiteralNode;
class RawStringLiteralNode;
class BooleanLiteralNode;
class NullPtrLiteralNode;
class UserDefinedLiteraNode;
class LiteralOperatorIdNode;
// Module:
class ModuleDeclarationNode;
class ExportDeclarationNode;
class ExportNode;
class ImportNode;
class ImportDeclarationNode;
class ModulePartitionNode;
class ModuleNode;
class GlobalModuleFragmentNode;
class PrivateModuleFragmentNode;
class AngleHeaderName;
class QuoteHeaderName;
// Qualifier:
class ConstNode;
class VolatileNode;
class LvalueRefNode;
class RvalueRefNode;
class PtrNode;
class CVQualifierSequenceNode;
// SimpleType:
class CharNode;
class Char8Node;
class Char16Node;
class Char32Node;
class WCharNode;
class BoolNode;
class ShortNode;
class IntNode;
class LongNode;
class SignedNode;
class UnsignedNode;
class FloatNode;
class DoubleNode;
class VoidNode;
// Statement:
class LabeledStatementNode;
class CaseStatementNode;
class DefaultStatementNode;
class CompoundStatementNode;
class SequenceStatementNode;
class IfStatementNode;
class SwitchStatementNode;
class WhileStatementNode;
class DoStatementNode;
class RangeForStatementNode;
class ForRangeDeclarationNode;
class StructuredBindingNode;
class ForStatementNode;
class BreakStatementNode;
class ContinueStatementNode;
class ReturnStatementNode;
class CoReturnStatementNode;
class GotoStatementNode;
class TryStatementNode;
class HandlerSequenceNode;
class HandlerNode;
class ExceptionDeclarationNode;
class ExpressionStatementNode;
class DeclarationStatementNode;
class InitConditionNode;
class BoundStatementNode;
// Template:
class TemplateDeclarationNode;
class TemplateHeadNode;
class TemplateParameterListNode;
class TypeParameterNode;
class TemplateIdNode;
class TypenameNode;
class DeductionGuideNode;
class ExplicitInstantiationNode;
class TemplateNode;
class ExplicitSpecializationNode;
// TranslationUnit:
class TranslationUnitNode;
class ModuleUnitNode;
// Type:
class TypeSpecifierSequenceNode;
class TypenameSpecifierNode;
class TypeIdNode;
class DefiningTypeIdNode;
class DefiningTypeSpecifierSequenceNode;
class TrailingReturnTypeNode;
class ElaboratedTypeSpecifierNode;
class DeclTypeSpecifierNode;
class PlaceholderTypeSpecifierNode;
// Punctuation:
class SemicolonNode;
class CommaNode;
class QuestNode;
class ColonNode;
class EllipsisNode;
class LParenNode;
class RParenNode;
class LBracketNode;
class RBracketNode;
class LBraceNode;
class RBraceNode;

class Visitor
{
public:
    virtual ~Visitor();

    void VisitSingleNode(Node& node);
    void VisitUnaryNode(UnaryNode& unaryNode);
    void VisitBinaryNode(BinaryNode& binaryNode);
    void VisitSequence(SequenceNode& sequenceNode);
    void VisitSequenceContent(SequenceNode& sequenceNode);
    void VisitList(ListNode& listNode);
    void VisitListContent(ListNode& listNode);

    virtual void BeginVisit(Node& node) {}
    virtual void EndVisit(Node& node) {}
    virtual void VisitIdentifier(const std::u32string& id, const soul::ast::Span& span) {}
    virtual void VisitKeyword(const std::string& keyword, const soul::ast::Span& span) {}
    virtual void VisitOperator(const std::string& symbol, const soul::ast::Span& span) {}
    virtual void VisitToken(const std::u32string& tokenStr, const soul::ast::Span& span) {}
    virtual void VisitLiteral(const std::u32string& rep, const soul::ast::Span& span) {}
    virtual void VisitHeaderName(const std::u32string& rep, const soul::ast::Span& span) {}

    //  Attribute:
    virtual void Visit(AttributeSpecifierSequenceNode& node) {}
    virtual void Visit(AttributeSpecifierNode& node) {}
    virtual void Visit(AttributeUsingPrefixNode& node) {}
    virtual void Visit(AttributeNode& node) {}
    virtual void Visit(AttributeScopedTokenNode& node) {}
    virtual void Visit(AttributeArgumentsNode& node) {}
    virtual void Visit(BalancedTokenSequenceNode& node) {}
    virtual void Visit(TokenNode& node) {}
    virtual void Visit(AlignmentSpecifierNode& node) {}
    //  Class:
    virtual void Visit(ClassSpecifierNode& node) {}
    virtual void Visit(ClassHeadNode& node) {}
    virtual void Visit(BaseClauseNode& node) {}
    virtual void Visit(BaseSpecifierListNode& node) {}
    virtual void Visit(BaseSpecifierNode& node) {}
    virtual void Visit(BeginAccessGroupNode& node) {}
    virtual void Visit(MemberDeclarationNode& node) {}
    virtual void Visit(MemberDeclaratorListNode& node) {}
    virtual void Visit(ConstructorNode& node) {}
    virtual void Visit(ConstructorInitializerNode& node) {}
    virtual void Visit(MemberInitializerListNode& node) {}
    virtual void Visit(MemberInitializerNode& node) {}
    virtual void Visit(VirtSpecifierSequenceNode& node) {}
    virtual void Visit(ClassNode& node) {}
    virtual void Visit(StructNode& node) {}
    virtual void Visit(UnionNode& node) {}
    virtual void Visit(PublicNode& node) {}
    virtual void Visit(ProtectedNode& node) {}
    virtual void Visit(PrivateNode& node) {}
    virtual void Visit(VirtualNode& node) {}
    virtual void Visit(OverrideNode& node) {}
    virtual void Visit(FinalNode& node) {}
    virtual void Visit(PureSpecifierNode& node) {}
    // Concept:
    virtual void Visit(ConceptDefinitionNode& node) {}
    virtual void Visit(RequiresExprNode& node) {}
    virtual void Visit(RequirementBodyNode& node) {}
    virtual void Visit(SimpleRequirementNode& node) {}
    virtual void Visit(TypeRequirementNode& node) {}
    virtual void Visit(CompoundRequirementNode& node) {}
    virtual void Visit(ReturnTypeRequirementNode& node) {}
    virtual void Visit(NestedRequirementNode& node) {}
    virtual void Visit(TypeConstraintNode& node) {}
    virtual void Visit(RequiresClauseNode& node) {}
    // Declaration:
    virtual void Visit(DeclarationSequenceNode& node) {}
    virtual void Visit(SimpleDeclarationNode& node) {}
    virtual void Visit(AsmDeclarationNode& node) {}
    virtual void Visit(AsmNode& node) {}
    virtual void Visit(LinkageSpecificationNode& node) {}
    virtual void Visit(NamespaceDefinitionNode& node) {}
    virtual void Visit(NamespaceBodyNode& node) {}
    virtual void Visit(NamespaceAliasDefinitionNode& node) {}
    virtual void Visit(UsingDeclarationNode& node) {}
    virtual void Visit(UsingNode& node) {}
    virtual void Visit(UsingDeclaratorListNode& node) {}
    virtual void Visit(UsingEnumDeclarationNode& node) {}
    virtual void Visit(UsingDirectiveNode& node) {}
    virtual void Visit(NamespaceNode& node) {}
    virtual void Visit(StaticAssertDeclarationNode& node) {}
    virtual void Visit(StaticAssertNode& node) {}
    virtual void Visit(AliasDeclarationNode& node) {}
    virtual void Visit(EmptyDeclarationNode& node) {}
    virtual void Visit(AttributeDeclarationNode& node) {}
    virtual void Visit(InitDeclaratorListNode& node) {}
    virtual void Visit(InitDeclaratorNode& node) {}
    virtual void Visit(TrailingFunctionDeclaratorNode& node) {}
    virtual void Visit(ParenthesizedDeclaratorNode& node) {}
    virtual void Visit(AbstractDeclaratorNode& node) {}
    virtual void Visit(DeclSpecifierSequenceNode& node) {}
    virtual void Visit(FriendNode& node) {}
    virtual void Visit(TypedefNode& node) {}
    virtual void Visit(ConstExprNode& node) {}
    virtual void Visit(ConstEvalNode& node) {}
    virtual void Visit(ConstInitNode& node) {}
    virtual void Visit(InlineNode& node) {}
    virtual void Visit(StaticNode& node) {}
    virtual void Visit(ThreadLocalNode& node) {}
    virtual void Visit(ExternNode& node) {}
    virtual void Visit(MutableNode& node) {}
    virtual void Visit(ExplicitNode& node) {}
    virtual void Visit(ConditionalExplicitNode& node) {}
    virtual void Visit(QualifiedPtrNode& node) {}
    virtual void Visit(PtrOperatorNode& node) {}
    virtual void Visit(PtrDeclaratorNode& node) {}
    virtual void Visit(PackDeclaratorIdNode& node) {}
    virtual void Visit(ArrayDeclaratorNode& node) {}
    virtual void Visit(FunctionDeclaratorNode& node) {}
    virtual void Visit(PrefixNode& node) {}
    virtual void Visit(TrailingQualifiersNode& node) {}
    virtual void Visit(TrailingAttributesNode& node) {}
    virtual void Visit(NoexceptSpecifierNode& node) {}
    virtual void Visit(ThrowSpecifierNode& node) {}
    virtual void Visit(ThrowExprNode& node) {}
    // Enum:
    virtual void Visit(EnumSpecifierNode& node) {}
    virtual void Visit(EnumHeadNode& node) {}
    virtual void Visit(EnumBaseNode& node) {}
    virtual void Visit(EnumClassNode& node) {}
    virtual void Visit(EnumStructNode& node) {}
    virtual void Visit(EnumNode& node) {}
    virtual void Visit(EnumeratorDefinitionNode& node) {}
    virtual void Visit(EnumeratorNode& node) {}
    virtual void Visit(ElaboratedEnumSpecifierNode& node) {}
    virtual void Visit(OpaqueEnumDeclarationNode& node) {}
    // Expression:
    virtual void Visit(BinaryExprNode& node) {}
    virtual void Visit(UnaryExprNode& node) {}
    virtual void Visit(ExpressionListNode& node) {}
    virtual void Visit(AssignmentInitNode& node) {}
    virtual void Visit(YieldExprNode& node) {}
    virtual void Visit(ConditionalExprNode& node) {}
    virtual void Visit(AssignNode& node) {}
    virtual void Visit(PlusAssignNode& node) {}
    virtual void Visit(MinusAssignNode& node) {}
    virtual void Visit(MulAssignNode& node) {}
    virtual void Visit(DivAssignNode& node) {}
    virtual void Visit(ModAssignNode& node) {}
    virtual void Visit(XorAssignNode& node) {}
    virtual void Visit(AndAssignNode& node) {}
    virtual void Visit(OrAssignNode& node) {}
    virtual void Visit(ShiftLeftAssignNode& node) {}
    virtual void Visit(ShiftRightAssignNode& node) {}
    virtual void Visit(DisjunctionNode& node) {}
    virtual void Visit(ConjunctionNode& node) {}
    virtual void Visit(InclusiveOrNode& node) {}
    virtual void Visit(ExclusiveOrNode& node) {}
    virtual void Visit(AndNode& node) {}
    virtual void Visit(EqualNode& node) {}
    virtual void Visit(NotEqualNode& node) {}
    virtual void Visit(LessNode& node) {}
    virtual void Visit(GreaterNode& node) {}
    virtual void Visit(LessOrEqualNode& node) {}
    virtual void Visit(GreaterOrEqualNode& node) {}
    virtual void Visit(CompareNode& node) {}
    virtual void Visit(ShiftLeftNode& node) {}
    virtual void Visit(ShiftRightNode& node) {}
    virtual void Visit(PlusNode& node) {}
    virtual void Visit(MinusNode& node) {}
    virtual void Visit(MulNode& node) {}
    virtual void Visit(DivNode& node) {}
    virtual void Visit(ModNode& node) {}
    virtual void Visit(DotStarNode& node) {}
    virtual void Visit(ArrowStarNode& node) {}
    virtual void Visit(CastExprNode& node) {}
    virtual void Visit(DerefNode& node) {}
    virtual void Visit(AddrOfNode& node) {}
    virtual void Visit(NotNode& node) {}
    virtual void Visit(ComplementNode& node) {}
    virtual void Visit(PrefixIncNode& node) {}
    virtual void Visit(PrefixDecNode& node) {}
    virtual void Visit(AwaitExprNode& node) {}
    virtual void Visit(SizeOfTypeExprNode& node) {}
    virtual void Visit(SizeOfPackExprNode& node) {}
    virtual void Visit(SizeOfUnaryExprNode& node) {}
    virtual void Visit(AlignOfExprNode& node) {}
    virtual void Visit(NoexceptExprNode& node) {}
    virtual void Visit(OpNewCall& node) {}
    virtual void Visit(NewExprNode& node) {}
    virtual void Visit(NewPlacementNode& node) {}
    virtual void Visit(ParenNewTypeIdNode& node) {}
    virtual void Visit(NewTypeIdNode& node) {}
    virtual void Visit(ArrayDeletePtrNode& node) {}
    virtual void Visit(DeletePtrNode& node) {}
    virtual void Visit(SubscriptExprNode& node) {}
    virtual void Visit(InvokeExprNode& node) {}
    virtual void Visit(PairNode& node) {}
    virtual void Visit(DotNode& node) {}
    virtual void Visit(ArrowNode& node) {}
    virtual void Visit(MemberExprNode& node) {}
    virtual void Visit(PostfixIncExprNode& node) {}
    virtual void Visit(PostfixDecExprNode& node) {}
    virtual void Visit(TypeIdExprNode& node) {}
    virtual void Visit(DynamicCastNode& node) {}
    virtual void Visit(StaticCastNode& node) {}
    virtual void Visit(ReinterpretCastNode& node) {}
    virtual void Visit(ConstCastNode& node) {}
    virtual void Visit(CppCastExprNode& node) {}
    virtual void Visit(ThisNode& node) {}
    virtual void Visit(ParenthesizedExprNode& node) {}
    virtual void Visit(FoldExprNode& node) {}
    virtual void Visit(NewDeclaratorNode& node) {}
    virtual void Visit(ArrayNewDeclaratorNode& node) {}
    virtual void Visit(NewInitializerNode& node) {}
    virtual void Visit(BracedInitListNode& node) {}
    virtual void Visit(DesignatedInitializerNode& node) {}
    virtual void Visit(DesignatorNode& node) {}
    // Function:
    virtual void Visit(FunctionDefinitionNode& node) {}
    virtual void Visit(FunctionBodyNode& node) {}
    virtual void Visit(DefaultNode& node) {}
    virtual void Visit(DeleteNode& node) {}
    virtual void Visit(DefaultedOrDeletedFunctionNode& node) {}
    virtual void Visit(NoDeclSpecFunctionDeclarationNode& node) {}
    virtual void Visit(OperatorNode& node) {}
    virtual void Visit(NewArrayOpNode& node) {}
    virtual void Visit(NewOpNode& node) {}
    virtual void Visit(DeleteArrayOpNode& node) {}
    virtual void Visit(DeleteOpNode& node) {}
    virtual void Visit(CoAwaitOpNode& node) {}
    virtual void Visit(InvokeOpNode& node) {}
    virtual void Visit(SubscriptOpNode& node) {}
    virtual void Visit(OperatorFunctionIdNode& node) {}
    virtual void Visit(ConversionFunctionIdNode& node) {}
    virtual void Visit(ConversionTypeIdNode& node) {}
    virtual void Visit(ConversionDeclaratorNode& node) {}
    virtual void Visit(DestructorIdNode& node) {}
    virtual void Visit(ParameterNode& node) {}
    virtual void Visit(ParameterListNode& node) {}
    virtual void Visit(NoexceptNode& node) {}
    virtual void Visit(FunctionTryBlockNode& node) {}
    // Identifier:
    virtual void Visit(IdentifierNode& node) {}
    virtual void Visit(UnnamedNode& node) {}
    virtual void Visit(ColonColonNode& node) {}
    virtual void Visit(NestedNameSpecifierNode& node) {}
    virtual void Visit(QualifiedIdNode& node) {}
    virtual void Visit(IdentifierListNode& node) {}
    virtual void Visit(ModuleNameNode& node) {}
    // Lambda:
    virtual void Visit(LambdaExpressionNode& node) {}
    virtual void Visit(LambdaIntroducerNode& node) {}
    virtual void Visit(LambdaCaptureNode& node) {}
    virtual void Visit(DefaultRefCaptureNode& node) {}
    virtual void Visit(DefaultCopyCaptureNode& node) {}
    virtual void Visit(ByRefCaptureNode& node) {}
    virtual void Visit(SimpleCaptureNode& node) {}
    virtual void Visit(CurrentObjectCopyCapture& node) {}
    virtual void Visit(CurrentObjectByRefCapture& node) {}
    virtual void Visit(InitCaptureNode& node) {}
    virtual void Visit(LambdaDeclaratorNode& node) {}
    virtual void Visit(LambdaSpecifiersNode& node) {}
    virtual void Visit(LambdaTemplateParamsNode& node) {}
    // Literal:
    virtual void Visit(IntegerLiteralNode& node) {}
    virtual void Visit(FloatingLiteralNode& node) {}
    virtual void Visit(CharacterLiteralNode& node) {}
    virtual void Visit(StringLiteralNode& node) {}
    virtual void Visit(RawStringLiteralNode& node) {}
    virtual void Visit(BooleanLiteralNode& node) {}
    virtual void Visit(NullPtrLiteralNode& node) {}
    virtual void Visit(UserDefinedLiteraNode& node) {}
    virtual void Visit(LiteralOperatorIdNode& node) {}
    // Module:
    virtual void Visit(ModuleDeclarationNode& node) {}
    virtual void Visit(ExportDeclarationNode& node) {}
    virtual void Visit(ExportNode& node) {}
    virtual void Visit(ImportNode& node) {}
    virtual void Visit(ImportDeclarationNode& node) {}
    virtual void Visit(ModulePartitionNode& node) {}
    virtual void Visit(ModuleNode& node) {}
    virtual void Visit(GlobalModuleFragmentNode& node) {}
    virtual void Visit(PrivateModuleFragmentNode& node) {}
    virtual void Visit(AngleHeaderName& node) {}
    virtual void Visit(QuoteHeaderName& node) {}
    // Qualifier:
    virtual void Visit(ConstNode& node) {}
    virtual void Visit(VolatileNode& node) {}
    virtual void Visit(LvalueRefNode& node) {}
    virtual void Visit(RvalueRefNode& node) {}
    virtual void Visit(PtrNode& node) {}
    virtual void Visit(CVQualifierSequenceNode& node) {}
    // SimpleType:
    virtual void Visit(CharNode& node) {}
    virtual void Visit(Char8Node& node) {}
    virtual void Visit(Char16Node& node) {}
    virtual void Visit(Char32Node& node) {}
    virtual void Visit(WCharNode& node) {}
    virtual void Visit(BoolNode& node) {}
    virtual void Visit(ShortNode& node) {}
    virtual void Visit(IntNode& node) {}
    virtual void Visit(LongNode& node) {}
    virtual void Visit(SignedNode& node) {}
    virtual void Visit(UnsignedNode& node) {}
    virtual void Visit(FloatNode& node) {}
    virtual void Visit(DoubleNode& node) {}
    virtual void Visit(VoidNode& node) {}
    // Statement:
    virtual void Visit(LabeledStatementNode& node) {}
    virtual void Visit(CaseStatementNode& node) {}
    virtual void Visit(DefaultStatementNode& node) {}
    virtual void Visit(CompoundStatementNode& node) {}
    virtual void Visit(SequenceStatementNode& node) {}
    virtual void Visit(IfStatementNode& node) {}
    virtual void Visit(SwitchStatementNode& node) {}
    virtual void Visit(WhileStatementNode& node) {}
    virtual void Visit(DoStatementNode& node) {}
    virtual void Visit(RangeForStatementNode& node) {}
    virtual void Visit(ForRangeDeclarationNode& node) {}
    virtual void Visit(StructuredBindingNode& node) {}
    virtual void Visit(ForStatementNode& node) {}
    virtual void Visit(BreakStatementNode& node) {}
    virtual void Visit(ContinueStatementNode& node) {}
    virtual void Visit(ReturnStatementNode& node) {}
    virtual void Visit(CoReturnStatementNode& node) {}
    virtual void Visit(GotoStatementNode& node) {}
    virtual void Visit(TryStatementNode& node) {}
    virtual void Visit(HandlerSequenceNode& node) {}
    virtual void Visit(HandlerNode& node) {}
    virtual void Visit(ExceptionDeclarationNode& node) {}
    virtual void Visit(ExpressionStatementNode& node) {}
    virtual void Visit(DeclarationStatementNode& node) {}
    virtual void Visit(InitConditionNode& node) {}
    virtual void Visit(BoundStatementNode& node) {}
    // Template:
    virtual void Visit(TemplateDeclarationNode& node) {}
    virtual void Visit(TemplateHeadNode& node) {}
    virtual void Visit(TemplateParameterListNode& node) {}
    virtual void Visit(TypeParameterNode& node) {}
    virtual void Visit(TemplateIdNode& node) {}
    virtual void Visit(TypenameNode& node) {}
    virtual void Visit(DeductionGuideNode& node) {}
    virtual void Visit(ExplicitInstantiationNode& node) {}
    virtual void Visit(TemplateNode& node) {}
    virtual void Visit(ExplicitSpecializationNode& node) {}
    // TranslationUnit:
    virtual void Visit(TranslationUnitNode& node) {}
    virtual void Visit(ModuleUnitNode& node) {}
    // Type:
    virtual void Visit(TypeSpecifierSequenceNode& node) {}
    virtual void Visit(TypenameSpecifierNode& node) {}
    virtual void Visit(TypeIdNode& node) {}
    virtual void Visit(DefiningTypeIdNode& node) {}
    virtual void Visit(DefiningTypeSpecifierSequenceNode& node) {}
    virtual void Visit(TrailingReturnTypeNode& node) {}
    virtual void Visit(ElaboratedTypeSpecifierNode& node) {}
    virtual void Visit(DeclTypeSpecifierNode& node) {}
    virtual void Visit(PlaceholderTypeSpecifierNode& node) {}
    // Punctuation:
    virtual void Visit(SemicolonNode& node) {}
    virtual void Visit(CommaNode& node) {}
    virtual void Visit(QuestNode& node) {}
    virtual void Visit(ColonNode& node) {}
    virtual void Visit(EllipsisNode& node) {}
    virtual void Visit(LParenNode& node) {}
    virtual void Visit(RParenNode& node) {}
    virtual void Visit(LBracketNode& node) {}
    virtual void Visit(RBracketNode& node) {}
    virtual void Visit(LBraceNode& node) {}
    virtual void Visit(RBraceNode& node) {}
};

class DefaultVisitor : public Visitor
{
public:
    // Attribute:
    void Visit(AttributeSpecifierSequenceNode& node) override;
    void Visit(AttributeSpecifierNode& node) override;
    void Visit(AttributeUsingPrefixNode& node) override;
    void Visit(AttributeNode& node) override;
    void Visit(AttributeScopedTokenNode& node) override;
    void Visit(AttributeArgumentsNode& node) override;
    void Visit(BalancedTokenSequenceNode& node) override;
    void Visit(TokenNode& node) override;
    void Visit(AlignmentSpecifierNode& node) override;
    void Visit(LParenNode& node) override;
    void Visit(RParenNode& node) override;
    void Visit(LBracketNode& node) override;
    void Visit(RBracketNode& node) override;
    void Visit(LBraceNode& node) override;
    void Visit(RBraceNode& node) override;
    //  Class:
    void Visit(ClassSpecifierNode& node) override;
    void Visit(ClassHeadNode& node) override;
    void Visit(BaseClauseNode& node) override;
    void Visit(BaseSpecifierListNode& node) override;
    void Visit(BaseSpecifierNode& node) override;
    void Visit(BeginAccessGroupNode& node) override;
    void Visit(MemberDeclarationNode& node) override;
    void Visit(MemberDeclaratorListNode& node) override;
    void Visit(ConstructorNode& node) override;
    void Visit(ConstructorInitializerNode& node) override;
    void Visit(MemberInitializerListNode& node) override;
    void Visit(MemberInitializerNode& node) override;
    void Visit(VirtSpecifierSequenceNode& node) override;
    void Visit(ClassNode& node) override;
    void Visit(StructNode& node) override;
    void Visit(UnionNode& node) override;
    void Visit(PublicNode& node) override;
    void Visit(ProtectedNode& node) override;
    void Visit(PrivateNode& node) override;
    void Visit(VirtualNode& node) override;
    void Visit(OverrideNode& node) override;
    void Visit(FinalNode& node) override;
    void Visit(PureSpecifierNode& node) override;
    // Concept:
    void Visit(ConceptDefinitionNode& node) override;
    void Visit(RequiresExprNode& node) override;
    void Visit(RequirementBodyNode& node) override;
    void Visit(SimpleRequirementNode& node) override;
    void Visit(TypeRequirementNode& node) override;
    void Visit(CompoundRequirementNode& node) override;
    void Visit(ReturnTypeRequirementNode& node) override;
    void Visit(NestedRequirementNode& node) override;
    void Visit(TypeConstraintNode& node) override;
    void Visit(RequiresClauseNode& node) override;
    // Declaration:
    void Visit(DeclarationSequenceNode& node) override;
    void Visit(SimpleDeclarationNode& node) override;
    void Visit(AsmDeclarationNode& node) override;
    void Visit(AsmNode& node) override;
    void Visit(LinkageSpecificationNode& node) override;
    void Visit(NamespaceDefinitionNode& node) override;
    void Visit(NamespaceBodyNode& node) override;
    void Visit(NamespaceAliasDefinitionNode& node) override;
    void Visit(UsingDeclarationNode& node) override;
    void Visit(UsingNode& node) override;
    void Visit(UsingDeclaratorListNode& node) override;
    void Visit(UsingEnumDeclarationNode& node) override;
    void Visit(UsingDirectiveNode& node) override;
    void Visit(NamespaceNode& node) override;
    void Visit(StaticAssertDeclarationNode&node) override;
    void Visit(StaticAssertNode& node) override;
    void Visit(AliasDeclarationNode& node) override;
    void Visit(EmptyDeclarationNode& node) override;
    void Visit(AttributeDeclarationNode& node) override;
    void Visit(InitDeclaratorListNode& node) override;
    void Visit(InitDeclaratorNode& node) override;
    void Visit(TrailingFunctionDeclaratorNode& node) override;
    void Visit(ParenthesizedDeclaratorNode& node) override;
    void Visit(AbstractDeclaratorNode& node) override;
    void Visit(DeclSpecifierSequenceNode& node) override;
    void Visit(FriendNode& node) override;
    void Visit(TypedefNode& node) override;
    void Visit(ConstExprNode& node) override;
    void Visit(ConstEvalNode& node) override;
    void Visit(ConstInitNode& node) override;
    void Visit(InlineNode& node) override;
    void Visit(StaticNode& node) override;
    void Visit(ThreadLocalNode& node) override;
    void Visit(ExternNode& node) override;
    void Visit(MutableNode& node) override;
    void Visit(ExplicitNode& node) override;
    void Visit(ConditionalExplicitNode& node) override;
    void Visit(QualifiedPtrNode& node) override;
    void Visit(PtrOperatorNode& node) override;
    void Visit(PtrDeclaratorNode& node) override;
    void Visit(PackDeclaratorIdNode& node) override;
    void Visit(ArrayDeclaratorNode& node) override;
    void Visit(FunctionDeclaratorNode& node) override;
    void Visit(PrefixNode& node) override;
    void Visit(TrailingQualifiersNode& node) override;
    void Visit(TrailingAttributesNode& node) override;
    void Visit(NoexceptSpecifierNode& node) override;
    void Visit(ThrowSpecifierNode& node) override;
    void Visit(ThrowExprNode& node) override;
    // Enum:
    void Visit(EnumSpecifierNode& node) override;
    void Visit(EnumHeadNode& node) override;
    void Visit(EnumBaseNode& node) override;
    void Visit(EnumClassNode& node) override;
    void Visit(EnumStructNode& node) override;
    void Visit(EnumNode& node) override;
    void Visit(EnumeratorDefinitionNode& node) override;
    void Visit(EnumeratorNode& node) override;
    void Visit(ElaboratedEnumSpecifierNode& node) override;
    void Visit(OpaqueEnumDeclarationNode& node) override;
    // Expression:
    void Visit(BinaryExprNode& node) override;
    void Visit(UnaryExprNode& node) override;
    void Visit(ExpressionListNode& node) override;
    void Visit(AssignmentInitNode& node) override;
    void Visit(YieldExprNode& node) override;
    void Visit(ConditionalExprNode& node) override;
    void Visit(CommaNode& node) override;
    void Visit(AssignNode& node) override;
    void Visit(PlusAssignNode& node) override;
    void Visit(MinusAssignNode& node) override;
    void Visit(MulAssignNode& node) override;
    void Visit(DivAssignNode& node) override;
    void Visit(ModAssignNode& node) override;
    void Visit(XorAssignNode& node) override;
    void Visit(AndAssignNode& node) override;
    void Visit(OrAssignNode& node) override;
    void Visit(ShiftLeftAssignNode& node) override;
    void Visit(ShiftRightAssignNode& node) override;
    void Visit(QuestNode& node) override;
    void Visit(ColonNode& node) override;
    void Visit(DisjunctionNode& node) override;
    void Visit(ConjunctionNode& node) override;
    void Visit(InclusiveOrNode& node) override;
    void Visit(ExclusiveOrNode& node) override;
    void Visit(AndNode& node) override;
    void Visit(EqualNode& node) override;
    void Visit(NotEqualNode& node) override;
    void Visit(LessNode& node) override;
    void Visit(GreaterNode& node) override;
    void Visit(LessOrEqualNode& node) override;
    void Visit(GreaterOrEqualNode& node) override;
    void Visit(CompareNode& node) override;
    void Visit(ShiftLeftNode& node) override;
    void Visit(ShiftRightNode& node) override;
    void Visit(PlusNode& node) override;
    void Visit(MinusNode& node) override;
    void Visit(MulNode& node) override;
    void Visit(DivNode& node) override;
    void Visit(ModNode& node) override;
    void Visit(DotStarNode& node) override;
    void Visit(ArrowStarNode& node) override;
    void Visit(CastExprNode& node) override;
    void Visit(DerefNode& node) override;
    void Visit(AddrOfNode& node) override;
    void Visit(NotNode& node) override;
    void Visit(ComplementNode& node) override;
    void Visit(PrefixIncNode& node) override;
    void Visit(PrefixDecNode& node) override;
    void Visit(AwaitExprNode& node) override;
    void Visit(SizeOfTypeExprNode& node) override;
    void Visit(SizeOfPackExprNode& node) override;
    void Visit(SizeOfUnaryExprNode& node) override;
    void Visit(AlignOfExprNode& node) override;
    void Visit(NoexceptExprNode& node) override;
    void Visit(NewExprNode& node) override;
    void Visit(NewPlacementNode& node) override;
    void Visit(ParenNewTypeIdNode& node) override;
    void Visit(NewTypeIdNode& node) override;
    void Visit(ArrayDeletePtrNode& node) override;
    void Visit(DeletePtrNode& node) override;
    void Visit(SubscriptExprNode& node) override;
    void Visit(InvokeExprNode& node) override;
    void Visit(PairNode& node) override;
    void Visit(DotNode& node) override;
    void Visit(ArrowNode& node) override;
    void Visit(MemberExprNode& node) override;
    void Visit(PostfixIncExprNode& node) override;
    void Visit(PostfixDecExprNode& node) override;
    void Visit(TypeIdExprNode& node) override;
    void Visit(DynamicCastNode& node) override;
    void Visit(StaticCastNode& node) override;
    void Visit(ReinterpretCastNode& node) override;
    void Visit(ConstCastNode& node) override;
    void Visit(CppCastExprNode& node) override;
    void Visit(ThisNode& node) override;
    void Visit(ParenthesizedExprNode& node) override;
    void Visit(FoldExprNode& node) override;
    void Visit(NewDeclaratorNode& node) override;
    void Visit(ArrayNewDeclaratorNode& node) override;
    void Visit(NewInitializerNode& node) override;
    void Visit(BracedInitListNode& node) override;
    void Visit(DesignatedInitializerNode& node) override;
    void Visit(DesignatorNode& node) override;
    void Visit(EllipsisNode& node) override;
    // Function:
    void Visit(FunctionDefinitionNode& node) override;
    void Visit(FunctionBodyNode& node) override;
    void Visit(DefaultNode& node) override;
    void Visit(DeleteNode& node) override;
    void Visit(DefaultedOrDeletedFunctionNode& node) override;
    void Visit(NoDeclSpecFunctionDeclarationNode& node) override;
    void Visit(OperatorNode& node) override;
    void Visit(NewArrayOpNode& node) override;
    void Visit(NewOpNode& node) override;
    void Visit(DeleteArrayOpNode& node) override;
    void Visit(DeleteOpNode& node) override;
    void Visit(CoAwaitOpNode& node) override;
    void Visit(InvokeOpNode& node) override;
    void Visit(SubscriptOpNode& node) override;
    void Visit(OperatorFunctionIdNode& node) override;
    void Visit(ConversionFunctionIdNode& node) override;
    void Visit(ConversionTypeIdNode& node) override;
    void Visit(ConversionDeclaratorNode& node) override;
    void Visit(DestructorIdNode& node) override;
    void Visit(ParameterNode& node) override;
    void Visit(ParameterListNode& node) override;
    void Visit(NoexceptNode& node) override;
    void Visit(FunctionTryBlockNode& node) override;
    // Identifier:
    void Visit(IdentifierNode& node) override;
    void Visit(UnnamedNode& node) override;
    void Visit(ColonColonNode& node) override;
    void Visit(NestedNameSpecifierNode& node) override;
    void Visit(QualifiedIdNode& node) override;
    void Visit(IdentifierListNode& node) override;
    void Visit(ModuleNameNode& node) override;
    // Lambda:
    void Visit(LambdaExpressionNode& node) override;
    void Visit(LambdaIntroducerNode& node) override;
    void Visit(LambdaCaptureNode& node) override;
    void Visit(DefaultRefCaptureNode& node) override;
    void Visit(DefaultCopyCaptureNode& node) override;
    void Visit(ByRefCaptureNode& node) override;
    void Visit(SimpleCaptureNode& node) override;
    void Visit(CurrentObjectCopyCapture& node) override;
    void Visit(CurrentObjectByRefCapture& node) override;
    void Visit(InitCaptureNode& node) override;
    void Visit(LambdaDeclaratorNode& node) override;
    void Visit(LambdaSpecifiersNode& node) override;
    void Visit(LambdaTemplateParamsNode& node) override;
    // Literal:
    void Visit(IntegerLiteralNode& node) override;
    void Visit(FloatingLiteralNode& node) override;
    void Visit(CharacterLiteralNode& node) override;
    void Visit(StringLiteralNode& node) override;
    void Visit(RawStringLiteralNode& node) override;
    void Visit(BooleanLiteralNode& node) override;
    void Visit(NullPtrLiteralNode& node) override;
    void Visit(UserDefinedLiteraNode& node) override;
    void Visit(LiteralOperatorIdNode& node) override;
    // Module:
    void Visit(ModuleDeclarationNode& node) override;
    void Visit(ExportDeclarationNode& node) override;
    void Visit(ExportNode& node) override;
    void Visit(ImportNode& node) override;
    void Visit(ImportDeclarationNode& node) override;
    void Visit(ModulePartitionNode& node) override;
    void Visit(ModuleNode& node) override;
    void Visit(GlobalModuleFragmentNode& node) override;
    void Visit(PrivateModuleFragmentNode& node) override;
    void Visit(AngleHeaderName& node) override;
    void Visit(QuoteHeaderName& node) override;
    // Qualifier:
    void Visit(ConstNode& node) override;
    void Visit(VolatileNode& node) override;
    void Visit(LvalueRefNode& node) override;
    void Visit(RvalueRefNode& node) override;
    void Visit(PtrNode& node) override;
    void Visit(CVQualifierSequenceNode& node) override;
    // SimpleType:
    void Visit(CharNode& node) override;
    void Visit(Char8Node& node) override;
    void Visit(Char16Node& node) override;
    void Visit(Char32Node& node) override;
    void Visit(WCharNode& node) override;
    void Visit(BoolNode& node) override;
    void Visit(ShortNode& node) override;
    void Visit(IntNode& node) override;
    void Visit(LongNode& node) override;
    void Visit(SignedNode& node) override;
    void Visit(UnsignedNode& node) override;
    void Visit(FloatNode& node) override;
    void Visit(DoubleNode& node) override;
    void Visit(VoidNode& node) override;
    // Statement:
    void Visit(LabeledStatementNode& node) override;
    void Visit(CaseStatementNode& node) override;
    void Visit(DefaultStatementNode& node) override;
    void Visit(CompoundStatementNode& node) override;
    void Visit(SequenceStatementNode& node) override;
    void Visit(IfStatementNode& node) override;
    void Visit(SwitchStatementNode& node) override;
    void Visit(WhileStatementNode& node) override;
    void Visit(DoStatementNode& node) override;
    void Visit(RangeForStatementNode& node) override;
    void Visit(ForRangeDeclarationNode& node) override;
    void Visit(StructuredBindingNode& node) override;
    void Visit(ForStatementNode& node) override;
    void Visit(BreakStatementNode& node) override;
    void Visit(ContinueStatementNode& node) override;
    void Visit(ReturnStatementNode& node) override;
    void Visit(CoReturnStatementNode& node) override;
    void Visit(GotoStatementNode& node) override;
    void Visit(TryStatementNode& node) override;
    void Visit(HandlerSequenceNode& node) override;
    void Visit(HandlerNode& node) override;
    void Visit(ExceptionDeclarationNode& node) override;
    void Visit(ExpressionStatementNode& node) override;
    void Visit(DeclarationStatementNode& node) override;
    void Visit(InitConditionNode& node) override;
    void Visit(SemicolonNode& node) override;
    // Template:
    void Visit(TemplateDeclarationNode& node) override;
    void Visit(TemplateHeadNode& node) override;
    void Visit(TemplateParameterListNode& node) override;
    void Visit(TypeParameterNode& node) override;
    void Visit(TemplateIdNode& node) override;
    void Visit(TypenameNode& node) override;
    void Visit(DeductionGuideNode& node) override;
    void Visit(ExplicitInstantiationNode& node) override;
    void Visit(TemplateNode& node) override;
    void Visit(ExplicitSpecializationNode& node) override;
    // TranslationUnit:
    void Visit(TranslationUnitNode& node) override;
    void Visit(ModuleUnitNode& node) override;
    // Type:
    void Visit(TypeSpecifierSequenceNode& node) override;
    void Visit(TypenameSpecifierNode& node) override;
    void Visit(TypeIdNode& node) override;
    void Visit(DefiningTypeIdNode& node) override;
    void Visit(DefiningTypeSpecifierSequenceNode& node) override;
    void Visit(TrailingReturnTypeNode& node) override;
    void Visit(ElaboratedTypeSpecifierNode& node) override;
    void Visit(DeclTypeSpecifierNode& node) override;
    void Visit(PlaceholderTypeSpecifierNode& node) override;
};

} // namespace otava::ast
