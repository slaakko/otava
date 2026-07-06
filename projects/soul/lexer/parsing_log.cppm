export module soul.lexer.parsing_log;

import std;

export namespace soul::lexer {

class ParsingLog
{
public:
    ParsingLog() noexcept;
    ParsingLog(int maxLineLength_) noexcept;
    virtual ~ParsingLog();
    virtual void IncIndent() = 0;
    virtual void DecIndent() = 0;
    virtual void WriteBeginRule(const std::string& ruleName) = 0;
    virtual void WriteEndRule(const std::string& ruleName) = 0;
    virtual void WriteTry(const std::string& s) = 0;
    virtual void WriteSuccess(const std::string& match) = 0;
    virtual void WriteFail() = 0;
    virtual int MaxLineLength() const noexcept;
private:
    int maxLineLength;
};

} // namespace soul::lexer
