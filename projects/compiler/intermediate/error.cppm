// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.error;

import soul.ast.span;
import std;

export namespace otava::intermediate {

class IntermediateError : public std::runtime_error
{
public:
    IntermediateError(const std::string& message_);
    inline const std::string& Message() const noexcept { return message; }
private:
    std::string message;
};

class IntermediateContext;

bool ExceptionThrown();
void SetExceptionThrown();
[[noreturn]] void Error(const std::string& message, const soul::ast::Span& span, IntermediateContext* context);
[[noreturn]] void Error(const std::string& message, const soul::ast::Span& span, IntermediateContext* context, const soul::ast::Span& refSpan);
void Warning(const std::string& message, const soul::ast::Span& span, IntermediateContext* context);

} // otava::intermediate
