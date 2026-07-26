export module soul.xml.entity_reference;

import std;
import soul.xml.character_data;
import util.code_formatter;
import soul.ast.source_pos;

export namespace soul::xml {

class Visitor;

class EntityReference : public CharacterData
{
public:
    EntityReference(const soul::ast::SourcePos& sourcePos_);
    EntityReference(const soul::ast::SourcePos& sourcePos_, const std::string& entityRef_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

EntityReference* MakeEntityReference(const std::string& entityRef);

} // namespace soul::xml
