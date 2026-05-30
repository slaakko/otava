// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.node;

import std;
import soul.ast.span;
import otava.ast.node_list;

export namespace otava::ast {

class Node;
class Visitor;
class Writer;
class Reader;

enum class NodeType
{
    single, compound, unary, binary, sequence, list
};

std::string NodeTypeStr(NodeType nodeType);

using NodeDestroyedFunc = void(*)(Node*);

void SetNodeDestroyedFunc(NodeDestroyedFunc func) noexcept;

constexpr std::int32_t internallyMappedFlag = 0x40000000;
constexpr std::int32_t destructorNodeId = 0x20000000;

enum class NodeKind : std::uint16_t
{
    nullNode,
    translationUnitNode, moduleUnitNode,
    moduleDeclarationNode, exportDeclarationNode, exportNode, importNode, importDeclarationNode, modulePartitionNode, moduleNode, globalModuleFragmentNode, 
    privateModuleFragmentNode, angleHeaderNameNode, quoteHeaderNameNode,
    identifierNode, unnamedNode, colonColonNode, nestedNameSpecifierNode, qualifiedIdNode, identifierListNode, moduleNameNode,
    charNode, char8Node, char16Node, char32Node, wcharNode, boolNode, shortNode, intNode, longNode, signedNode, unsignedNode, floatNode, doubleNode, voidNode,
    constNode, volatileNode, lvalueRefNode, rvalueRefNode, ptrNode, cvQualifierSequenceNode, ptrOperatorNode, noexceptSpecifierNode, throwSpecifierNode, 
    packDeclaratorIdNode, arrayDeclaratorNode, functionDeclaratorNode, prefixNode, trailingQualifiersNode, trailingAttributesNode, ptrDeclaratorNode,
    expressionListNode, assignmentInitializerNode,
    conditionalExprNode, questNode, colonNode, commaNode, binaryExprNode, unaryExprNode, yieldExprNode, throwExprNode,
    assignNode, plusAssignNode, minusAssignNode, mulAssignNode, divAssignNode, modAssignNode, xorAssignNode, andAssignNode, orAssignNode, 
    shiftLeftAssignNode, shiftRightAssignNode,
    disjunctionNode, conjunctionNode, inclusiveOrNode, exclusiveOrNode, andNode, equalNode, notEqualNode, lessNode, greaterNode, lessOrEqualNode, greaterOrEqualNode,
    compareNode, shiftLeftNode, shiftRightNode, plusNode, minusNode, mulNode, divNode, modNode, dotStarNode, arrowStarNode,
    castExprNode, derefNode, addrOfNode, notNode, complementNode, prefixIncNode, prefixDecNode,
    awaitExprNode, sizeOfTypeExprNode, sizeOfPackExpNode, sizeOfUnaryExprNode, alignOfExprNode, noexceptExprNode,
    foldExprNode, newDeclaratorNode, arrayNewDeclaratorNode, newInitializerNode, ellipsisNode, bracedInitListNode, designatedInitializerNode, designatorNode,
    newExprNode, newPlacementNode, parenNewTypeIdNode, newTypeIdNode, arrayDeletePtrNode, deletePtrNode,
    subscriptExprNode, invokeExprNode, dotNode, templateNode, arrowNode, pairNode, memberExprNode, postfixIncExprNode, postfixDecExprNode, typeIdExprNode,
    dynamicCastNode, staticCastNode, reinterpretCastNode, constCastNode, cppCastExprNode, thisNode, parenExprNode,
    lambdaExpressionNode, lambdaIntroducerNode, lambdaCaptureNode, lambdaDeclaratorNode, lambdaSpecifiersNode, lambdaTemplateParamsNode,
    defaultRefCaptureNode, defaultCopyCaptureNode, byRefCaptureNode, simpleCaptureNode, initCaptureNode, currentObjectCopyCapture, currentObjectByRefCapture,
    integerLiteralNode, floatingLiteralNode, characterLiteralNode, stringLiteralNode, rawStringLiteralNode, booleanLiteralNode, nullPtrLiteralNode, 
    userDefinedLiteralNode, literalOperatorIdNode,
    labeledStatementNode, caseStatmentNode, defaultStatementNode, compoundStatementNode, sequenceStatementNode, ifStatementNode, switchStatemeNode, 
    whileStatementNode, doStatementNode, rangeForStatementNode, forRangeDeclarationNode, structuredBindingNode, forStatementNode, 
    breakStatementNode, continueStatementNode, returnStatementNode, coReturnStatementNode,
    gotoStatementNode, tryStatementNode, handlerSequenceNode, handlerNode, exceptionDeclarationNode, expressionStatementNode, declarationStatementNode, initConditionNode, 
    semicolonNode,
    templateDeclarationNode, templateHeadNode, templateParameterListNode, typeParameterNode, templateIdNode, typenameNode, deductionGuideNode, explicitInstantiationNode, 
    explicitSpecializationNode,
    declarationSequenceNode, simpleDeclarationNode, usingDeclarationNode, usingNode, usingDeclaratorListNode, usingEnumDeclarationNode, 
    emptyDeclarationNode, namespaceNode, usingDirectiveNode,
    asmDeclarationNode, asmNode, namespaceAliasDefinitionNode, staticAssertDeclarationNode, staticAssertNode, aliasDeclarationNode, definingTypeIdNode, 
    definingTypeSpecifierSequenceNode,
    opaqueEnumDeclarationNode, noDeclSpecFunctionDeclarationNode, linkageSpecificationNode, namespaceDefinitionNode, namespaceBodyNode, attributeDeclarationNode,
    initDeclaratorListNode, initDeclaratorNode, trailingFunctionDeclaratorNode, parenthesizedDeclaratorNode, abstractDeclaratorNode, declSpecifierSequenceNode,
    friendNode, typedefNode, constExprNode, constEvalNode, constInitNode, inlineNode, staticNode, threadLocalNode, externNode, mutableNode, virtualNode, explicitNode, 
    conditionalExplicitNode,
    qualifiedPtrNode,
    typeSpecifierSequenceNode, typenameSpecifierNode, typeIdNode, trailingReturnTypeNode, elaboratedTypeSpecifierNode, declTypeSpecifierNode, placeholderTypeSpecifierNode,
    classSpecifierNode, classHeadNode, baseClauseNode, baseSpecifierListNode, baseSpecifierNode, beginAccessGroupNode, memberDeclarationNode, memberDeclaratorListNode, 
    virtSpecifierSequenceNode,
    constructorNode, constructorInitializerNode, memberInitializerListNode, memberInitializerNode,
    publicNode, protectedNode, privateNode, classNode, structNode, unionNode, overrideNode, finalNode, pureSpecifierNode,
    enumSpecifierNode, enumHeadNode, enumBaseNode, enumClassNode, enumStructNode, enumNode, enumeratorDefinitionNode, enumeratorNode, elaboratedEnumSpecifierNode,
    functionDefinitionNode, functionBodyNode, defaultedOrDeletedFunctionNode, defaultNode, deleteNode,
    operatorFnIdNode, operatorNode, newArrayOpNode, newOpNode, deleteArrayOpNode, deleteOpNode, coAwaitOpNode, invokeOpNode, subscriptOpNode,
    conversionFnIdNode, conversionTypeIdNode, conversionDeclaratorNode, destructorIdNode, parameterNode, parameterListNode, noexceptNode, functionTryBlock,
    conceptDefinitionNode, requiresExprNode, requirementBodyNode, simpleRequirementNode, typeRequirementNode, compoundRequirementNode, returnTypeRequirementNode, 
    nestedRequirementNode,
    typeConstraintNode, requiresClauseNode,
    attributeSpecifierSequenceNode, attributeSpecifierNode, attributeUsingPrefixNode, attributeNode, attributeScopedTokenNode,
    attributeArgumentsNode, balancedTokenSequenceNode, tokenNode,
    lparenNode, rparenNode, lbracketNode, rbracketNode, lbraceNode, rbraceNode, alignmentSpecifierNode,
    pragmaNode, boundStatementNode, opNewCall,
    max
};

std::string NodeKindStr(NodeKind nodeKind);

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_) noexcept;
    virtual ~Node();
    virtual Node* Clone() const = 0;
    inline NodeKind Kind() const noexcept { return kind; }
    inline std::int64_t Id() const noexcept { return id; }
    inline void SetId(std::int64_t id_) noexcept { id = id_; }
    virtual std::string Str() const { return std::string(); }
    virtual NodeType Type() const noexcept { return NodeType::single; }
    virtual int Count() const noexcept { return 0; }
    inline const soul::ast::Span& GetSpan() const noexcept { return span; }
    inline soul::ast::FullSpan GetFullSpan() const noexcept { return soul::ast::FullSpan(fileIndex, span); }
    inline void SetSpan(const soul::ast::Span& span_) noexcept { span = span_; }
    inline int FileIndex() const noexcept { return fileIndex; }
    inline void SetFileIndex(int fileIndex_) noexcept { fileIndex = fileIndex_; }
    virtual void Accept(Visitor& visitor) = 0;
    virtual void Write(Writer& writer);
    virtual void Read(Reader& reader);
    virtual void AddNode(Node* node);
    virtual void Clear();
    inline Node* Parent() const noexcept { return parent; }
    inline void SetParent(Node* parent_) noexcept { parent = parent_; }
    inline std::int32_t NodeId() const noexcept { return id & 0xFFFFFFFF; }
    inline std::int64_t InternalId() const noexcept { return id & ~internallyMappedFlag; }
    inline bool IsInternallyMapped() const noexcept { return (id & internallyMappedFlag) != 0; }
    inline bool IsImportDeclarationNode() const noexcept { return kind == NodeKind::importDeclarationNode; }
    inline bool IsClassNode() const noexcept { return kind == NodeKind::classNode; }
    inline bool IsStructNode() const noexcept { return kind == NodeKind::structNode;  }
    inline bool IsUnionNode() const noexcept { return kind == NodeKind::unionNode; }
    inline bool IsClassSpecifierNode() const noexcept { return kind == NodeKind::classSpecifierNode; }
    inline bool IsCompoundStatementNode() const noexcept { return kind == NodeKind::compoundStatementNode; }
    inline bool IsSequenceStatemetnNode() const noexcept { return kind == NodeKind::sequenceStatementNode; }
    inline bool IsReturnStatementNode() const noexcept { return kind == NodeKind::returnStatementNode; }
    inline bool IsInitConditionNode() const noexcept { return kind == NodeKind::initConditionNode; }
    inline bool IsCtorInitializerNode() const noexcept { return kind == NodeKind::constructorInitializerNode; }
    inline bool IsFunctionBodyNode() const noexcept { return kind == NodeKind::functionBodyNode; }
    inline bool IsConstructorNode() const noexcept { return kind == NodeKind::constructorNode; }
    inline bool IsEnumeratorNode() const noexcept { return kind == NodeKind::enumeratorNode; }
    inline bool IsIdentifierNode() const noexcept { return kind == NodeKind::identifierNode; }
    inline bool IsQualifiedIdNode() const noexcept { return kind == NodeKind::qualifiedIdNode; }
    inline bool IsTemplateIdNode() const noexcept { return kind == NodeKind::templateIdNode; }
    inline bool IsArrowNode() const noexcept { return kind == NodeKind::arrowNode; }
    inline bool IsDotNode() const noexcept { return kind == NodeKind::dotNode; }
    inline bool IsMulNode() const noexcept { return kind == NodeKind::mulNode; }
    inline bool IsInvokeExprNode() const noexcept { return kind == NodeKind::invokeExprNode; }
    inline bool IsMemberExprNode() const noexcept { return kind == NodeKind::memberExprNode; }
    inline bool IsBinaryExprNode() const noexcept { return kind == NodeKind::binaryExprNode; }
    inline bool IsExpressionListNode() const noexcept { return kind == NodeKind::expressionListNode; }
    inline bool IsNewInitializerNode() const noexcept { return kind == NodeKind::newInitializerNode; }
    inline bool IsBracedInitListNode() const noexcept { return kind == NodeKind::bracedInitListNode; }
    inline bool IsAssignmentInitializerNode() const noexcept { return kind == NodeKind::assignmentInitializerNode; }
    inline bool IsDefaultedOrDeletedFunctionNode() const noexcept { return kind == NodeKind::defaultedOrDeletedFunctionNode; }
    inline bool IsStringLiteralNode() const noexcept { return kind == NodeKind::stringLiteralNode; }
    inline bool IsFunctionDefinitionNode() const noexcept { return kind == NodeKind::functionDefinitionNode; }
    inline bool IsNewPlacementNode() const noexcept { return kind == NodeKind::newPlacementNode; }
    inline bool IsCommaNode() const noexcept { return kind == NodeKind::commaNode; }
    inline bool IsStaticNode() const noexcept { return kind == NodeKind::staticNode; }
    inline bool IsBoundStatementNode() const noexcept { return kind == NodeKind::boundStatementNode; }
    inline bool IsInitDeclaratorNode() const noexcept { return kind == NodeKind::initDeclaratorNode; }
    inline bool IsAliasDeclarationNode() const noexcept { return kind == NodeKind::aliasDeclarationNode; }
    inline bool IsInitDeclaratorListNode() const noexcept { return kind == NodeKind::initDeclaratorListNode; }
    inline bool IsLBraceNode() const noexcept { return kind == NodeKind::lbraceNode; }
    inline bool IsRBraceNode() const noexcept { return kind == NodeKind::rbraceNode; }
    inline bool IsFunctionDeclaratorNode() const noexcept { return kind == NodeKind::functionDeclaratorNode; }
    inline bool IsTypenameSpecifierNode() const noexcept { return kind == NodeKind::typenameSpecifierNode; }
    inline bool IsConstNode() const noexcept { return kind == NodeKind::constNode; }
    inline bool IsLvalueRefNode() const noexcept { return kind == NodeKind::lvalueRefNode; }
    inline bool IsRvalueRefNode() const noexcept { return kind == NodeKind::rvalueRefNode; }
    inline bool IsPtrNode() const noexcept { return kind == NodeKind::ptrNode; }
    inline bool IsTypeSpecifierSequenceNode() const { return kind == NodeKind::typeSpecifierSequenceNode; }
    inline bool IsOpNewCall() const { return kind == NodeKind::opNewCall; }
    inline bool IsPtrDeclaratorNode() const noexcept { return kind == NodeKind::ptrDeclaratorNode; }
    inline bool IsSimpleDeclarationNode() const noexcept { return kind == NodeKind::simpleDeclarationNode; }
