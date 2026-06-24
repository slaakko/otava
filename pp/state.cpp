// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.pp.state;

import otava.token;
import otava.ast;
import util.text_util;
import util.unicode;

namespace otava::pp::state {

Section::Section(Section* parent_) : parent(parent_), skip(false), processed(false)
{
}

State::State(Lexer* lexer_) : lxr(lexer_)
{
    currentSection.reset(new Section(nullptr));
    Define("OTAVA", 1);
}

void State::If(bool value)
{
    PushSection();
    bool skip = false;
    currentSection->SetProcessed(false);
    if (currentSection->Parent()->Skip())
    {
        skip = true;
    }
    else
    {
        skip = !value;
        if (!skip)
        {
            currentSection->SetProcessed(true);
        }
    }
    currentSection->SetSkip(skip);
    if (skip)
    {
        SkipSection();
    }
}

void State::Elif(bool value)
{
    bool skip = false;
    if (!currentSection->Parent()->Skip() && !currentSection->Processed())
    {
        skip = !value;
        if (!skip)
        {
            currentSection->SetProcessed(true);
        }
    }
    else
    {
        skip = true;
    }
    currentSection->SetSkip(skip);
    if (skip)
    {
        SkipSection();
    }
}

void State::Ifdef(const std::string& symbol)
{
    PushSection();
    bool skip = false;
    currentSection->SetProcessed(false);
    if (currentSection->Parent()->Skip())
    {
        skip = true;
    }
    else
    {
        skip = !Defined(symbol);
        if (!skip)
        {
            currentSection->SetProcessed(true);
        }
    }
    currentSection->SetSkip(skip);
    if (skip)
    {
        SkipSection();
    }
}

void State::Ifndef(const std::string& symbol)
{
    PushSection();
    bool skip = false;
    currentSection->SetProcessed(false);
    if (currentSection->Parent()->Skip())
    {
        skip = true;
    }
    else
    {
        skip = Defined(symbol);
        if (!skip)
        {
            currentSection->SetProcessed(true);
        }
    }
    currentSection->SetSkip(skip);
    if (skip)
    {
        SkipSection();
    }
}

void State::Else()
{
    bool skip = false;
    if (!currentSection->Parent()->Skip() && !currentSection->Processed())
    {
        skip = false;
        currentSection->SetProcessed(true);
    }
    else
    {
        skip = true;
    }
    currentSection->SetSkip(skip);
    if (skip)
    {
        SkipSection();
    }
}

void State::Endif()
{
    PopSection();
    if (currentSection->Skip())
    {
        SkipSection();
    }
}

void State::Include()
{
    if (!currentSection->Skip())
    {
        std::string directiveStr = util::Trim(util::ToUtf8(lxr->CurrentToken().match.ToString()));
        std::cout << "warning: include directive '" << directiveStr << "' skipped" << std::endl;
    }
}

void State::Define(const std::string& symbol, std::int64_t value)
{
    if (!currentSection->Skip())
    {
        symbolMap[symbol] = value;
    }
}

void State::Undef(const std::string& symbol)
{
    if (currentSection->Skip())
    {
        SkipSection();
    }
    else
    {
        symbolMap.erase(symbol);
    }
}

bool State::Defined(const std::string& symbol) const
{
    auto it = symbolMap.find(symbol);
    if (it != symbolMap.cend())
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::int64_t State::GetSymbolValue(const std::string& symbol) const
{
    auto it = symbolMap.find(symbol);
    if (it != symbolMap.cend())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

void State::PushSection()
{
    sectionStack.push(std::move(currentSection));
    currentSection.reset(new Section(sectionStack.top().get()));
}

void State::PopSection()
{
    if (sectionStack.empty())
    {
        otava::ast::SetExceptionThrown();
        throw std::runtime_error("otava.pp.state: error: section stack is empty");
    }
    currentSection = std::move(sectionStack.top());
    sectionStack.pop();
}

void State::SkipSection()
{
    lxr->Skip(true);
    lxr->Increment();
    while (lxr->CurrentToken().id != soul::lexer::END_TOKEN && lxr->CurrentToken().id != otava::token::PP_LINE)
    {
        lxr->Increment();
    }
    lxr->Skip(false);
    if (lxr->CurrentToken().id == otava::token::PP_LINE)
    {
        lxr->PreprocessCurrentToken();
    }
}

State* LexerStateMap::GetState(Lexer* lxr)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = stateMap.find(lxr);
    if (it != stateMap.cend())
    {
        return it->second;
    }
    else
    {
        State* state = new State(lxr);
        stateMap[lxr] = state;
        states.push_back(std::unique_ptr<State>(state));
        return state;
    }
}

LexerStateMap& LexerStateMap::Instance()
{
    static LexerStateMap instance;
    return instance;
}

} // namespace otava::pp::state
