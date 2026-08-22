// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.evaluator;

import std;
import otava.ast.node;
import otava.ast.function;
import otava.ast.literal;
import soul.ast.span;

export namespace otava::symbols {

class Value;
class Context;
class TypeSymbol;
class FunctionSymbol;
class BoundExpressionNode;

struct EvaluationMapSetter
{
    EvaluationMapSetter(Context* context_);
    ~EvaluationMapSetter();
    Context* context;
};

TypeSymbol* GetIntegerType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context);
TypeSymbol* GetFloatingPointType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context);
TypeSymbol* GetStringType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context);
TypeSymbol* GetCharacterType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context);
otava::ast::FunctionDefinitionNode* GetFunctionDefinitionNode(FunctionSymbol* fn, Context* context);
void PopulateEvaluationMap(FunctionSymbol* fn, const std::vector<Value*>& args, Context* context);
std::vector<std::unique_ptr<BoundExpressionNode>> ValuesToLiterals(const std::vector<Value*>& values, const soul::ast::FullSpan& fullSpan, Context* context);
std::vector<Value*> ExpressionsToValues(const std::vector<std::unique_ptr<BoundExpressionNode>>& exprs, const soul::ast::FullSpan& fullSpan, Context* context, 
    bool& failed);
Value* Evaluate(otava::ast::Node* node, Context* context);

} // namespace otava::symbols