private:
    NodeKind kind;
    soul::ast::Span span;
    int fileIndex;
    Node* parent;
    std::int64_t id;
};

class CompoundNode : public Node
{
public:
    CompoundNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_) noexcept;
    NodeType Type() const noexcept override { return NodeType::compound; }
};

class UnaryNode : public Node
{
public:
    UnaryNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    UnaryNode(const UnaryNode&) = delete;
    UnaryNode& operator=(const UnaryNode&) = delete;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    NodeType Type() const noexcept override { return NodeType::unary; }
    int Count() const noexcept override { return 1; }
    inline Node* Child() const noexcept  { return child.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> child;
};

class BinaryNode : public Node
{
public:
    BinaryNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_, Node* left_, Node* right_) noexcept;
    BinaryNode(const BinaryNode&) = delete;
    BinaryNode& operator=(const BinaryNode&) = delete;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    NodeType Type() const noexcept override { return NodeType::binary; }
    int Count() const noexcept override { return 2; }
    inline Node* Left() const noexcept { return left.get(); }
    inline Node* Right() const noexcept { return right.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class SequenceNode : public Node
{
public:
    SequenceNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_) noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    NodeType Type() const noexcept override { return NodeType::sequence; }
    int Count() const noexcept override { return nodes.Count(); }
    void AddNode(Node* node) override;
    void InsertNode(int index, Node* node);
    void Clear() override;
    inline NodeList<Node>& Nodes() noexcept { return nodes; }
    inline const NodeList<Node>& Nodes() const noexcept { return nodes; }
    std::string Str() const override;
private:
    NodeList<Node> nodes;
};

class ListNode : public Node
{
public:
    ListNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_) noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    NodeType Type() const noexcept override { return NodeType::list; }
    int Count() const noexcept override { return nodes.Count(); }
    void AddNode(Node* node) override;
    void Clear() override;
    inline NodeList<Node>& Nodes() noexcept { return nodes; }
    inline const NodeList<Node>& Nodes() const noexcept { return nodes; }
    inline const std::vector<Node*>& Items() const noexcept { return items; }
    std::string Str() const override;
private:
    NodeList<Node> nodes;
    std::vector<Node*> items;
};

class NodeIdFactory
{
public:
    NodeIdFactory() noexcept;
    void SetInternallyMapped(bool internallyMapped_) noexcept;
    bool IsInternallyMapped() const noexcept;
    inline void SetModuleId(std::int32_t moduleId_) noexcept { moduleId = moduleId_; }
    std::int64_t GetNextNodeId() noexcept;
private:
    std::int32_t moduleId;
    std::int32_t nodeId;
    std::int32_t internallyMappedBit;
};

void MakeNodeFactoryCollection();
NodeIdFactory* GetNodeIdFactory() noexcept;
void SetNodeIdFactory(NodeIdFactory* factory) noexcept;
Node* CreateNode(NodeKind nodeKind, const soul::ast::Span& span, int fileIndex);
std::int64_t GetNextNodeId() noexcept;

} // namespace otava::ast
