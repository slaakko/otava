// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.expression;

import otava.symbols.symbol;
import otava.symbols.context;
import otava.symbols.symbol_table;
import otava.ast.expression;
import otava.ast.identifier;

namespace otava::symbols {

bool MultiplicativeRightIdOperandNotFound(otava::ast::Node* op, otava::ast::Node* rightOperand, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!op->IsMulNode()) return false;
    if (!rightOperand->IsInvokeExprNode()) return false;
    otava::ast::InvokeExprNode* rightinvoke = static_cast<otava::ast::InvokeExprNode*>(rightOperand);
    if (!rightinvoke->Subject()->IsIdentifierNode()) return false;
    otava::ast::IdentifierNode* id = static_cast<otava::ast::IdentifierNode*>(rightinvoke->Subject());
    Symbol* symbol = context->GetSymbolTable()->CurrentScope()->Lookup(id->Str(), SymbolGroupKind::all, ScopeLookup::allScopes, fullSpan, context, LookupFlags::none);
    if (!symbol) return true;
    return false;
}

} // namespace otava::symbols
