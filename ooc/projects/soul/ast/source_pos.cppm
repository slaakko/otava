export module soul.ast.source_pos;

import std;

export namespace soul::ast {

struct SourcePos
{
    inline SourcePos() noexcept : pos(-1), file(-1), line(0), col(0) {}
    inline SourcePos(int line_, int col_) noexcept : pos(-1), file(-1), line(line_), col(col_) {}
    inline SourcePos(int file_, int line_, int col_) noexcept : pos(-1), file(file_), line(line_), col(col_) {}
    inline SourcePos(std::int64_t pos_, int line_, int col_) noexcept : pos(pos_), file(-1), line(line_), col(col_) {}
    inline SourcePos(std::int64_t pos_, int file_, int line_, int col_) noexcept : pos(pos_), file(file_), line(line_), col(col_) {}
    inline bool IsValid() const noexcept { return line != 0; }
    std::int64_t pos;
    int file;
    int line;
    int col;
};

inline bool operator==(const SourcePos& left, const SourcePos& right) noexcept
{
    return left.file == right.file && left.line == right.line && left.col == right.col;
}

inline bool operator<(const SourcePos& left, const SourcePos& right) noexcept
{
    if (left.file < right.file) return true;
    if (left.file > right.file) return false;
    if (left.line < right.line) return true;
    if (left.line > right.line) return false;
    return left.col < right.col;
}

std::string ToString(const SourcePos& sourcePos);

} // namespace soul::ast
