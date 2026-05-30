// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.error;

import otava.intermediate.context;

namespace otava::intermediate {

bool exceptionThrown = false;

bool ExceptionThrown()
{
    return exceptionThrown;
}

void SetExceptionThrown()
{
    exceptionThrown = true;
}

IntermediateError::IntermediateError(const std::string& message_) : std::runtime_error(message_.c_str()), message(message_)
{
}

[[noreturn]]
void Error(const std::string& message, const soul::ast::Span& span, IntermediateContext* context)
{
    SetExceptionThrown();
    if (span.IsValid())
    {
        soul::ast::LineColLen lineColLen;
        const std::vector<int>* lineStartIndeces = context->GetFileMap().LineStartIndeces(context->FileId());
        if (lineStartIndeces)
        {
            lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
        }
        if (lineColLen.IsValid())
        {
            throw IntermediateError(message + " at\n'" + context->FilePath() + "':" + std::to_string(lineColLen.line) + ":\n" +
                context->ErrorLines(lineColLen));
        }
        else if (context->GetLexer())
        {
            soul::lexer::LexerBase<char32_t>* lexer = context->GetLexer();
            throw IntermediateError(message + " at\n'" + context->FilePath() + "':" + std::to_string(lexer->Line()));
        }
        else
        {
            throw IntermediateError(message);
        }
    }
    else
    {
        throw IntermediateError(message);
    }
}

[[noreturn]]
void Error(const std::string& message, const soul::ast::Span& span, IntermediateContext* context, const soul::ast::Span& refSpan)
{
    SetExceptionThrown();
    if (span.IsValid() && refSpan.IsValid())
    {
        soul::ast::LineColLen lineColLen;
        soul::ast::LineColLen refLineColLen;
        const std::vector<int>* lineStartIndeces = context->GetFileMap().LineStartIndeces(context->FileId());
        if (lineStartIndeces)
        {
            lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
            refLineColLen = soul::ast::SpanToLineColLen(refSpan, *lineStartIndeces);
        }
        if (lineColLen.IsValid() && refLineColLen.IsValid())
        {
            throw IntermediateError(message + " at\n'" + context->FilePath() + "':" + std::to_string(lineColLen.line) + ":\n" +
                context->ErrorLines(lineColLen) + ": see reference line " + std::to_string(refLineColLen.line) + ":\n" +
                context->ErrorLines(refLineColLen));
        }
        else if (context->GetLexer())
        {
            soul::lexer::LexerBase<char32_t>* lexer = context->GetLexer();
            throw IntermediateError(message + " at\n'" + context->FilePath() + "':" + std::to_string(lexer->Line()));
        }
        else
        {
            throw IntermediateError(message + " at\n'" + context->FilePath() + "'");
        }
    }
    else
    {
        throw IntermediateError(message);
    }
}

void Warning(const std::string& message, const soul::ast::Span& span, IntermediateContext* context)
{
    if (span.IsValid())
    {
        soul::ast::LineColLen lineColLen;
        const std::vector<int>* lineStartIndeces = context->GetFileMap().LineStartIndeces(context->FileId());
        if (lineStartIndeces)
        {
            lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
        }
        if (lineColLen.IsValid())
        {
            std::cout << "warning: " << message << " at\n'" + context->FilePath() << "':" << std::to_string(lineColLen.line) << ":\n" <<
                context->ErrorLines(lineColLen) << "\n";
        }
        else
        {
            std::cout << "warning: " << message << "\n";
        }
    }
    else
    {
        std::cout << "warning: " << message << "\n";
    }
}

} // otava::intermediate
