// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.exception;

import otava.symbols.context;
import soul.lexer.file_map;
import util.unicode;

namespace otava::symbols {

bool exceptionThrown = false;

bool ExceptionThrown()
{
    return exceptionThrown;
}

void SetExceptionThrown()
{
    exceptionThrown = true;
}

int LineNumber(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (fullSpan.IsValid())
    {
        soul::lexer::FileMap* fileMap = context->GetFileMap();
        if (fileMap)
        {
            const std::vector<int>* lineStartIndeces = fileMap->LineStartIndeces(fullSpan.fileIndex);
            if (lineStartIndeces)
            {
                soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(fullSpan.span, *lineStartIndeces);
                return lineColLen.line;
            }
        }
    }
    return 0;
}

std::string ErrorLine(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (fullSpan.IsValid())
    {
        soul::lexer::FileMap* fileMap = context->GetFileMap();
        if (fileMap)
        {
            const std::vector<int>* lineStartIndeces = fileMap->LineStartIndeces(fullSpan.fileIndex);
            if (lineStartIndeces)
            {
                soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(fullSpan.span, *lineStartIndeces);
                std::string errorLine = util::ToUtf8(fileMap->GetFileLine(fullSpan.fileIndex, lineColLen.line));
                std::string caretLine = std::string(lineColLen.col - 1, ' ') + std::string(lineColLen.len, '^');
                return ":\n" + errorLine + "\n" + caretLine;
            }
        }
    }
    return std::string();
}

std::string SourceFilePath(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (fullSpan.IsValid())
    {
        soul::lexer::FileMap* fileMap = context->GetFileMap();
        if (fileMap)
        {
            return fileMap->GetFilePath(fullSpan.fileIndex);
        }
    }
    return std::string();
}

std::string ReferenceInfo(const soul::ast::FullSpan& refSpan, Context* context)
{
    if (refSpan.IsValid())
    {
        soul::lexer::FileMap* fileMap = context->GetFileMap();
        if (fileMap)
        {
            const std::vector<int>* lineStartIndeces = fileMap->LineStartIndeces(refSpan.fileIndex);
            if (lineStartIndeces)
            {
                soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(refSpan.span, *lineStartIndeces);
                std::string message = "\nSee reference file " + SourceFilePath(refSpan, context) + ", line " +
                    std::to_string(lineColLen.line) + ErrorLine(refSpan, context);
                return message;
            }
        }
    }
    return std::string();
}

Exception::Exception(const std::string& message_) : std::runtime_error(message_), warning(false)
{
}

Exception::Exception(const std::string& message_, const soul::ast::FullSpan& fullSpan, Context* context) : 
    Exception(message_, fullSpan, soul::ast::FullSpan(), context)
{
}

Exception::Exception(const std::string& message_, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, Context* context) : 
    Exception("error: ", message_, fullSpan, refSpan, context)
{
}

Exception::Exception(const std::string& title, const std::string& message_, const soul::ast::FullSpan& fullSpan, Context* context) :
    Exception(title, message_, fullSpan, soul::ast::FullSpan(), context)
{
}

Exception::Exception(const std::string& title_, const std::string& message_, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, Context* context) : 
    std::runtime_error(title_ + message_ + ", file '" + SourceFilePath(fullSpan, context) +
        "', line " + std::to_string(LineNumber(fullSpan, context)) + ErrorLine(fullSpan, context) + ReferenceInfo(refSpan, context)), warning(false)
{
}

[[noreturn]]
void ThrowException(const std::string& message)
{
    SetExceptionThrown();
    Exception exception(message);
    throw exception;
}

[[noreturn]]
void ThrowException(const std::string& message, const soul::ast::FullSpan& fullSpan, Context* context)
{
    exceptionThrown = true;
    throw Exception(message, fullSpan, soul::ast::FullSpan(), context);
}

[[noreturn]]
void ThrowException(const std::string& message, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, Context* context)
{
    exceptionThrown = true;
    throw Exception(message, fullSpan, refSpan, context);
}

[[noreturn]]
void ThrowException(const Exception& ex)
{
    exceptionThrown = true;
    throw ex;
}

void PrintWarning(const Exception& ex, Context* context)
{
    if (!context->GetFlag(ContextFlags::noWarnings))
    {
        std::cout << ex.what() << std::endl;
    }
}

void PrintWarning(const std::string& message, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    PrintWarning(message, fullSpan, soul::ast::FullSpan(), context);
}

void PrintWarning(const std::string& message, const soul::ast::FullSpan& fullSpan, const soul::ast::FullSpan& refSpan, otava::symbols::Context* context)
{
    Exception exception("warning: ", message, fullSpan, refSpan, context);
    exception.SetWarning();
    PrintWarning(exception, context);
}
} // namespace otava::symbols
