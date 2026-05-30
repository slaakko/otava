// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.rex.match;

import std;
import soul.rex.nfa;
import soul.rex.context;
import util;

export namespace soul::rex {

bool FilePatternMatch(const std::u32string& filePath, const std::u32string& filePattern);

bool FilePatternMatch(const std::string& filePath, const std::string& filePattern);

bool PatternMatch(const std::u32string& str, const std::u32string& regularExpressionPattern);

bool PatternMatch(const std::string& str, const std::string& regularExpressionPattern);

bool PatternMatch(const std::u32string& str, soul::rex::nfa::Nfa& nfa);

bool PatternMatch(const std::string& str, soul::rex::nfa::Nfa& nfa);

soul::rex::nfa::Nfa CompileRegularExpressionPattern(soul::rex::context::Context& context, const std::u32string& regularExpressionPattern);

soul::rex::nfa::Nfa CompileRegularExpressionPattern(soul::rex::context::Context& context, const std::string& regularExpressionPattern);

soul::rex::nfa::Nfa CompileFilePattern(soul::rex::context::Context& context, const std::u32string& filePattern);

soul::rex::nfa::Nfa CompileFilePattern(soul::rex::context::Context& context, const std::string& filePattern);

bool FilePatternMatch(const std::string& lexerModuleFileName, const std::u32string& filePath, const std::u32string& filePattern);

bool FilePatternMatch(const std::string& lexerModuleFileName, const std::string& filePath, const std::string& filePattern);

bool PatternMatch(const std::string& lexerModuleFileName, const std::u32string& str, const std::u32string& regularExpressionPattern);

bool PatternMatch(const std::string& lexerModuleFileName, const std::string& str, const std::string& regularExpressionPattern);

soul::rex::nfa::Nfa CompileRegularExpressionPattern(const std::string& lexerModuleFileName, util::ResourceFlags resourceFlags,
    soul::rex::context::Context& context, const std::u32string& regularExpressionPattern);

soul::rex::nfa::Nfa CompileRegularExpressionPattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::u32string& regularExpressionPattern);

soul::rex::nfa::Nfa CompileRegularExpressionPattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::string& regularExpressionPattern);

soul::rex::nfa::Nfa CompileFilePattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::u32string& filePattern);

soul::rex::nfa::Nfa CompileFilePattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::string& filePattern);

} // namespace soul::rex