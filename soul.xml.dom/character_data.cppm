// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.character.data;

import std;
import soul.xml.node;

export namespace soul::xml {

std::string XmlCharDataEscape(const std::string& text);

class CharacterData : public Node
{
public:
    CharacterData(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    CharacterData(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_, const std::string& data_);
    inline const std::string& Data() const noexcept { return data; }
    inline std::string& Data() noexcept { return data; }
    void Write(util::CodeFormatter& formatter) override;
    bool ValueContainsNewLine() const noexcept override;
private:
    std::string data;
};

} // namespace soul::xml
