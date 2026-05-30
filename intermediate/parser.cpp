// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.main.parser;

import otava.intermediate.context;
import otava.intermediate.lexer;
import otava.intermediate.parser.rules;
import otava.intermediate.parser;
import util.file_stream;
import util.unicode;

namespace otava::intermediate {

void Parse(const std::string& filePath, IntermediateContext& context, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << filePath << "\n";
    }
    int fileIndex = context.GetFileMap().MapFile(filePath);
    context.SetFileId(fileIndex);
    std::u32string content;
    try
    {
        content = util::ToUtf32(util::ReadFile(filePath));
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + filePath);
    }
    auto lexer = otava::intermediate::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), filePath);
    lexer.SetFile(fileIndex);
    soul::lexer::LexerBase<char32_t>* lexerBase = &lexer;
    context.SetLexer(lexerBase);
    using LexerType = decltype(lexer);
    lexer.SetRuleNameMapPtr(otava::intermediate::parser::rules::GetRuleNameMapPtr());
    otava::intermediate::parser::IntermediateParser<LexerType>::Parse(lexer, &context);
    context.GetTypes().ResolveComments();
    context.GetFileMap().AddFileContent(fileIndex, std::move(content), lexer.GetLineStartIndeces());
}

} // otava::intermediate
