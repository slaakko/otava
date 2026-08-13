// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.declaration;

import util.code_formatter;
import std;

export namespace otava::assembly {

class Declaration
{
public:
    Declaration(const std::string& name_);
    virtual ~Declaration();
    inline const std::string& Name() const noexcept { return name; }
    virtual void Write(util::CodeFormatter& formatter) = 0;
private:
    std::string name;
};

} // namespace otava::assembly
