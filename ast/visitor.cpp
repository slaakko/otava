// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.visitor;

import otava.ast.attribute;
import otava.ast.classes;
import otava.ast.concepts;
import otava.ast.declaration;
import otava.ast.enums;
import otava.ast.expression;
import otava.ast.function;
import otava.ast.identifier;
import otava.ast.lambda;
import otava.ast.literal;
import otava.ast.modules;
import otava.ast.node;
import otava.ast.node.list;
import otava.ast.punctuation;
import otava.ast.reader;
import otava.ast.qualifier;
import otava.ast.simple.type;
import otava.ast.statement;
import otava.ast.templates;
import otava.ast.translation.unit;
import otava.ast.type;
import otava.ast.util;
import otava.ast.writer;

namespace otava::ast {

Visitor::~Visitor()
{
}

void Visitor::VisitSingleNode(Node& node)
{
    BeginVisit(node);
    EndVisit(node);
}

void Visitor::VisitUnaryNode(UnaryNode& unaryNode)
{
    BeginVisit(unaryNode);
    unaryNode.Child()->Accept(*this);
    EndVisit(unaryNode);
}

void Visitor::VisitBinaryNode(BinaryNode& binaryNode)
{
    BeginVisit(binaryNode);
    binaryNode.Left()->Accept(*this);
    binaryNode.Right()->Accept(*this);
    EndVisit(binaryNode);
}

void Visitor::VisitSequence(SequenceNode& sequenceNode)
{
    BeginVisit(sequenceNode);
    VisitSequenceContent(sequenceNode);
    EndVisit(sequenceNode);
}

void Visitor::VisitSequenceContent(SequenceNode& sequenceNode)
{
    int n = sequenceNode.Nodes().Count();
    for (int i = 0; i < n; ++i)
    {
        Node* node = sequenceNode.Nodes()[i];
        node->Accept(*this);
    }
}

void Visitor::VisitList(ListNode& listNode)
{
    BeginVisit(listNode);
    VisitListContent(listNode);
    EndVisit(listNode);
}

void Visitor::VisitListContent(ListNode& listNode)
{
    int n = listNode.Nodes().Count();
    for (int i = 0; i < n; ++i)
    {
        Node* node = listNode.Nodes()[i];
        node->Accept(*this);
    }
}

// DefaultVisitor:

// Attribute:

void DefaultVisitor::Visit(AttributeSpecifierSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(AttributeSpecifierNode& node)
{
    BeginVisit(node);
    VisitOperator("[", node.LBracketSpan1());
    VisitOperator("[", node.LBracketSpan2());
    if (node.UsingPrefix())
    {
        node.UsingPrefix()->Accept(*this);
    }
    VisitListContent(node);
    VisitOperator("]", node.RBracketSpan1());
    VisitOperator("]", node.RBracketSpan2());
    EndVisit(node);
}

void DefaultVisitor::Visit(AttributeUsingPrefixNode& node)
{
    BeginVisit(node);
    VisitKeyword("using", node.GetSpan());
    node.Child()->Accept(*this);
    VisitOperator(":", node.ColonSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AttributeNode& node)
{
    BeginVisit(node);
    node.AttributeToken()->Accept(*this);
    if (node.AttributeArgs())
    {
        node.AttributeArgs()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(AttributeScopedTokenNode& node)
{
    BeginVisit(node);
    node.Ns()->Accept(*this);
    node.ColonColon()->Accept(*this);
    node.Identifier()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(AttributeArgumentsNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    if (node.BalancedTokenSequence())
    {
        node.BalancedTokenSequence()->Accept(*this);
    }
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(BalancedTokenSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(TokenNode& node)
{
    BeginVisit(node);
    VisitToken(node.Str(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AlignmentSpecifierNode& node)
{
    BeginVisit(node);
    VisitKeyword("alignas", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Alignment()->Accept(*this);
    if (node.Ellipsis())
    {
        node.Ellipsis()->Accept(*this);
    }
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LParenNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(RParenNode& node)
{
    BeginVisit(node);
    VisitOperator(")", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LBracketNode& node)
{
    BeginVisit(node);
    VisitOperator("[", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(RBracketNode& node)
{
    BeginVisit(node);
    VisitOperator("]", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LBraceNode& node)
{
    BeginVisit(node);
    VisitOperator("{", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(RBraceNode& node)
{
    BeginVisit(node);
    VisitOperator("}", node.GetSpan());
    EndVisit(node);
}

//  Class:

void DefaultVisitor::Visit(ClassSpecifierNode& node)
{
    BeginVisit(node);
    node.ClassHead()->Accept(*this);
    VisitOperator("{", node.LBraceSpan());
    VisitSequenceContent(node);
    VisitOperator("}", node.RBraceSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ClassHeadNode& node)
{
    BeginVisit(node);
    node.ClassKey()->Accept(*this);
    if (node.Attibutes())
    {
        node.Attibutes()->Accept(*this);
    }
    node.ClassHeadName()->Accept(*this);
    if (node.ClassVirtSpecifier())
    {
        node.ClassVirtSpecifier()->Accept(*this);
    }
    if (node.BaseClause())
    {
        node.BaseClause()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(BaseClauseNode& node)
{
    BeginVisit(node);
    VisitOperator(":", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(BaseSpecifierListNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(BaseSpecifierNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    if (node.VirtualFirst())
    {
        if (node.VirtualSpecifier())
        {
            node.VirtualSpecifier()->Accept(*this);
        }
        if (node.AccessSpecifier())
        {
            node.AccessSpecifier()->Accept(*this);
        }
    }
    else
    {
        if (node.AccessSpecifier())
        {
            node.AccessSpecifier()->Accept(*this);
        }
        if (node.VirtualSpecifier())
        {
            node.VirtualSpecifier()->Accept(*this);
        }
    }
    node.ClassOrDeclType()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(BeginAccessGroupNode& node)
{
    BeginVisit(node);
    node.Child()->Accept(*this);
    VisitOperator(":", node.ColonSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(MemberDeclarationNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    if (node.DeclSpecifiers())
    {
        node.DeclSpecifiers()->Accept(*this);
    }
    if (node.MemberDeclarators())
    {
        node.MemberDeclarators()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(MemberDeclaratorListNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(ConstructorNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(ConstructorInitializerNode& node)
{
    BeginVisit(node);
    VisitOperator(":", node.GetSpan());
    node.GetMemberInitializerListNode()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(MemberInitializerListNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(MemberInitializerNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(VirtSpecifierSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(ClassNode& node)
{
    BeginVisit(node);
    VisitKeyword("class", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(StructNode& node)
{
    BeginVisit(node);
    VisitKeyword("struct", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(UnionNode& node)
{
    BeginVisit(node);
    VisitKeyword("union", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PublicNode& node)
{
    BeginVisit(node);
    VisitKeyword("public", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ProtectedNode& node)
{
    BeginVisit(node);
    VisitKeyword("protected", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PrivateNode& node)
{
    BeginVisit(node);
    VisitKeyword("private", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(VirtualNode& node)
{
    BeginVisit(node);
    VisitKeyword("virtual", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(OverrideNode& node)
{
    BeginVisit(node);
    VisitKeyword("override", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(FinalNode& node)
{
    BeginVisit(node);
    VisitKeyword("final", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PureSpecifierNode& node)
{
    BeginVisit(node);
    VisitOperator("=", node.GetSpan());
    VisitOperator("0", node.ZeroSpan());
    EndVisit(node);
}

// Concept:

void DefaultVisitor::Visit(ConceptDefinitionNode& node)
{
    BeginVisit(node);
    VisitKeyword("concept", node.GetSpan());
    node.ConceptName()->Accept(*this);
    node.Assign()->Accept(*this);
    node.ConstraintExpression()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(RequiresExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("requires", node.GetSpan());
    if (node.Params())
    {
        node.Params()->Accept(*this);
    }
    node.Body()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(RequirementBodyNode& node)
{
    BeginVisit(node);
    VisitOperator("{", node.LBraceSpan());
    VisitSequenceContent(node);
    VisitOperator("}", node.RBraceSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(SimpleRequirementNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(TypeRequirementNode& node)
{
    BeginVisit(node);
    VisitKeyword("typename", node.GetSpan());
    if (node.NestedNameSpecifier())
    {
        node.NestedNameSpecifier()->Accept(*this);
    }
    node.TypeName()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(CompoundRequirementNode& node)
{
    BeginVisit(node);
    VisitOperator("{", node.LBraceSpan());
    node.Expr()->Accept(*this);
    VisitOperator("}", node.RBraceSpan());
    if (node.NoExcept())
    {
        node.NoExcept()->Accept(*this);
    }
    if (node.ReturnTypeRequirement())
    {
        node.ReturnTypeRequirement()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ReturnTypeRequirementNode& node)
{
    BeginVisit(node);
    VisitOperator("->", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(NestedRequirementNode& node)
{
    BeginVisit(node);
    VisitKeyword("requires", node.GetSpan());
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(TypeConstraintNode& node)
{
    BeginVisit(node);
    node.ConceptName()->Accept(*this);
    if (node.HasTemplateArgumentList())
    {
        VisitOperator("<", node.LAngleSpan());
        VisitListContent(node);
        VisitOperator(">", node.RAngleSpan());
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(RequiresClauseNode& node)
{
    BeginVisit(node);
    VisitKeyword("requires", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

// Declaration:

void DefaultVisitor::Visit(DeclarationSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(SimpleDeclarationNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.DeclarationSpecifiers()->Accept(*this);
    if (node.InitDeclaratorList())
    {
        node.InitDeclaratorList()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(AsmDeclarationNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Asm()->Accept(*this);
    VisitOperator("(", node.LParenSpan());
    node.AsmText()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AsmNode& node)
{
    BeginVisit(node);
    VisitKeyword("asm", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LinkageSpecificationNode& node)
{
    BeginVisit(node);
    node.Extern()->Accept(*this);
    node.Linkage()->Accept(*this);
    if (node.LBraceSpan().IsValid())
    {
        VisitOperator("{", node.LBraceSpan());
        if (node.Declarations())
        {
            node.Declarations()->Accept(*this);
        }
        VisitOperator("}", node.RBraceSpan());
    }
    else
    {
        node.Declarations()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(NamespaceDefinitionNode& node)
{
    BeginVisit(node);
    if (node.Inline())
    {
        node.Inline()->Accept(*this);
    }
    node.Namespace()->Accept(*this);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    if (node.NamespaceName())
    {
        node.NamespaceName()->Accept(*this);
    }
    VisitOperator("{", node.LBraceSpan());
    node.NamespaceBody()->Accept(*this);
    VisitOperator("}", node.RBraceSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NamespaceBodyNode& node)
{
    if (node.Declarations())
    {
        BeginVisit(node);
        node.Declarations()->Accept(*this);
        EndVisit(node);
    }
}

void DefaultVisitor::Visit(NamespaceAliasDefinitionNode& node)
{
    BeginVisit(node);
    VisitKeyword("namespace", node.GetSpan());
    node.Id()->Accept(*this);
    node.Assign()->Accept(*this);
    node.QualifiedNamespaceSpecifier()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(UsingDeclarationNode& node)
{
    BeginVisit(node);
    node.Using()->Accept(*this);
    node.Declarators()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(UsingNode& node)
{
    BeginVisit(node);
    VisitKeyword("using", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(UsingDeclaratorListNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(UsingEnumDeclarationNode& node)
{
    BeginVisit(node);
    node.Using()->Accept(*this);
    node.ElaboratedEnumSpecifier()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(UsingDirectiveNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Using()->Accept(*this);
    node.Namespace()->Accept(*this);
    node.GetId()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(NamespaceNode& node)
{
    BeginVisit(node);
    VisitKeyword("namespace", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(StaticAssertDeclarationNode& node)
{
    BeginVisit(node);
    node.StaticAssert()->Accept(*this);
    VisitOperator("(", node.LParenSpan());
    node.ConstantExpr()->Accept(*this);
    if (node.Comma())
    {
        node.Comma()->Accept(*this);
        node.StringLiteral()->Accept(*this);
    }
    VisitOperator(")", node.RParenSpan());
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(StaticAssertNode& node)
{
    BeginVisit(node);
    VisitKeyword("static_assert", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AliasDeclarationNode& node)
{
    BeginVisit(node);
    node.Using()->Accept(*this);
    node.Identifier()->Accept(*this);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Assign()->Accept(*this);
    node.DefiningTypeId()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(EmptyDeclarationNode& node)
{
    BeginVisit(node);
    VisitOperator(";", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AttributeDeclarationNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(InitDeclaratorListNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(InitDeclaratorNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(TrailingFunctionDeclaratorNode& node)
{
    BeginVisit(node);
    node.Declarator()->Accept(*this);
    node.Parameters()->Accept(*this);
    node.TrailingReturnType()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ParenthesizedDeclaratorNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    node.Declarator()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AbstractDeclaratorNode& node)
{
    VisitSingleNode(node);
}

void DefaultVisitor::Visit(DeclSpecifierSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(FriendNode& node)
{
    BeginVisit(node);
    VisitKeyword("friend", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(TypedefNode& node)
{
    BeginVisit(node);
    VisitKeyword("typedef", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ConstExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("constexpr", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ConstEvalNode& node)
{
    BeginVisit(node);
    VisitKeyword("consteval", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ConstInitNode& node)
{
    BeginVisit(node);
    VisitKeyword("constinit", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(InlineNode& node)
{
    BeginVisit(node);
    VisitKeyword("inline", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(StaticNode& node)
{
    BeginVisit(node);
    VisitKeyword("static", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ThreadLocalNode& node)
{
    BeginVisit(node);
    VisitKeyword("thread_local", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ExternNode& node)
{
    BeginVisit(node);
    VisitKeyword("extern", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(MutableNode& node)
{
    BeginVisit(node);
    VisitKeyword("mutable", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ExplicitNode& node)
{
    BeginVisit(node);
    VisitKeyword("explicit", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ConditionalExplicitNode& node)
{
    BeginVisit(node);
    VisitKeyword("explicit", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Condition()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
}

void DefaultVisitor::Visit(QualifiedPtrNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(PtrOperatorNode& node)
{
    VisitUnaryNode(node);
}

void DefaultVisitor::Visit(PtrDeclaratorNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(PackDeclaratorIdNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(ArrayDeclaratorNode& node)
{
    BeginVisit(node);
    node.Child()->Accept(*this);
    VisitOperator("[", node.LBracketSpan());
    if (node.Dimension())
    {
        node.Dimension()->Accept(*this);
    }
    VisitOperator("]", node.RBracketSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(FunctionDeclaratorNode& node)
{
    VisitUnaryNode(node);
    node.Params()->Accept(*this);
}

void DefaultVisitor::Visit(PrefixNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(TrailingQualifiersNode& node)
{
    node.Subject()->Accept(*this);
    VisitSequence(node);
}

void DefaultVisitor::Visit(TrailingAttributesNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(NoexceptSpecifierNode& node)
{
    BeginVisit(node);
    VisitKeyword("noexcept", node.GetSpan());
    if (node.ConstantExpr())
    {
        VisitOperator("(", node.LParenSpan());
        node.ConstantExpr()->Accept(*this);
        VisitOperator("}", node.RParenSpan());
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ThrowSpecifierNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    VisitOperator("}", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ThrowExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("throw", node.GetSpan());
    VisitUnaryNode(node);
    EndVisit(node);
}

// Enum:

void DefaultVisitor::Visit(EnumSpecifierNode& node)
{
    BeginVisit(node);
    node.EnumHead()->Accept(*this);
    VisitOperator("{", node.LBraceSpan());
    VisitListContent(node);
    VisitOperator("}", node.RBraceSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(EnumHeadNode& node)
{
    BeginVisit(node);
    node.EnumKey()->Accept(*this);
    node.EnumHeadName()->Accept(*this);
    if (node.EnumBase())
    {
        node.EnumBase()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(EnumBaseNode& node)
{
    BeginVisit(node);
    VisitOperator(":", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(EnumClassNode& node)
{
    BeginVisit(node);
    VisitKeyword("enum", node.GetSpan());
    VisitKeyword("class", node.ClassSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(EnumStructNode& node)
{
    BeginVisit(node);
    VisitKeyword("enum", node.GetSpan());
    VisitKeyword("struct", node.StructSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(EnumNode& node)
{
    BeginVisit(node);
    VisitKeyword("enum", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(EnumeratorDefinitionNode& node)
{
    BeginVisit(node);
    node.Enumerator()->Accept(*this);
    if (node.GetValue())
    {
        VisitOperator("=", node.AssignSpan());
        node.GetValue()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(EnumeratorNode& node)
{
    BeginVisit(node);
    node.Identifier()->Accept(*this);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ElaboratedEnumSpecifierNode& node)
{
    BeginVisit(node);
    VisitKeyword("enum", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(OpaqueEnumDeclarationNode& node)
{
    BeginVisit(node);
    node.EnumKey()->Accept(*this);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.EnumHeadName()->Accept(*this);
    if (node.EnumBase())
    {
        node.EnumBase()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

// Expression:

void DefaultVisitor::Visit(BinaryExprNode& node)
{
    BeginVisit(node);
    node.Left()->Accept(*this);
    node.Op()->Accept(*this);
    node.Right()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(UnaryExprNode& node)
{
    BeginVisit(node);
    node.Op()->Accept(*this);
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ExpressionListNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    VisitListContent(node);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AssignmentInitNode& node)
{
    BeginVisit(node);
    VisitOperator("=", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(YieldExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("co_yield", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ConditionalExprNode& node)
{
    BeginVisit(node);
    node.Condition()->Accept(*this);
    node.Quest()->Accept(*this);
    node.ThenExpr()->Accept(*this);
    node.Colon()->Accept(*this);
    node.ElseExpr()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(CommaNode& node)
{
    BeginVisit(node);
    VisitOperator(",", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AssignNode& node)
{
    BeginVisit(node);
    VisitOperator("=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PlusAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("+=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(MinusAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("-=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(MulAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("*=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DivAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("/=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ModAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("%=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(XorAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("^=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AndAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("&=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(OrAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("|=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ShiftLeftAssignNode& node)
{
    BeginVisit(node);
    VisitOperator("<<=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ShiftRightAssignNode& node)
{
    BeginVisit(node);
    VisitOperator(">>=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(QuestNode& node)
{
    BeginVisit(node);
    VisitOperator("?", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ColonNode& node)
{
    BeginVisit(node);
    VisitOperator(":", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DisjunctionNode& node)
{
    BeginVisit(node);
    VisitOperator("||", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ConjunctionNode& node)
{
    BeginVisit(node);
    VisitOperator("&&", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(InclusiveOrNode& node)
{
    BeginVisit(node);
    VisitOperator("|", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ExclusiveOrNode& node)
{
    BeginVisit(node);
    VisitOperator("^", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AndNode& node)
{
    BeginVisit(node);
    VisitOperator("&", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(EqualNode& node)
{
    BeginVisit(node);
    VisitOperator("==", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NotEqualNode& node)
{
    BeginVisit(node);
    VisitOperator("!=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LessNode& node)
{
    BeginVisit(node);
    VisitOperator("<", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(GreaterNode& node)
{
    BeginVisit(node);
    VisitOperator(">", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LessOrEqualNode& node)
{
    BeginVisit(node);
    VisitOperator("<=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(GreaterOrEqualNode& node)
{
    BeginVisit(node);
    VisitOperator(">=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(CompareNode& node)
{
    BeginVisit(node);
    VisitOperator("<=>", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ShiftLeftNode& node)
{
    BeginVisit(node);
    VisitOperator("<<", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ShiftRightNode& node)
{
    BeginVisit(node);
    VisitOperator(">>", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PlusNode& node)
{
    BeginVisit(node);
    VisitOperator("+", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(MinusNode& node)
{
    BeginVisit(node);
    VisitOperator("-", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(MulNode& node)
{
    BeginVisit(node);
    VisitOperator("*", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DivNode& node)
{
    BeginVisit(node);
    VisitOperator("/", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ModNode& node)
{
    BeginVisit(node);
    VisitOperator("%", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DotStarNode& node)
{
    BeginVisit(node);
    VisitOperator(".*", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ArrowStarNode& node)
{
    BeginVisit(node);
    VisitOperator("->*", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(CastExprNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    node.TypeId()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(DerefNode& node)
{
    BeginVisit(node);
    VisitOperator("*", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AddrOfNode& node)
{
    BeginVisit(node);
    VisitOperator("&", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NotNode& node)
{
    BeginVisit(node);
    VisitOperator("!", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ComplementNode& node)
{
    BeginVisit(node);
    VisitOperator("~", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PrefixIncNode& node)
{
    BeginVisit(node);
    VisitOperator("++", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PrefixDecNode& node)
{
    BeginVisit(node);
    VisitOperator("--", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(AwaitExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("co_await", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(SizeOfTypeExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("sizeof", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(SizeOfPackExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("sizeof", node.GetSpan());
    VisitOperator("...", node.EllipsisSpan());
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(SizeOfUnaryExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("sizeof", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(AlignOfExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("alignof", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NoexceptExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("noexcept", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NewExprNode& node)
{
    BeginVisit(node);
    if (node.ColonColonHead())
    {
        node.ColonColonHead()->Accept(*this);
    }
    VisitKeyword("new", node.NewPos());
    if (node.Placement())
    {
        node.Placement()->Accept(*this);
    }
    node.Child()->Accept(*this);
    if (node.Initializer())
    {
        node.Initializer()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(NewPlacementNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    VisitListContent(node);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ParenNewTypeIdNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.LParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NewTypeIdNode& node)
{
    BeginVisit(node);
    node.TypeSpecifierSeq()->Accept(*this);
    if (node.NewDeclarator())
    {
        node.NewDeclarator()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ArrayDeletePtrNode& node)
{
    BeginVisit(node);
    if (node.ColonColonHead())
    {
        node.ColonColonHead()->Accept(*this);
    }
    VisitKeyword("delete", node.DeleteSpan());
    VisitOperator("[", node.LBracketSpan());
    VisitOperator("]", node.RBracketSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(DeletePtrNode& node)
{
    BeginVisit(node);
    if (node.ColonColonHead())
    {
        node.ColonColonHead()->Accept(*this);
    }
    VisitKeyword("delete", node.DeleteSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(SubscriptExprNode& node)
{
    BeginVisit(node);
    node.Child()->Accept(*this);
    VisitOperator("[", node.LBracketSpan());
    node.Index()->Accept(*this);
    VisitOperator("]", node.RBracketSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(InvokeExprNode& node)
{
    BeginVisit(node);
    node.Subject()->Accept(*this);
    if (node.LParenSpan().IsValid())
    {
        VisitOperator("(", node.LParenSpan());
    }
    VisitListContent(node);
    if (node.RParenSpan().IsValid())
    {
        VisitOperator(")", node.RParenSpan());
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(PairNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(DotNode& node)
{
    BeginVisit(node);
    VisitOperator(".", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ArrowNode& node)
{
    BeginVisit(node);
    VisitOperator("->", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(MemberExprNode& node)
{
    BeginVisit(node);
    node.Child()->Accept(*this);
    node.Op()->Accept(*this);
    node.GetId()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(PostfixIncExprNode& node)
{
    BeginVisit(node);
    node.Child()->Accept(*this);
    VisitOperator("++", node.OpSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PostfixDecExprNode& node)
{
    BeginVisit(node);
    node.Child()->Accept(*this);
    VisitOperator("--", node.OpSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(TypeIdExprNode& node)
{
    BeginVisit(node);
    VisitKeyword("typeid", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DynamicCastNode& node)
{
    BeginVisit(node);
    VisitKeyword("dynamic_cast", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(StaticCastNode& node)
{
    BeginVisit(node);
    VisitKeyword("static_cast", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ReinterpretCastNode& node)
{
    BeginVisit(node);
    VisitKeyword("reinterpret_cast", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ConstCastNode& node)
{
    BeginVisit(node);
    VisitKeyword("const_cast", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(CppCastExprNode& node)
{
    BeginVisit(node);
    node.Op()->Accept(*this);
    VisitOperator("<", node.LAngleSpan());
    node.TypeId()->Accept(*this);
    VisitOperator(">", node.RAngleSpan());
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ThisNode& node)
{
    BeginVisit(node);
    VisitKeyword("this", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ParenthesizedExprNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    node.Child()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(FoldExprNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    VisitSequence(node);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NewDeclaratorNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(ArrayNewDeclaratorNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(NewInitializerNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    VisitListContent(node);
    VisitOperator(")", node.RParenSpan());
}

void DefaultVisitor::Visit(BracedInitListNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(DesignatedInitializerNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(DesignatorNode& node)
{
    BeginVisit(node);
    VisitOperator(".", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(EllipsisNode& node)
{
    BeginVisit(node);
    VisitOperator("...", node.GetSpan());
    EndVisit(node);
}

// Function:

void DefaultVisitor::Visit(FunctionDefinitionNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    if (node.DeclSpecifiers())
    {
        node.DeclSpecifiers()->Accept(*this);
    }
    node.Declarator()->Accept(*this);
    if (node.Specifiers())
    {
        node.Specifiers()->Accept(*this);
    }
    node.FunctionBody()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(FunctionBodyNode& node)
{
    VisitUnaryNode(node);
}

void DefaultVisitor::Visit(DefaultNode& node)
{
    BeginVisit(node);
    VisitKeyword("default", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DeleteNode& node)
{
    BeginVisit(node);
    VisitKeyword("delete", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DefaultedOrDeletedFunctionNode& node)
{
    BeginVisit(node);
    node.Assign()->Accept(*this);
    node.DefaultOrDelete()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(NoDeclSpecFunctionDeclarationNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Declarator()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(OperatorNode& node)
{
    BeginVisit(node);
    VisitKeyword("operator", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NewArrayOpNode& node)
{
    BeginVisit(node);
    VisitKeyword("new", node.GetSpan());
    VisitOperator("[", node.LBracketSpan());
    VisitOperator("]", node.RBracketSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NewOpNode& node)
{
    BeginVisit(node);
    VisitKeyword("new", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DeleteArrayOpNode& node)
{
    BeginVisit(node);
    VisitKeyword("delete", node.GetSpan());
    VisitOperator("[", node.LBracketSpan());
    VisitOperator("]", node.RBracketSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DeleteOpNode& node)
{
    BeginVisit(node);
    VisitKeyword("delete", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(CoAwaitOpNode& node)
{
    BeginVisit(node);
    VisitKeyword("co_await", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(InvokeOpNode& node)
{
    BeginVisit(node);
    VisitOperator("()", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(SubscriptOpNode& node)
{
    BeginVisit(node);
    VisitOperator("[]", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(OperatorFunctionIdNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(ConversionFunctionIdNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(ConversionTypeIdNode& node)
{
    BeginVisit(node);
    node.TypeSpecifierSeq()->Accept(*this);
    if (node.ConversionDeclarator())
    {
        node.ConversionDeclarator()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ConversionDeclaratorNode& node)
{
    BeginVisit(node);
    node.PtrOp()->Accept(*this);
    if (node.ConversionDeclarator())
    {
        node.ConversionDeclarator()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(DestructorIdNode& node)
{
    BeginVisit(node);
    VisitOperator("~", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ParameterNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.DeclSpecifiers()->Accept(*this);
    if (node.Declarator())
    {
        node.Declarator()->Accept(*this);
    }
    if (node.Assign())
    {
        node.Assign()->Accept(*this);
        node.Initializer()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ParameterListNode& node)
{
    BeginVisit(node);
    VisitOperator("(", node.LParenSpan());
    VisitListContent(node);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NoexceptNode& node)
{
    BeginVisit(node);
    VisitKeyword("noexcept", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(FunctionTryBlockNode& node)
{
    BeginVisit(node);
    VisitKeyword("try", node.GetSpan());
    if (node.CtorInitializer())
    {
        node.CtorInitializer()->Accept(*this);
    }
    node.TryBlock()->Accept(*this);
    node.Handlers()->Accept(*this);
    EndVisit(node);
}

// Identifier:

void DefaultVisitor::Visit(IdentifierNode& node)
{
    BeginVisit(node);
    VisitIdentifier(node.Str(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(UnnamedNode& node)
{
    VisitSingleNode(node);
}

void DefaultVisitor::Visit(ColonColonNode& node)
{
    BeginVisit(node);
    VisitOperator("::", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NestedNameSpecifierNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(QualifiedIdNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(IdentifierListNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(ModuleNameNode& node)
{
    BeginVisit(node);
    VisitIdentifier(node.Str(), node.GetSpan());
    EndVisit(node);
}

// Lambda:

void DefaultVisitor::Visit(LambdaExpressionNode& node)
{
    BeginVisit(node);
    node.Introducer()->Accept(*this);
    if (node.TemplateParams())
    {
        node.TemplateParams()->Accept(*this);
    }
    node.Declarator()->Accept(*this);
    node.Body()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(LambdaIntroducerNode& node)
{
    BeginVisit(node);
    VisitOperator("[", node.LBracketSpan());
    if (node.Capture())
    {
        node.Capture()->Accept(*this);
    }
    VisitOperator("]", node.RBracketSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LambdaCaptureNode& node)
{
    VisitList(node);
}

void DefaultVisitor::Visit(DefaultRefCaptureNode& node)
{
    BeginVisit(node);
    VisitOperator("&", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DefaultCopyCaptureNode& node)
{
    BeginVisit(node);
    VisitOperator("=", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ByRefCaptureNode& node)
{
    BeginVisit(node);
    VisitOperator("&", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(SimpleCaptureNode& node)
{
    BeginVisit(node);
    if (node.ByRefCapture())
    {
        node.ByRefCapture()->Accept(*this);
    }
    node.Identifier()->Accept(*this);
    if (node.Ellipsis())
    {
        node.Ellipsis()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(CurrentObjectCopyCapture& node)
{
    BeginVisit(node);
    VisitOperator("*", node.GetSpan());
    VisitKeyword("this", node.ThisSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(CurrentObjectByRefCapture& node)
{
    BeginVisit(node);
    VisitKeyword("this", node.ThisSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(InitCaptureNode& node)
{
    BeginVisit(node);
    if (node.ByRefCapture())
    {
        node.ByRefCapture()->Accept(*this);
    }
    if (node.Ellipsis())
    {
        node.Ellipsis()->Accept(*this);
    }
    node.Identifier()->Accept(*this);
    node.Initializer()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(LambdaDeclaratorNode& node)
{
    BeginVisit(node);
    if (node.ParameterList())
    {
        node.ParameterList()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    if (node.RequiresClause())
    {
        node.RequiresClause()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(LambdaSpecifiersNode& node)
{
    if (!node.IsEmpty())
    {
        BeginVisit(node);
        if (node.DeclSpecifiers())
        {
            node.DeclSpecifiers()->Accept(*this);
        }
        if (node.NoexceptionSpecifier())
        {
            node.NoexceptionSpecifier()->Accept(*this);
        }
        if (node.Attributes())
        {
            node.Attributes()->Accept(*this);
        }
        if (node.TrailingReturnType())
        {
            node.TrailingReturnType()->Accept(*this);
        }
        EndVisit(node);
    }
}

void DefaultVisitor::Visit(LambdaTemplateParamsNode& node)
{
    BeginVisit(node);
    node.TemplateParams()->Accept(*this);
    if (node.RequiresClause())
    {
        node.RequiresClause()->Accept(*this);
    }
    EndVisit(node);
}

// Literal:

void DefaultVisitor::Visit(IntegerLiteralNode& node)
{
    BeginVisit(node);
    VisitLiteral(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(FloatingLiteralNode& node)
{
    BeginVisit(node);
    VisitLiteral(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(CharacterLiteralNode& node)
{
    BeginVisit(node);
    VisitLiteral(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(StringLiteralNode& node)
{
    BeginVisit(node);
    VisitLiteral(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(RawStringLiteralNode& node)
{
    BeginVisit(node);
    VisitLiteral(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(BooleanLiteralNode& node)
{
    BeginVisit(node);
    VisitLiteral(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(NullPtrLiteralNode& node)
{
    BeginVisit(node);
    VisitLiteral(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(UserDefinedLiteraNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(LiteralOperatorIdNode& node)
{
    BeginVisit(node);
    VisitKeyword("operator", node.GetSpan());
    VisitOperator("\"\"", node.StringLitSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

// Module:

void DefaultVisitor::Visit(ModuleDeclarationNode& node)
{
    BeginVisit(node);
    if (node.Export())
    {
        node.Export()->Accept(*this);
    }
    node.Module()->Accept(*this);
    node.ModuleName()->Accept(*this);
    if (node.ModulePartition())
    {
        node.ModulePartition()->Accept(*this);
    }
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ExportDeclarationNode& node)
{
    BeginVisit(node);
    node.Export()->Accept(*this);
    if (node.LBraceSpan().IsValid())
    {
        VisitOperator("{", node.LBraceSpan());
        if (node.Subject())
        {
            node.Subject()->Accept(*this);
        }
        VisitOperator("}", node.RBraceSpan());
    }
    else
    {
        node.Subject()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ExportNode& node)
{
    BeginVisit(node);
    VisitKeyword("export", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ImportNode& node)
{
    BeginVisit(node);
    VisitKeyword("import", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ImportDeclarationNode& node)
{
    BeginVisit(node);
    node.Import()->Accept(*this);
    node.Subject()->Accept(*this);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ModulePartitionNode& node)
{
    BeginVisit(node);
    VisitOperator(":", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ModuleNode& node)
{
    BeginVisit(node);
    VisitKeyword("module", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(GlobalModuleFragmentNode& node)
{
    BeginVisit(node);
    node.Module()->Accept(*this);
    node.Semicolon()->Accept(*this);
    if (node.Declarations())
    {
        node.Declarations()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(PrivateModuleFragmentNode& node)
{
    BeginVisit(node);
    node.Module()->Accept(*this);
    node.Colon()->Accept(*this);
    node.Private()->Accept(*this);
    node.Semicolon()->Accept(*this);
    if (node.Declarations())
    {
        node.Declarations()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(AngleHeaderName& node)
{
    BeginVisit(node);
    VisitHeaderName(node.Rep(), node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(QuoteHeaderName& node)
{
    BeginVisit(node);
    VisitHeaderName(node.Rep(), node.GetSpan());
    EndVisit(node);
}

// Qualifier:

void DefaultVisitor::Visit(ConstNode& node)
{
    BeginVisit(node);
    VisitKeyword("const", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(VolatileNode& node)
{
    BeginVisit(node);
    VisitKeyword("volatile", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LvalueRefNode& node)
{
    BeginVisit(node);
    VisitOperator("&", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(RvalueRefNode& node)
{
    BeginVisit(node);
    VisitOperator("&&", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PtrNode& node)
{
    BeginVisit(node);
    VisitOperator("*", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(CVQualifierSequenceNode& node)
{
    VisitSequence(node);
}

// SimpleType:

void DefaultVisitor::Visit(CharNode& node)
{
    BeginVisit(node);
    VisitKeyword("char", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(Char8Node& node)
{
    BeginVisit(node);
    VisitKeyword("char8_t", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(Char16Node& node)
{
    BeginVisit(node);
    VisitKeyword("char16_t", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(Char32Node& node)
{
    BeginVisit(node);
    VisitKeyword("char32_t", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(WCharNode& node)
{
    BeginVisit(node);
    VisitKeyword("wchar_t", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(BoolNode& node)
{
    BeginVisit(node);
    VisitKeyword("bool", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ShortNode& node)
{
    BeginVisit(node);
    VisitKeyword("short", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(IntNode& node)
{
    BeginVisit(node);
    VisitKeyword("int", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(LongNode& node)
{
    BeginVisit(node);
    VisitKeyword("long", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(SignedNode& node)
{
    BeginVisit(node);
    VisitKeyword("signed", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(UnsignedNode& node)
{
    BeginVisit(node);
    VisitKeyword("unsigned", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(FloatNode& node)
{
    BeginVisit(node);
    VisitKeyword("float", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DoubleNode& node)
{
    BeginVisit(node);
    VisitKeyword("double", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(VoidNode& node)
{
    BeginVisit(node);
    VisitKeyword("void", node.GetSpan());
    EndVisit(node);
}

// Statement:

void DefaultVisitor::Visit(LabeledStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Label()->Accept(*this);
    VisitOperator(":", node.ColonSpan());
    node.Statement()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(CaseStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("case", node.CaseSpan());
    node.CaseExpression()->Accept(*this);
    VisitOperator(":", node.ColonSpan());
    node.Statement()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(DefaultStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("default", node.DefaultSpan());
    VisitOperator(":", node.ColonSpan());
    node.Statement()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(CompoundStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitOperator("{", node.LBraceSpan());
    VisitSequenceContent(node);
    VisitOperator("}", node.RBraceSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(SequenceStatementNode& node)
{
    BeginVisit(node);
    node.FirstStmt()->Accept(*this);
    node.SecondStmt()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(IfStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("if", node.IfSpan());
    if (node.IsConstExprIf())
    {
        VisitKeyword("constexpr", node.ConstExprSpan());
    }
    VisitOperator("(", node.LParenSpan());
    node.Condition()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.ThenStatement()->Accept(*this);
    if (node.ElseStatement())
    {
        VisitKeyword("else", node.ElseSpan());
        node.ElseStatement()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(SwitchStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("switch", node.SwitchSpan());
    VisitOperator("(", node.LParenSpan());
    node.Condition()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.Statement()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(WhileStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("while", node.WhileSpan());
    VisitOperator("(", node.LParenSpan());
    node.Condition()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.Statement()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(DoStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("do", node.DoSpan());
    node.Statement()->Accept(*this);
    VisitKeyword("while", node.WhileSpan());
    VisitOperator("(", node.LParenSpan());
    node.Expression()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(RangeForStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("for", node.ForSpan());
    VisitOperator("(", node.LParenSpan());
    if (node.InitStatement())
    {
        node.InitStatement()->Accept(*this);
    }
    node.Declaration()->Accept(*this);
    VisitOperator(":", node.ColonSpan());
    node.Initializer()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.Statement()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ForRangeDeclarationNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(StructuredBindingNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.DeclSpecifiers()->Accept(*this);
    if (node.RefQualifier())
    {
        node.RefQualifier()->Accept(*this);
    }
    VisitOperator("[", node.LBracketSpan());
    node.Identifiers()->Accept(*this);
    VisitOperator("]", node.RBracketSpan());
    if (node.Initializer())
    {
        node.Initializer()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ForStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("for", node.ForSpan());
    VisitOperator("(", node.LParenSpan());
    node.InitStatement()->Accept(*this);
    if (node.Condition())
    {
        node.Condition()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    if (node.LoopExpr())
    {
        node.LoopExpr()->Accept(*this);
    }
    VisitOperator(")", node.RParenSpan());
    node.Statement()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(BreakStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("break", node.BreakSpan());
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ContinueStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("continue", node.ContinueSpan());
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ReturnStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("return", node.ReturnSpan());
    if (node.ReturnValue())
    {
        node.ReturnValue()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(CoReturnStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("co_return", node.CoReturnSpan());
    if (node.ReturnValue())
    {
        node.ReturnValue()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(GotoStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("goto", node.GotoSpan());
    node.Target()->Accept(*this);
    if (node.Semimcolon())
    {
        node.Semimcolon()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(TryStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    VisitKeyword("try", node.TrySpan());
    node.TryBlock()->Accept(*this);
    node.Handlers()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(HandlerSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(HandlerNode& node)
{
    BeginVisit(node);
    VisitKeyword("catch", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Exception()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.CatchBlock()->Accept(*this);
}

void DefaultVisitor::Visit(ExceptionDeclarationNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    if (node.TypeSpecifiers())
    {
        node.TypeSpecifiers()->Accept(*this);
    }
    if (node.Declarator())
    {
        node.Declarator()->Accept(*this);
    }
    if (node.Ellipsis())
    {
        node.Ellipsis()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ExpressionStatementNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    if (node.Expression())
    {
        node.Expression()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(DeclarationStatementNode& node)
{
    BeginVisit(node);
    node.Declaration()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(InitConditionNode& node)
{
    BeginVisit(node);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.DeclSpecifiers()->Accept(*this);
    node.Declarator()->Accept(*this);
    node.Initializer()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(SemicolonNode& node)
{
    BeginVisit(node);
    VisitOperator(";", node.GetSpan());
    EndVisit(node);
}

// Template:

void DefaultVisitor::Visit(TemplateDeclarationNode& node)
{
    VisitBinaryNode(node);
}

void DefaultVisitor::Visit(TemplateHeadNode& node)
{
    BeginVisit(node);
    VisitKeyword("template", node.GetSpan());
    node.TemplateParamList()->Accept(*this);
    if (node.RequiresClause())
    {
        node.RequiresClause()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(TemplateParameterListNode& node)
{
    BeginVisit(node);
    VisitOperator("<", node.LAngleSpan());
    VisitListContent(node);
    VisitOperator(">", node.RAngleSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(TypeParameterNode& node)
{
    BeginVisit(node);
    if (node.TemplateHead())
    {
        node.TemplateHead()->Accept(*this);
    }
    node.TypeConstraint()->Accept(*this);
    if (node.Ellipsis())
    {
        node.Ellipsis()->Accept(*this);
    }
    if (node.Identifier())
    {
        node.Identifier()->Accept(*this);
    }
    if (node.Assign())
    {
        node.Assign()->Accept(*this);
    }
    if (node.TypeId())
    {
        node.TypeId()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(TemplateIdNode& node)
{
    BeginVisit(node);
    node.TemplateName()->Accept(*this);
    VisitOperator("<", node.LAngleSpan());
    VisitListContent(node);
    VisitOperator(">", node.RAngleSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(TypenameNode& node)
{
    BeginVisit(node);
    VisitKeyword("typename", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(DeductionGuideNode& node)
{
    BeginVisit(node);
    if (node.ExplicitSpecifier())
    {
        node.ExplicitSpecifier()->Accept(*this);
    }
    node.TemplateName()->Accept(*this);
    VisitOperator("(", node.LParenSpan());
    node.Parameters()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    node.Arrow()->Accept(*this);
    node.TemplateId()->Accept(*this);
    node.Semicolon()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ExplicitInstantiationNode& node)
{
    BeginVisit(node);
    if (node.Extern())
    {
        node.Extern()->Accept(*this);
    }
    node.Template()->Accept(*this);
    node.Declaration()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(TemplateNode& node)
{
    BeginVisit(node);
    VisitKeyword("template", node.GetSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(ExplicitSpecializationNode& node)
{
    BeginVisit(node);
    node.Template()->Accept(*this);
    VisitOperator("<", node.LAngleSpan());
    VisitOperator(">", node.RAngleSpan());
    node.Declaration()->Accept(*this);
    EndVisit(node);
}

// TranslationUnit:

void DefaultVisitor::Visit(TranslationUnitNode& node)
{
    BeginVisit(node);
    if (node.Unit())
    {
        node.Unit()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(ModuleUnitNode& node)
{
    BeginVisit(node);
    if (node.GlobalModuleFragment())
    {
        node.GlobalModuleFragment()->Accept(*this);
    }
    node.ModuleDeclaration()->Accept(*this);
    if (node.Declarations())
    {
        node.Declarations()->Accept(*this);
    }
    if (node.PrivateModuleFragment())
    {
        node.PrivateModuleFragment()->Accept(*this);
    }
    EndVisit(node);
}

// Type:

void DefaultVisitor::Visit(TypeSpecifierSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(TypenameSpecifierNode& node)
{
    BeginVisit(node);
    VisitKeyword("typename", node.GetSpan());
    node.NestedNameSpecifier()->Accept(*this);
    if (node.TemplateNode())
    {
        node.TemplateNode()->Accept(*this);
    }
    node.GetId()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(TypeIdNode& node)
{
    BeginVisit(node);
    node.TypeSpecifiers()->Accept(*this);
    if (node.Declarator())
    {
        node.Declarator()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(DefiningTypeIdNode& node)
{
    BeginVisit(node);
    node.DefiningTypeSpecifiers()->Accept(*this);
    if (node.AbstractDeclarator())
    {
        node.AbstractDeclarator()->Accept(*this);
    }
    EndVisit(node);
}

void DefaultVisitor::Visit(DefiningTypeSpecifierSequenceNode& node)
{
    VisitSequence(node);
}

void DefaultVisitor::Visit(TrailingReturnTypeNode& node)
{
    BeginVisit(node);
    VisitOperator("->", node.GetSpan());
    node.Child()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(ElaboratedTypeSpecifierNode& node)
{
    BeginVisit(node);
    node.ClassKey()->Accept(*this);
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.GetId()->Accept(*this);
    EndVisit(node);
}

void DefaultVisitor::Visit(DeclTypeSpecifierNode& node)
{
    BeginVisit(node);
    VisitKeyword("delctype", node.GetSpan());
    VisitOperator("(", node.LParenSpan());
    node.Expression()->Accept(*this);
    VisitOperator(")", node.RParenSpan());
    EndVisit(node);
}

void DefaultVisitor::Visit(PlaceholderTypeSpecifierNode& node)
{
    BeginVisit(node);
    if (node.TypeConstraint())
    {
        node.TypeConstraint()->Accept(*this);
    }
    if (node.IsDeclType())
    {
        VisitKeyword("delctype", node.DeclTypeSpan());
        VisitOperator("(", node.LParenSpan());
        VisitKeyword("auto", node.AutoSpan());
        VisitOperator(")", node.RParenSpan());
    }
    else
    {
        VisitKeyword("auto", node.AutoSpan());
    }
    EndVisit(node);
}

} // namespace otava::ast
