// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.error;

import std;
import soul.lexer.file.map;
import soul.ast.source.pos;

export namespace soul::xml {

class XmlException : public std::runtime_error
{
public:
    XmlException(const std::string& errorMessage_, const soul::ast::SourcePos& sourcePos_);
    inline const soul::ast::SourcePos& GetSourcePos() const noexcept { return sourcePos; }
private:
    soul::ast::SourcePos sourcePos;
};

} // namespace soul::xml
