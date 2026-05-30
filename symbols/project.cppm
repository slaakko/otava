// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.project;

import std;

export namespace otava::symbols {

class SymbolsProject
{
public:
    virtual ~SymbolsProject();
    virtual bool HasDefine(const std::string& symbol) const noexcept = 0;
};

} // otava::symbols
