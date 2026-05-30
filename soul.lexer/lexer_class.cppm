// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.cls;

import std;
import soul.ast.source_pos;
import util.unicode;
import soul.ast.slg;
import soul.ast.lexer_pos_pair;
import soul.ast.span;
import soul.lexer.token_parser;
import soul.lexer.base;
import soul.lexer.concepts;
import soul.lexer.error;
import soul.lexer.parsing_log;
import soul.lexer.token;

export namespace soul::lexer {

enum class LexerFlags : std::int32_t
{
    none = 0, synchronize = 1 << 0, synchronized = 1 << 1, synchronizedAtLeastOnce = 1 << 2, cursorSeen = 1 << 3, recordedParse = 1 << 4, farthestError = 1 << 5
};

constexpr LexerFlags operator|(LexerFlags left, LexerFlags right) noexcept
{
    return static_cast<LexerFlags>(static_cast<std::int32_t>(left) | static_cast<std::int32_t>(right));
}

constexpr LexerFlags operator&(LexerFlags left, LexerFlags right) noexcept
{
    return static_cast<LexerFlags>(static_cast<std::int32_t>(left) & static_cast<std::int32_t>(right));
}

constexpr LexerFlags operator~(LexerFlags flag) noexcept
{
    return static_cast<LexerFlags>(~static_cast<std::int32_t>(flag));
}

bool parsing_error_thrown = false;

template<class Char, class LexerBaseT>
struct LexerState
{
    LexerState(LexerBaseT* lexer) noexcept : token(lexer), line(0), lexeme(), pos(), tokens(), flags(), recordedPosPair(), farthestPos(), currentPos()
    {
    }
    soul::lexer::Token<Char, LexerBaseT> token;
    int line;
    Lexeme<Char> lexeme;
    const Char* pos;
    std::vector<soul::lexer::Token<Char, LexerBaseT>> tokens;
    LexerFlags flags;
    soul::ast::lexer::pos::pair::LexerPosPair recordedPosPair;
    std::int64_t farthestPos;
    std::vector<std::int64_t> ruleContext;
    std::vector<std::int64_t> farthestRuleContext;
    std::int64_t currentPos;
};

template<typename Stack>
    requires RuleStack<Stack>
struct RuleGuard
{
    RuleGuard(Stack& stack_, std::int64_t ruleId) noexcept : stack(stack_)
    {
        stack.PushRule(ruleId);
    }
    ~RuleGuard()
    {
        stack.PopRule();
    }
    Stack& stack;
};

template<typename Char>
const Char* LineStart(const Char* start, const Char* p) noexcept
{
    while (p != start && *p != '\n' && *p != '\r')
    {
        --p;
    }
    if (p != start)
    {
        ++p;
    }
    return p;
}

template<typename Char>
const Char* LineEnd(const Char* end, const Char* p) noexcept
{
    while (p != end && *p != '\n' && *p != '\r')
    {
        ++p;
    }
    return p;
}

template<typename Machine, typename Char>
    requires FiniteStateMachine<Machine, Char>
class Lexer : public LexerBase<Char>
{
public:
    using LexerType = Lexer<Machine, Char>;
    using CharType = Char;
    using TokenType = soul::lexer::Token<Char, LexerBase<Char>>;
    using TokenLineType = soul::lexer::TokenLine<Char, LexerBase<Char>>;
    using VariableClassType = Machine::Variables;
    using PPHook = void (*)(LexerType* lexer, TokenType* token);

