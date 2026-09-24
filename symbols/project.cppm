// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.project;

import otava.symbols.id;
import std;

export namespace otava::symbols {

class SymbolsProject
{
public:
    virtual ~SymbolsProject();
    virtual bool HasDefine(const std::string& symbol) const noexcept = 0;
    virtual ProjectId GetProjectId() const noexcept = 0;
    virtual otava::symbols::SymbolId GetIrId(const std::string& groupFullName, otava::symbols::Cardinality arity) = 0;
    virtual void SetIrId(const std::string& groupFullName, otava::symbols::Cardinality arity, otava::symbols::SymbolId irId) = 0;
};

} // otava::symbols
