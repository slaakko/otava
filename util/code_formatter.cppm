// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.code.formatter;

import std;

export namespace util {

class CodeFormatter
{
public:
    CodeFormatter(std::ostream& stream_) noexcept : 
        stream(stream_), indent(0), indentSize(4), atBeginningOfLine(true), line(1), start(false), preserveSpace(false), contentCount(0), logging(false) {}
    inline int Indent() const noexcept { return indent; }
    inline int IndentSize() const noexcept { return indentSize; }
    inline void SetIndentSize(int indentSize_) noexcept { indentSize = indentSize_; }
    inline int CurrentIndent() const noexcept { return indentSize * indent; }
    void Write(const std::string& text);
    void WriteLine(const std::string& text);
    void NewLine();
    void WriteLine() { NewLine(); }
    void Flush();
    inline void IncIndent() noexcept
    {
        ++indent;
    }
    inline void DecIndent() noexcept
    {
        --indent;
    }
    inline int Line() const noexcept { return line; }
    inline void SetLine(int line_) noexcept { line = line_; }
    inline bool Start() const noexcept { return start; }
    inline void SetStart() noexcept { start = true; }
    inline void ResetStart() noexcept { start = false; }
    inline void SetStartText(const std::string& startText_) { startText = startText_; }
    inline const std::string& StartText() const noexcept { return startText; }
    inline bool PreserveSpace() const noexcept { return preserveSpace; }
    inline void SetPreserveSpace(bool preserveSpace_) noexcept { preserveSpace = preserveSpace_; }
    inline void SetLogging() noexcept { logging = true; }
    inline void BeginContent() noexcept { ++contentCount; }
    inline void EndContent() noexcept { --contentCount; }
private:
    std::ostream& stream;
    int indent;
    int indentSize;
    bool atBeginningOfLine;
    int line;
    bool start;
    std::string startText;
    bool preserveSpace;
    int contentCount;
    bool logging;
};

using CoutType = std::ostream;
using StandardEndLine = CoutType& (*)(CoutType&);

CodeFormatter& operator<<(CodeFormatter& f, StandardEndLine manip);
CodeFormatter& operator<<(CodeFormatter& f, const std::string& s);
CodeFormatter& operator<<(CodeFormatter& f, const char* s);
CodeFormatter& operator<<(CodeFormatter& f, char c);
CodeFormatter& operator<<(CodeFormatter& f, bool b);
CodeFormatter& operator<<(CodeFormatter& f, int x);
CodeFormatter& operator<<(CodeFormatter& f, double x);
CodeFormatter& operator<<(CodeFormatter& f, std::int64_t x);
CodeFormatter& operator<<(CodeFormatter& f, std::uint64_t x);

void WriteUtf8(std::ostream& s, const std::string& str);
bool IsHandleRedirected(int handle);

} // namespace util