    Lexer(const Char* start_, const Char* end_, const std::string& fileName_) :
        flags(LexerFlags::none),
        file(-1),
        line(1),
        current(tokens.end()),
        token(this),
        separatorChar('\0'),
        start(start_),
        end(end_),
        pos(start),
        fileName(fileName_),
        countLines(true),
        classMap(nullptr),
        tokenCollection(nullptr),
        keywordMap(nullptr),
        ruleNameMapPtr(nullptr),
        farthestPos(GetPos()),
        log(nullptr),
        vars(),
        state(this),
        ppHook(nullptr),
        skip(false),
        commentTokenId(-1)
    {
        ComputeLineStarts();
    }
    inline std::int64_t operator*() const noexcept
    {
        return current->id;
    }
    void operator++()
    {
        if (GetFlag(LexerFlags::recordedParse))
        {
            // precondition: !tokens.empty() && tokens.back().id == END_TOKEN
            std::int64_t currentPos = GetPos();
            if (currentPos == recordedPosPair.end)
            {
                current = tokens.end() - 1; // set current to last token whose id is END_TOKEN
                auto& token = tokens.back();
                pos = token.match.end;
            }
            else
            {
                ++current;
            }
            line = current->line;
        }
        else
        {
            if (current != tokens.end())
            {
                ++current;
            }
            if (current == tokens.end())
            {
                NextToken();
            }
            else
            {
                line = current->line;
            }
            std::int64_t p = GetPos();
            if (p > farthestPos)
            {
                farthestPos = p;
                farthestRuleContext = ruleContext;
            }
        }
    }
    std::int64_t GetPos() const noexcept override
    {
        std::int32_t p = static_cast<std::int32_t>(current - tokens.begin());
        return (static_cast<std::int64_t>(line) << 32) | static_cast<std::int64_t>(p);
    }
    void SetPos(std::int64_t pos) noexcept
    {
        current = tokens.begin() + static_cast<std::int32_t>(pos);
        line = static_cast<std::int32_t>(pos >> 32);
    }
    soul::ast::Span GetSpan() const override
    {
        return GetSpan(GetPos());
    }
    soul::ast::Span GetSpan(std::int64_t pos) const override
    {
        const auto& token = GetToken(pos);
        return soul::ast::Span(static_cast<int>(token.match.begin - start), token.match.Length());
    }
    soul::ast::FullSpan GetFullSpan(std::int64_t pos) const override
    {
        return soul::ast::FullSpan(file, GetSpan(pos));
    }
    const TokenType& GetToken(std::int64_t pos) const override
    {
        std::int32_t tokenIndex = static_cast<std::int32_t>(pos);
        if (tokenIndex >= 0 && tokenIndex < tokens.size())
        {
            return tokens[tokenIndex];
        }
        else
        {
            throw std::runtime_error("invalid token index");
        }
    }
    const std::string& FileName() const noexcept override
    {
        return fileName;
    }
    int File() const noexcept override
    {
        return file;
    }
    void SetFile(int file_) noexcept
    {
        file = file_;
    }
    int Line() const noexcept override
    {
        return line;
    }
    void SetLine(std::int32_t line_) noexcept override
    {
        line = line_;
    }
    soul::lexer::ClassMap<Char>* GetClassMap() const noexcept override
    {
        return classMap;
    }
    void SetClassMap(soul::lexer::ClassMap<Char>* classMap_) noexcept override
    {
        classMap = classMap_;
    }
    soul::ast::common::TokenCollection* GetTokenCollection() const noexcept override
    {
        return tokenCollection;
    }
    void SetTokenCollection(soul::ast::common::TokenCollection* tokenCollection_) noexcept override
    {
        tokenCollection = tokenCollection_;
    }
    KeywordMap<Char>* GetKeywordMap() const noexcept override
    {
        return keywordMap;
    }
    void SetKeywordMap(KeywordMap<Char>* keywordMap_) noexcept override
    {
        keywordMap = keywordMap_;
    }
    std::map<std::int64_t, std::string>* GetRuleNameMapPtr() const noexcept override
    {
        return ruleNameMapPtr;
    }
    void SetRuleNameMapPtr(std::map<std::int64_t, std::string>* ruleNameMapPtr_) noexcept override
    {
        ruleNameMapPtr = ruleNameMapPtr_;
    }
    inline LexerFlags Flags() const noexcept { return flags; }
    inline bool GetFlag(LexerFlags flag) const noexcept { return (flags & flag) != LexerFlags::none; }
    inline void SetFlag(LexerFlags flag) noexcept { flags = flags | flag; }
    inline void ResetFlag(LexerFlags flag) noexcept { flags = flags & ~flag; }
    inline void PushRule(std::int64_t ruleId) 
    {
        ruleContext.push_back(ruleId);
    }
    inline void PopRule() 
    {
        ruleContext.pop_back();
    }
    std::int64_t GetKeywordToken(const Lexeme<Char>& lexeme) const noexcept override
    {
        if (keywordMap)
        {
            return keywordMap->GetKeywordToken(lexeme);
        }
        else
        {
            return soul::lexer::INVALID_TOKEN;
        }
    }
    void Retract() noexcept override
    {
        token.match.end = pos;
    }
    soul::lexer::Token<Char, LexerBase<Char>>& CurrentToken() noexcept override
    {
        return token;
    }
    void EraseTail() override
    {
        tokens.erase(current + 1, tokens.end());
    }
    const Lexeme<Char>& CurrentLexeme() const noexcept override
    {
        return lexeme;
    }
    Lexeme<Char>& CurrentLexeme() noexcept override
    {
        return lexeme;
    }
    std::string GetTokenName(std::int64_t tokenID) const
    {
        soul::ast::common::TokenCollection* tokenCollection = GetTokenCollection();
        auto t = tokenCollection->GetToken(tokenID);
        if (t)
        {
            return t->Name();
        }
        else
        {
            return "<unknown token>";
        }
    }
    std::string GetTokenInfo(std::int64_t tokenID) const
    {
        soul::ast::common::TokenCollection* tokenCollection = GetTokenCollection();
        auto t = tokenCollection->GetToken(tokenID);
        if (t)
        {
            return t->Info();
        }
        else
        {
            return "<unknown token>";
        }
    }
    inline int GetLine(std::int64_t pos) const noexcept
    {
        return pos >> 32;
    }
    soul::ast::SourcePos GetSourcePos(std::int64_t pos) const noexcept
    {
        const Char* s = start;
        int line = GetLine(pos);
        if (line < lineStarts.size())
        {
            s = lineStarts[line];
        }
        auto token = GetToken(pos);
        int col = static_cast<int>(token.match.begin - s + 1);
        return soul::ast::SourcePos(pos, file, line, col);
    }
    std::string ErrorLines(std::int64_t pos) const override
    {
        auto token = GetToken(pos);
        std::basic_string<Char> lines;
        const Char* lineStart = LineStart(start, token.match.begin);
        const Char* lineEnd = LineEnd(end, token.match.end);
        if (token.match.begin > lineStart)
        {
            lines.append(std::basic_string<Char>(lineStart, token.match.begin));
        }
        lines.append(token.match.ToString());
        if (lineEnd > token.match.end)
        {
            lines.append(std::basic_string<Char>(token.match.end, lineEnd));
        }
        lines.append(1, '\n');
        if (token.match.begin > lineStart)
        {
            lines.append(token.match.begin - lineStart, ' ');
        }
        lines.append(std::max(static_cast<std::int64_t>(1), token.match.end - token.match.begin), '^');
        if (lineEnd > token.match.end)
        {
            lines.append(lineEnd - token.match.end, ' ');
        }
        return util::ToUtf8(lines);
    }
    void ThrowExpectationFailure(std::int64_t pos, const std::string& name)
    {
        soul::ast::SourcePos sourcePos = GetSourcePos(pos);
        std::string parserStateStr = GetParserStateStr();
        throw ParsingException("parsing error at '" + fileName + ":" + std::to_string(sourcePos.line) + "': " + name + " expected:\n" + ErrorLines(pos) + parserStateStr,
            fileName, sourcePos);
    }
    std::string GetParserStateStr() const
    {
        std::string parserStateStr;
        int n = farthestRuleContext.size();
        if (ruleNameMapPtr && n > 0)
        {
            parserStateStr.append("\nParser state:\n");
            for (int i = 0; i < n; ++i)
            {
                std::int64_t ruleId = farthestRuleContext[i];
                auto it = (*ruleNameMapPtr).find(ruleId);
                if (it != (*ruleNameMapPtr).cend())
                {
                    std::string ruleName = it->second;
                    parserStateStr.append(ruleName.append("\n"));
                }
            }
        }
        return parserStateStr;
    }
    std::string GetError(std::int64_t pos) const
    {
        std::string parserStateStr = GetParserStateStr();
        return "parsing error at '" + fileName + ":" + std::to_string(GetLine(pos)) + "':\n" + ErrorLines(pos) + parserStateStr;
    }
    void ThrowFarthestError()
    {
        std::cout.flush();
        soul::ast::SourcePos sourcePos = GetSourcePos(farthestPos);
        parsing_error_thrown = true;
        throw ParsingException(GetError(farthestPos), fileName, sourcePos);
    }
    void SetLog(ParsingLog* log_) noexcept override
    {
        log = log_;
    }
    inline ParsingLog* Log() const noexcept
    {
        return log;
    }
    std::string RestOfLine(int maxLineLength) const
    {
        std::string restOfLine(util::ToUtf8(current->match.ToString()) + ToString(current->match.end, pos) + ToString(pos, LineEnd(end, pos)));
        if (maxLineLength != 0)
        {
            restOfLine = restOfLine.substr(0, maxLineLength);
        }
        return restOfLine;
    }
    std::vector<int> GetLineStartIndeces() const override
    {
        std::vector<int> lineStartIndeces;
        for (int i = 0; i < lineStarts.size(); ++i)
        {
            lineStartIndeces.push_back(static_cast<int>(lineStarts[i] - start));
        }
        return lineStartIndeces;
    }
    soul::lexer::Variables* GetVariables() const noexcept override
    {
        return const_cast<soul::lexer::Variables*>(static_cast<const soul::lexer::Variables*>(&vars));
    }
    inline const Char* Start() const noexcept
    {
        return start;
    }
    inline const Char* Pos() const noexcept
    {
        return pos;
    }
    void SetPos(const Char* pos_) noexcept override
    {
        pos = pos_;
    }
    const Char* End() const noexcept override
    {
        return end;
    }
    inline const std::vector<soul::lexer::Token<Char, LexerBase<Char>>>& Tokens() const noexcept
    {
        return tokens;
    }
    inline std::vector<soul::lexer::Token<Char, LexerBase<Char>>> ReleaseTokens() noexcept
    {
        return std::move(tokens);
    }
    void SetCurrentMatchEnd(const Char* end) noexcept override
    {
        current->match.end = end;
    }
    void Increment() noexcept override
    {
        ++*this;
    }
    void MoveToEnd() override
    {
        TokenType endToken(END_TOKEN, this);
        endToken.match.begin = end;
        endToken.match.end = end;
        tokens.push_back(endToken);
        current = tokens.end() - 1;
    }
    void BeginRecordedParse(const soul::ast::lexer::pos::pair::LexerPosPair& lexerPosPair) override
    {
        PushState();
        if (tokens.empty() || tokens.back().id != END_TOKEN)
        {
            TokenType endToken(END_TOKEN, this);
            endToken.match.begin = end;
            endToken.match.end = end;
            tokens.push_back(endToken);
        }
        recordedPosPair = lexerPosPair;
        SetPos(recordedPosPair.start);
        SetFlag(LexerFlags::recordedParse);
    }
    void EndRecordedParse() override
    {
        PopState();
    }
    inline void SetPPHook(PPHook ppHook_) noexcept
    {
        ppHook = ppHook_;
    }
    void PreprocessCurrentToken() override
    {
        if (ppHook)
        {
            ppHook(this, &token); 
        }
    }
    inline void PushState()
    {
        stateStack.push(GetState());
    }
    inline void PopState()
    {
        SetState(stateStack.top());
        stateStack.pop();
    }
    LexerState<Char, LexerBase<Char>> GetState()
    {
        LexerState<Char, LexerBase<Char>> state(this);
        state.token = token;
        state.line = line;
        state.lexeme = lexeme;
        state.pos = pos;
        state.tokens = tokens;
        state.flags = flags;
        state.recordedPosPair = recordedPosPair;
        state.farthestPos = farthestPos;
        state.ruleContext = ruleContext;
        state.farthestRuleContext = farthestRuleContext;
        state.currentPos = GetPos();
        return state;
    }
    void SetState(const LexerState<Char, LexerBase<Char>>& state)
    {
        token = state.token;
        line = state.line;
        lexeme = state.lexeme;
        pos = state.pos;
        tokens = state.tokens;
        flags = state.flags;
        recordedPosPair = state.recordedPosPair;
        farthestPos = state.farthestPos;
        ruleContext = state.ruleContext;
        farthestRuleContext = state.farthestRuleContext;
        SetPos(state.currentPos);
    }
    void Skip(bool skip_) noexcept override
    {
        skip = skip_;
        current = tokens.end();
    }
    bool Skipping() const noexcept override
    {
        return skip;
    }
    inline void SetCommentTokenId(std::int64_t commentTokenId_) noexcept
    {
        commentTokenId = commentTokenId_;
    }
    inline void SetBlockCommentStates(const std::set<int>& blockCommentStates_) noexcept
    {
        blockCommentStates = blockCommentStates_;
    }
    inline const std::set<int>& BlockCommentStates() const noexcept
    {
        return blockCommentStates;
    }
    TokenLineType TokenizeLine(const std::basic_string<Char>& line, int lineNumber, int startState)
    {
        TokenLineType tokenLine;
        pos = line.c_str();
        end = line.c_str() + line.length();
        tokenLine.startState = startState;
        lexeme.begin = pos;
        lexeme.end = end;
        token.match = lexeme;
        token.id = soul::lexer::INVALID_TOKEN;
        token.line = lineNumber;
        int state = startState;
        int prevState = 0;
        int prevPrevState = 0;
        bool cont = false;
        while (pos != end)
        {
            Char c = *pos;
            if (state == 0)
            {
                lexeme.begin = pos;
                token.id = soul::lexer::INVALID_TOKEN;
                token.line = lineNumber;
            }
            lexeme.end = pos + 1;
            prevPrevState = prevState;
            prevState = state;
            state = Machine::NextState(state, c, *static_cast<LexerBase<Char>*>(this));
            if (state == -1)
            {
                if (prevState == 0)
                {
                    break;
                }
                state = 0;
                pos = token.match.end;
                tokenLine.tokens.push_back(token);
                if (pos + 1 < end && *pos == '\"' && *(pos + 1) == '\\' && prevPrevState == 13 && prevState == 71)
                {
                    TokenType tok(nullptr);
                    tok.match.begin = pos;
                    tok.match.end = pos + 2;
                    tokenLine.tokens.push_back(tok);
                    pos += 2;
                }
                lexeme.begin = lexeme.end;
            }
            else
            {
                ++pos;
            }
        }
        if (state != 0 && state != -1)
        {
            state = Machine::NextState(state, '\r', *static_cast<LexerBase<Char>*>(this));
        }
        if (state != 0 && state != -1)
        {
            state = Machine::NextState(state, '\n', *static_cast<LexerBase<Char>*>(this));
        }
        if (state != 0 && state != -1)
        {
            if (blockCommentStates.find(state) != blockCommentStates.cend())
            {
                token.id = commentTokenId;
                token.match.end = end;
                tokenLine.tokens.push_back(token);
                tokenLine.endState = state;
                return tokenLine;
            }
        }
        if (lexeme.begin != lexeme.end)
        {
            token.match = lexeme;
            tokenLine.tokens.push_back(token);
        }
        if (state == -1)
        {
            state = 0;
        }
        tokenLine.endState = state;
        return tokenLine;
    }
private:
    void NextToken()
    {
        int state = 0;
        while (true)
        {
            Char c = separatorChar;
            if (pos != end)
            {
                c = *pos;
            }
            else if (c == '\0')
            {
                break;
            }
            if (state == 0)
            {
                lexeme.begin = pos;
                token.id = soul::lexer::INVALID_TOKEN;
                token.line = line;
            }
            if (pos == end)
            {
                lexeme.end = end;
            }
            else
            {
                lexeme.end = pos + 1;
            }
            state = Machine::NextState(state, c, *static_cast<LexerBase<Char>*>(this));
            if (state == -1)
            {
                if (token.id == soul::lexer::CONTINUE_TOKEN)
                {
                    if (pos == end)
                    {
                        break;
                    }
                    else
                    {
                        pos = token.match.end;
                    }
                    state = 0;
                    continue;
                }
                else if (token.id == soul::lexer::INVALID_TOKEN)
                {
                    if (pos == end)
                    {
                        break;
                    }
                    else
                    {
                        throw std::runtime_error("soul::lexer::Lexer::NextToken(): error: invalid character '" + 
                            util::ToUtf8(std::u32string(1, c)) + "' in file '" + fileName + "' at line " + std::to_string(line));
                    }
                }
                else
                {
                    if (!skip)
                    {
                        tokens.push_back(token);
                        current = tokens.end() - 1;
                        pos = token.match.end;
                        return;
                    }
                    else
                    {
                        pos = token.match.end;
                        return;
                    }
                }
            }
            if (c == '\n' && countLines)
            {
                ++line;
            }
            ++pos;
        }
        token.id = soul::lexer::INVALID_TOKEN;
        state = Machine::NextState(state, '\0', *this);
        std::int64_t p = -1;
        if (token.id != soul::lexer::INVALID_TOKEN && token.id != soul::lexer::CONTINUE_TOKEN)
        {
            tokens.push_back(token);
            current = tokens.end() - 1;
            p = GetPos();
        }
        TokenType endToken(soul::lexer::END_TOKEN, this);
        endToken.match.begin = end;
        endToken.match.end = end;
        tokens.push_back(endToken);
        if (p == -1)
        {
            current = tokens.end() - 1;
            p = GetPos();
        }
        SetPos(p);
    }
    void ComputeLineStarts()
    {
        lineStarts.push_back(pos);
        const Char* p = pos;
        bool startOfLine = true;
        while (p != end)
        {
            if (startOfLine)
            {
                lineStarts.push_back(p);
            }
            startOfLine = *p == '\n';
            ++p;
        }
        lineStarts.push_back(end);
    }
    LexerFlags flags;
    soul::ast::lexer::pos::pair::LexerPosPair recordedPosPair;
    Char separatorChar;
    std::vector<soul::lexer::Token<Char, LexerBase<Char>>> tokens;
    std::vector<soul::lexer::Token<Char, LexerBase<Char>>>::iterator current;
    soul::lexer::Token<Char, LexerBase<Char>> token;
    Lexeme<Char> lexeme;
    std::int32_t file;
    std::int32_t line;
    const Char* start;
    const Char* end;
    const Char* pos;
    std::string fileName;
    bool countLines;
    soul::lexer::ClassMap<Char>* classMap;
    soul::ast::common::TokenCollection* tokenCollection;
    KeywordMap<Char>* keywordMap;
    std::int64_t farthestPos;
    std::vector<std::int64_t> ruleContext;
    std::vector<std::int64_t> farthestRuleContext;
    std::vector<const Char*> lineStarts;
    std::map<std::int64_t, std::string>* ruleNameMapPtr;
    ParsingLog* log;
    Machine::Variables vars;
    LexerState<Char, LexerBase<Char>> state;
    std::stack<LexerState<Char, LexerBase<Char>>> stateStack;
    PPHook ppHook;
    bool skip;
    std::int64_t commentTokenId;
    std::set<int> blockCommentStates;
};

inline std::string GetEndTokenInfo()
{
    return "end-of-file";
}

template<typename Lexer>
void WriteBeginRuleToLog(Lexer& lexer, const std::string& ruleName)
{
    lexer.Log()->WriteBeginRule(ruleName);
    lexer.Log()->IncIndent();
    lexer.Log()->WriteTry(lexer.RestOfLine(lexer.Log()->MaxLineLength()));
    lexer.Log()->IncIndent();
}

template<typename Lexer>
void WriteSuccessToLog(Lexer& lexer, std::int64_t matchPos, const std::string& ruleName)
{
    lexer.Log()->DecIndent();
    lexer.Log()->WriteSuccess(util::ToUtf8(lexer.GetToken(matchPos).ToString()));
    lexer.Log()->DecIndent();
    lexer.Log()->WriteEndRule(ruleName);
}

template<typename Lexer>
void WriteFailureToLog(Lexer& lexer, const std::string& ruleName)
{
    lexer.Log()->DecIndent();
    lexer.Log()->WriteFail();
    lexer.Log()->DecIndent();
    lexer.Log()->WriteEndRule(ruleName);
}

} // namespace soul::lexer
