// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.re;

import std;
import util;
import soul.ast.slg;

export namespace soul::ast::re {

class Visitor;
class LexerContext;
using util::CodeFormatter;

const char32_t eps = '\0';

class Class;

enum class SymbolKind
{
    charSymbol, anySymbol, rangeSymbol, classSymbol
};

class Symbol
{
public:
    Symbol(SymbolKind kind_) noexcept;
    virtual ~Symbol();
    inline SymbolKind Kind() const noexcept { return kind; }
    virtual bool Match(char32_t c) noexcept = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual void Print(CodeFormatter& formatter) = 0;
    void SetName(const std::string& name_);
    inline const std::string& Name() const noexcept { return name; }
    inline bool Contained() const noexcept { return contained; }
    inline void SetContained() noexcept { if (!dontSetContained) contained = true; }
    inline void DontSetContained() noexcept { dontSetContained = true; }
private:
    SymbolKind kind;
    std::string name;
    bool contained;
    bool dontSetContained;
};

class CharSymbol : public Symbol
{
public:
    CharSymbol(char32_t chr_);
    bool Match(char32_t c) noexcept override;
    void Accept(Visitor& visitor) override;
    void Print(CodeFormatter& formatter) override;
    inline char32_t Chr() const noexcept { return chr; }
private:
    char32_t chr;
};

class Any : public Symbol
{
public:
    Any();
    bool Match(char32_t c) noexcept override;
    void Accept(Visitor& visitor) override;
    void Print(CodeFormatter& formatter) override;
};

class Range : public Symbol
{
public:
    Range() noexcept;
    Range(char32_t start_, char32_t end_);
    bool Match(char32_t c) noexcept override;
    void Accept(Visitor& visitor) override;
    void Print(CodeFormatter& formatter) override;
    inline bool IsEmpty() const noexcept { return start > end; }
    inline char32_t Start() const noexcept { return start; }
    inline char32_t End() const noexcept { return end; }
private:
    char32_t start;
    char32_t end;
};

inline bool operator==(const Range& left, const Range& right) noexcept
{
    return left.Start() == right.Start() && left.End() == right.End();
}

inline bool operator<(const Range& left, const Range& right) noexcept
{
    if (left.Start() < right.Start()) return true;
    if (left.Start() > right.Start()) return false;
    return left.End() < right.End();
}

bool Intersect(const Range& left, const Range& right) noexcept;

Range operator&(const Range& left, const Range& right) noexcept;

std::vector<Range> operator-(const Range& left, const Range& right);

std::vector<Range> operator~(const Range& that);

class Class : public Symbol
{
public:
    Class(int index_);
    bool Match(char32_t c) noexcept override;
    void Accept(Visitor& visitor) override;
    void Print(CodeFormatter& formatter) override;
    inline bool Inverse() const noexcept { return inverse; }
    inline void SetInverse() noexcept { inverse = true; }
    inline const std::vector<Symbol*>& Symbols() const noexcept { return symbols; }
    void AddSymbol(Symbol* symbol);
    void AddChar(char32_t c);
    inline int Index() const noexcept { return index; }
    void SetIndex(int index_);
    inline const std::vector<char32_t>& Chars() const noexcept { return chars; }
    Class* MakeCanonical(LexerContext& lexerContext);
    void MakeMinimal(LexerContext& lexerContext);
    void MakeInverse(LexerContext& lexerContext);
    Class* Clone();
    inline bool IsEmpty() const noexcept { return symbols.empty(); }
    inline const std::list<Range>& Ranges() const noexcept { return ranges; }
    inline std::list<Range>& Ranges() noexcept { return ranges; }
private:
    int index;
    bool inverse;
    std::vector<Symbol*> symbols;
    std::vector<char32_t> chars;
    std::list<Range> ranges;
};

bool operator==(const Class& left, const Class& right) noexcept;

bool Intersect(const Class& left, const Class& right) noexcept;

Class* MakeIntertersection(const Class& left, const Class& right, LexerContext& lexerContext);

Class* MakeDifference(const Class& left, const Class& right, LexerContext& lexerContext);

class NfaState;

class NfaEdge
{
public:
    NfaEdge(Symbol* symbol_, NfaState* next_) noexcept;
    inline Symbol* GetSymbol() const noexcept { return symbol; }
    inline NfaState* Next() const noexcept { return next; }
    void Print(CodeFormatter& formatter);
private:
    Symbol* symbol;
    NfaState* next;
};

class NfaState
{
public:
    NfaState(int id_, int ruleIndex_) noexcept;
    inline int Id() const noexcept { return id; }
    inline int RuleIndex() const noexcept { return ruleIndex; }
    inline void SetRuleIndex(int ruleIndex_) noexcept { ruleIndex = ruleIndex_; }
    inline bool Accept() const noexcept { return accept; }
    inline void SetAccept(bool accept_) noexcept { accept = accept_; }
    inline const std::vector<NfaEdge>& Edges() const noexcept { return edges; }
    void SetEdges(const std::vector<NfaEdge>& edges_);
    void AddEdge(const NfaEdge& edge);
    void AddPrev(NfaState* prev_);
    std::vector<NfaState*> Next(char32_t c) const;
    void Print(CodeFormatter& formatter);
private:
    int id;
    int ruleIndex;
    bool accept;
    std::vector<NfaEdge> edges;
    std::set<NfaState*> prev;
};

class Nfa
{
public:
    Nfa() noexcept;
    Nfa(NfaState* start_, NfaState* end_) noexcept;
    inline NfaState* Start() const noexcept { return start; }
    inline void SetStart(NfaState* start_) noexcept { start = start_; }
    inline NfaState* End() const noexcept { return end; }
    inline void SetEnd(NfaState* end_) noexcept { end = end_; }
    void Print(CodeFormatter& formatter);
private:
    NfaState* start;
    NfaState* end;
};

Nfa MakeNfa(LexerContext& lexerContext, Symbol* symbol);
Nfa Cat(const Nfa& left, const Nfa& right);
Nfa Alt(LexerContext& lexerContext, const Nfa& left, const Nfa& right);
Nfa Kleene(LexerContext& lexerContext, const Nfa& nfa);
Nfa Pos(LexerContext& lexerContext, const Nfa& nfa);
Nfa Opt(LexerContext& lexerContext, const Nfa& nfa);

class DfaState
{
public:
    DfaState(int id_, const std::vector<int>& nfaStateIds_);
    inline int Id() const noexcept { return id; }
    inline bool IsMarked() const noexcept { return marked; }
    inline void Mark() noexcept { marked = true; }
    inline bool Accept() const noexcept { return accept; }
    inline void SetAccept(bool accept_) noexcept { accept = accept_; }
    inline int RuleIndex() const noexcept { return ruleIndex; }
    inline void SetRuleIndex(int index) noexcept { ruleIndex = index; }
    inline const std::vector<int>& NfaStateIds() const noexcept { return nfaStateIds; }
    void AddNext(DfaState* next);
    DfaState* Next(int i) const noexcept;
    void Print(LexerContext& context, CodeFormatter& formatter);
private:
    int id;
    std::vector<int> nfaStateIds;
    std::vector<DfaState*> next;
    bool marked;
    bool accept;
    int ruleIndex;
};

class Dfa
{
public:
    void AddState(DfaState* state);
    inline const std::vector<DfaState*>& States() const noexcept { return states; }
    void Finalize(LexerContext& lexerContext);
    void Print(LexerContext& lexerContext, CodeFormatter& formatter);
private:
    std::vector<DfaState*> states;
};

std::vector<int> EpsilonClosure(LexerContext& lexerContext, const std::vector<int>& stateIds);
std::vector<int> EpsilonClosure(LexerContext& lexerContext, int nfaStateId);
std::vector<int> Move(LexerContext& lexerContext, const std::vector<int>& stateIds, char32_t c);
Dfa Compile(LexerContext& lexerContext, Nfa& nfa);

class ExprParser
{
public:
    virtual ~ExprParser();
    virtual Nfa Parse(const std::string& expression, LexerContext* lexerContext, const std::string& fileName, int line) = 0;
};

class LexerContext
{
public:
    LexerContext();
    ~LexerContext();
    inline const std::string& FileName() const noexcept { return fileName; }
    void SetFileName(const std::string& fileName_);
    void SetBase(const std::string& dir_);
    inline const std::string& Base() const noexcept { return dir; }
    NfaState* MakeNfaState();
    NfaState* GetNfaState(int id) const;
    inline const std::vector<NfaState*>& NfaStates() const noexcept { return nfaStates; }
    DfaState* MakeDfaState(const std::vector<int>& nfaStates);
    Symbol* MakeChar(char32_t c);
    inline Symbol* MakeAny() noexcept { return &any; }
    inline Symbol* MakeEpsilon() noexcept { return &epsilon; }
    Symbol* MakeRange(char32_t start, char32_t end);
    inline Class* MakeAsciiIdStart() noexcept { return asciiIdStart; }
    inline Class* MakeAsciiIdCont() noexcept { return asciiIdCont; }
    inline Class* MakeUnicodeIdStart() noexcept { return unicodeIdStart; }
    inline Class* MakeUnicodeIdCont() noexcept { return unicodeIdCont; }
    void AddToSymbols(Symbol* symbol);
    inline const std::vector<soul::ast::re::Symbol*>& Symbols() const noexcept { return symbols; }
    Class* MakeClass();
    void AddCanonicalClass(Class* cls);
    inline const std::vector<Class*>& CanonicalClasses() const noexcept { return canonicalClasses; }
    void AddToPartition(Class* cls);
    inline const std::vector<Class*>& Partition() const noexcept { return partition; }
    inline void SetCurrentExpression(soul::ast::slg::Expression* currentExpression_) noexcept { currentExpression = currentExpression_; }
    inline void SetExprParser(ExprParser* exprParser_) noexcept { exprParser = exprParser_; }
    inline ExprParser* GetExprParser() const noexcept { return exprParser; }
    Nfa MakeExpr(const std::string& id);
    inline soul::ast::slg::Tokens* GetTokens() const noexcept { return tokens; }
    inline void SetTokens(soul::ast::slg::Tokens* tokens_) noexcept { tokens = tokens_; }
    inline soul::ast::slg::Keywords* GetKeywords() const noexcept { return keywords; }
    inline void SetKeywords(soul::ast::slg::Keywords* keywords_) noexcept { keywords = keywords_; }
    inline soul::ast::slg::Expressions* GetExpressions() const noexcept { return expressions; }
    inline void SetExpressions(soul::ast::slg::Expressions* expressions_) noexcept { expressions = expressions_; }
    inline soul::ast::slg::Lexer* GetLexer() const noexcept { return lexer; }
    inline void SetLexer(soul::ast::slg::Lexer* lexer_) noexcept { lexer = lexer_; }
    int AddNfa(Nfa* nfa);
    Nfa* GetNfa(int index) const;
    inline int MasterNfaIndex() const noexcept { return masterNfaIndex; }
    inline void SetMasterNfaIndex(int masterNfaIndex_) noexcept { masterNfaIndex = masterNfaIndex_; }
    void SetDfa(Dfa&& dfa_);
    inline const Dfa& GetDfa() const noexcept { return dfa; }
    inline Dfa& GetDfa() noexcept { return dfa; }
    inline bool Verbose() const noexcept { return verbose; }
    inline void SetVerbose() noexcept { verbose = true; }
    inline bool Debug() const noexcept { return debug; }
    inline void SetDebug() noexcept { debug = true; }
private:
    std::string fileName;
    int nextNfaStateId;
    int nextDfaStateId;
    int ruleIndex;
    int classIndex;
    std::vector<NfaState*> nfaStates;
    std::map<int, NfaState*> nfaStateMap;
    std::vector<DfaState*> dfaStates;
    std::vector<Symbol*> symbols;
    std::vector<Class*> canonicalClasses;
    std::vector<Class*> partition;
    std::map<char32_t, Symbol*> charSymbols;
    std::map<Range, Symbol*> rangeSymbols;
    Any any;
    CharSymbol epsilon;
    Class* asciiIdStart;
    Class* asciiIdCont;
    Class* unicodeIdStart;
    Class* unicodeIdCont;
    soul::ast::slg::Tokens* tokens;
    soul::ast::slg::Keywords* keywords;
    soul::ast::slg::Expressions* expressions;
    soul::ast::slg::Lexer* lexer;
    soul::ast::slg::Expression* currentExpression;
    ExprParser* exprParser;
    std::vector<std::unique_ptr<Nfa>> nfas;
    std::map<int, Nfa*> nfaMap;
    int masterNfaIndex;
    Dfa dfa;
    bool verbose;
    bool debug;
    std::string dir;
};

class Visitor
{
public:
    virtual void Visit(CharSymbol& c) {}
    virtual void Visit(Any& a) {}
    virtual void Visit(Range& r) {}
    virtual void Visit(Class& c) {}
};

} // namespace soul::ast::re
