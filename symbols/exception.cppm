// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.exception;

import std;
import soul.ast.span;

export namespace otava::symbols {

bool ExceptionThrown();
void SetExceptionThrown();

class Context;

class Exception : public std::runtime_error
{
public:
    Exception(const std::string& message_);
    Exception(const std::string& message_, const soul::ast::FullSpan& fullSpan, Context* context);
    Exception(const std::string& message_, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, Context* context);
    Exception(const std::string& title, const std::string& message_, const soul::ast::FullSpan& fullSpan, Context* context);
    Exception(const std::string& title, const std::string& message_, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, Context* context);
    inline void SetWarning() noexcept { warning = true; }
    inline bool Warning() const noexcept { return warning; }
private:
    bool warning;
};

[[noreturn]]
void ThrowException(const std::string& message);

[[noreturn]]
void ThrowException(const std::string& message, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);

[[noreturn]]
void ThrowException(const std::string& message, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, otava::symbols::Context* context);

[[noreturn]]
void ThrowException(const Exception& ex);

void PrintWarning(const Exception& ex, Context* context);

void PrintWarning(const std::string& message, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);

void PrintWarning(const std::string& message, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, otava::symbols::Context* context);

} // namespace otava::symbols
