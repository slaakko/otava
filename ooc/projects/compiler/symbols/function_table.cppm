// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_table;

import std;

export namespace otava::symbols {

class FunctionTable
{
public:
    FunctionTable();
    void AddFunction(std::uint64_t fnId, const std::string& fnName);
    std::string GetFunction(std::uint64_t id) const;
    void Write(const std::string& filePath);
    void Read(const std::string& filePath);
private:
    std::map<std::uint64_t, std::string> table;
};

FunctionTable* GetFunctionTable();
void SetFunctionTable(FunctionTable* functionTable_);

} // namespace otava::symbols
