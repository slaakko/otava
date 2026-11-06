// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_table;

import util.file_stream;
import otava.symbols.exception;

namespace otava::symbols {

FunctionTable::FunctionTable() : table()
{
}

void FunctionTable::AddFunction(std::uint64_t fnId, const std::string& fnName)
{
    table[fnId] = fnName;
}

std::string FunctionTable::GetFunction(std::uint64_t id) const
{
    auto it = table.find(id);
    if (it != table.end())
    {
        return it->second;
    }
    return std::string();
}

void FunctionTable::Write(const std::string& filePath)
{
    std::ofstream file(filePath);
    if (!file)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("could not create file '" + filePath + "'");
    }
    for (const auto& entry : table)
    {
        file << entry.first << ";" << entry.second << "\n";
    }
}

void FunctionTable::Read(const std::string& filePath)
{
    std::string tableStr = util::ReadFile(filePath);
    int state = 0;
    std::string ids;
    std::string name;
    for (char c : tableStr)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '\r' || c == '\n')
            {
                state = 1;
            }
            else if (c == ';')
            {
                state = 2;
            }
            else
            {
                ids.append(1, c);
            }
            break;
        }
        case 1:
        {
            if (c != '\r' && c != '\n')
            {
                ids.append(1, c);
                state = 0;
            }
            break;
        }
        case 2:
        {
            if (c != '\r' && c != '\n')
            {
                name.append(1, c);
            }
            else
            {
                std::uint64_t id = std::stoull(ids);
                table[id] = name;
                ids.clear();
                name.clear();
                state = 1;
            }
            break;
        }
        }
    }
}

void FunctionTable::Add(const FunctionTable& that)
{
    for (const auto& p : that.table)
    {
        AddFunction(p.first, p.second);
    }
}

FunctionTable* functionTable = nullptr;

FunctionTable* GetFunctionTable() noexcept
{
    return functionTable;
}

void SetFunctionTable(FunctionTable* functionTable_) noexcept
{
    functionTable = functionTable_;
}

} // namespace otava::symbols
