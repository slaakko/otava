export module soul.ast.span;

import std;

export namespace soul::ast {

struct Span
{
    inline Span() noexcept : pos(0), len(0) {}
    inline Span(int pos_, int len_) noexcept : pos(pos_), len(len_) {}
    inline bool IsValid() const noexcept { return len != 0; }
    inline bool Contains(int pos_) const noexcept { return pos_ >= pos && pos_ < pos + len; }
    void Union(const Span& that) noexcept;
    int pos;
    int len;
};

constexpr bool operator==(const Span& left, const Span& right) noexcept
{
    return left.pos == right.pos && left.len == right.len;
}

struct FullSpan
{
    inline FullSpan() noexcept : fileIndex(-1), span() {}
    inline FullSpan(int fileIndex_, const Span& span_) noexcept : fileIndex(fileIndex_), span(span_) {}
    inline bool IsValid() const noexcept { return fileIndex != -1 && span.IsValid(); }
    int fileIndex;
    Span span;
};

inline bool operator==(const FullSpan& left, const FullSpan& right) noexcept
{
    return left.fileIndex == right.fileIndex && left.span == right.span;
}

struct LineColLen
{
    inline LineColLen() noexcept : line(0), col(0), len(0) {}
    inline LineColLen(int line_, int col_) noexcept : line(line_), col(col_), len(0) {}
    inline LineColLen(int line_, int col_, int len_) noexcept : line(line_), col(col_), len(len_) {}
    inline bool IsValid() const noexcept { return line != 0; }
    int line;
    int col;
    int len;
};

bool operator==(const LineColLen& left, const LineColLen& right) noexcept;
bool operator<(const LineColLen& left, const LineColLen& right) noexcept;

LineColLen SpanToLineColLen(const Span& span, const std::vector<int>& lineStarts) noexcept;
int LineColLenToPos(const LineColLen& lineColLen, const std::vector<int>& lineStarts) noexcept;
std::string ToString(const Span& span);

} // namespace soul::ast
