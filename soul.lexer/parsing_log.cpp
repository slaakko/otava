// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.parsing.log;

namespace soul::lexer {

ParsingLog::ParsingLog() noexcept : maxLineLength(80)
{
}

ParsingLog::ParsingLog(int maxLineLength_) noexcept : maxLineLength(maxLineLength_)
{
}

ParsingLog::~ParsingLog()
{
}

int ParsingLog::MaxLineLength() const noexcept
{
    return maxLineLength;
}

} // namespace soul::lexer
