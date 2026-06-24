// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.pp.state;

import std;
import otava.lexer;
import soul.lexer;

export namespace otava::pp::state {

using Lexer = soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>;

class Section
{
public:
    Section(Section* parent_);
    Section* Parent() const { return parent; }
    bool Skip() const { return skip; }
    void SetSkip(bool skip_) { skip = skip_; }
    bool Processed() const { return processed; }
    void SetProcessed(bool processed_) { processed = processed_; }
private:
    Section* parent;
    bool skip;
    bool processed;
};

class State
{
public:
    State(Lexer* lexer_);
    void If(bool value);
    void Elif(bool value);
    void Ifdef(const std::string& symbol);
    void Ifndef(const std::string& symbol);
    void Else();
    void Endif();
    void Define(const std::string& symbol, std::int64_t value);
    void Undef(const std::string& symbol);
    bool Defined(const std::string& symbol) const;
    void Include();
    std::int64_t GetSymbolValue(const std::string& symbol) const;
private:
    void PushSection();
    void PopSection();
    void SkipSection();
    Lexer* lxr;
    std::map<std::string, std::int64_t> symbolMap;
    std::unique_ptr<Section> currentSection;
    std::stack<std::unique_ptr<Section>> sectionStack;
};

class LexerStateMap
{
public:
    State* GetState(Lexer* lexer);
    static LexerStateMap& Instance();
private:
    std::mutex mtx;
    std::map<Lexer*, State*> stateMap;
    std::vector<std::unique_ptr<State>> states;
};

} // namespace otava::pp::state
