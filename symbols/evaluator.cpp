// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.evaluator;

namespace otava::symbols {

TypeSymbol* GetIntegerType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

TypeSymbol* GetFloatingPointType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

TypeSymbol* GetStringType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

TypeSymbol* GetCharacterType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

Value* Evaluate(otava::ast::Node* node, Context* context)
{
    // TODO
    return nullptr;
}

} // namespace otava::symbols

