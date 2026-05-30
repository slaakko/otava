// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.base;

import std;
import soul.lexer.lexeme;
import soul.lexer.token;
import soul.lexer.keyword;
import soul.lexer.classmap;
import soul.lexer.variables;
import soul.ast.slg;
import soul.ast.common;
import soul.ast.lexer_pos_pair;
import soul.ast.span;
import soul.lexer.parsing_log;

export namespace soul::lexer {

template<typename Char>
class LexerBase
{
public:
    using Self = LexerBase<Char>;
    LexerBase() noexcept {}
    virtual ~LexerBase() {}
    virtual void Retract() noexcept = 0;
    virtual std::int64_t GetKeywordToken(const Lexeme<Char>& lexeme) const noexcept = 0;
    virtual void EraseTail() = 0;
    virtual soul::lexer::Token<Char, Self>& CurrentToken() noexcept = 0;
    virtual const Lexeme<Char>& CurrentLexeme() const noexcept = 0;
    virtual Lexeme<Char>& CurrentLexeme() noexcept = 0;
    virtual soul::ast::Span GetSpan() const = 0;
    virtual soul::ast::Span GetSpan(std::int64_t pos) const = 0;
    virtual soul::ast::FullSpan GetFullSpan(std::int64_t pos) const = 0;
    virtual const soul::lexer::Token<Char, Self>& GetToken(std::int64_t pos) const = 0;
    virtual const std::string& FileName() const noexcept = 0;
    virtual int File() const noexcept = 0;
    virtual int Line() const noexcept = 0;
    virtual void SetLine(std::int32_t line_) noexcept = 0;
    virtual soul::lexer::ClassMap<Char>* GetClassMap() const noexcept = 0;
    virtual void SetClassMap(soul::lexer::ClassMap<Char>* classMap) noexcept = 0;
    virtual Variables* GetVariables() const noexcept = 0;
    virtual std::vector<int> GetLineStartIndeces() const = 0;
    virtual soul::ast::common::TokenCollection* GetTokenCollection() const noexcept = 0;
    virtual void SetTokenCollection(soul::ast::common::TokenCollection* tokenCollection_) noexcept = 0;
    virtual KeywordMap<Char>* GetKeywordMap() const noexcept = 0;
    virtual void SetKeywordMap(KeywordMap<Char>* keywordMap_) noexcept = 0;
    virtual std::map<std::int64_t, std::string>* GetRuleNameMapPtr() const noexcept = 0;
    virtual void SetRuleNameMapPtr(std::map<std::int64_t, std::string>* ruleNameMapPtr) noexcept = 0;
    virtual std::string ErrorLines(std::int64_t pos) const = 0;
    virtual std::int64_t GetPos() const noexcept = 0;
    virtual const Char* End() const noexcept = 0;
    virtual void SetPos(const Char* p) noexcept = 0;
    virtual void SetCurrentMatchEnd(const Char* end) noexcept = 0;
    virtual void Increment() noexcept = 0;
    virtual void MoveToEnd() = 0;
    virtual void BeginRecordedParse(const soul::ast::lexer::pos::pair::LexerPosPair& lexerPosPair) = 0;
    virtual void EndRecordedParse() = 0;
    virtual void PreprocessCurrentToken() = 0;
    virtual void Skip(bool skip_) noexcept = 0;
    virtual bool Skipping() const noexcept = 0;
    virtual void SetLog(ParsingLog* log) noexcept = 0;
};

} // namespace soul::lexer
